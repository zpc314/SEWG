/* SPDX-License-Identifier: GPL-2.0
 *
 * Copyright (C) 2017-2018 WireGuard LLC. All Rights Reserved.
 */

package tun

/* Implementation of the TUN device interface for linux
 */

import (
	"bytes"
	"errors"
	"fmt"
	"git.zx2c4.com/wireguard-go/rwcancel"
	"golang.org/x/net/ipv6"
	"golang.org/x/sys/unix"
	"net"
	"os"
	"strconv"
	"sync"
	"time"
	"unsafe"
)

const (
	cloneDevicePath = "/dev/net/tun"
	ifReqSize       = unix.IFNAMSIZ + 64
)

type nativeTun struct {
	tunFile                 *os.File
	fd                      uintptr
	fdCancel                *rwcancel.RWCancel
	index                   int32         // if index
	name                    string        // name of interface
	errors                  chan error    // async error handling
	events                  chan TUNEvent // device related events
	nopi                    bool          // the device was pased IFF_NO_PI
	netlinkSock             int
	netlinkCancel           *rwcancel.RWCancel
	hackListenerClosed      sync.Mutex
	statusListenersShutdown chan struct{}
}

func (tun *nativeTun) File() *os.File {
	return tun.tunFile
}

func (tun *nativeTun) routineHackListener() {
	defer tun.hackListenerClosed.Unlock()
	/* This is needed for the detection to work across network namespaces
	 * If you are reading this and know a better method, please get in touch.
	 */
	fd := int(tun.fd)
	for {
		_, err := unix.Write(fd, nil)
		switch err {
		case unix.EINVAL:
			tun.events <- TUNEventUp
		case unix.EIO:
			tun.events <- TUNEventDown
		default:
			return
		}
		select {
		case <-time.After(time.Second):
		case <-tun.statusListenersShutdown:
			return
		}
	}
}

func createNetlinkSocket() (int, error) {
	sock, err := unix.Socket(unix.AF_NETLINK, unix.SOCK_RAW, unix.NETLINK_ROUTE)
	if err != nil {
		return -1, err
	}
	saddr := &unix.SockaddrNetlink{
		Family: unix.AF_NETLINK,
		Groups: uint32((1 << (unix.RTNLGRP_LINK - 1)) | (1 << (unix.RTNLGRP_IPV4_IFADDR - 1)) | (1 << (unix.RTNLGRP_IPV6_IFADDR - 1))),
	}
	err = unix.Bind(sock, saddr)
	if err != nil {
		return -1, err
	}
	return sock, nil
}

func (tun *nativeTun) routineNetlinkListener() {
	defer func() {
		unix.Close(tun.netlinkSock)
		tun.hackListenerClosed.Lock()
		close(tun.events)
	}()

	for msg := make([]byte, 1<<16); ; {

		var err error
		var msgn int
		for {
			msgn, _, _, _, err = unix.Recvmsg(tun.netlinkSock, msg[:], nil, 0)
			if err == nil || !rwcancel.RetryAfterError(err) {
				break
			}
			if !tun.netlinkCancel.ReadyRead() {
				tun.errors <- fmt.Errorf("netlink socket closed: %s", err.Error())
				return
			}
		}
		if err != nil {
			tun.errors <- fmt.Errorf("failed to receive netlink message: %s", err.Error())
			return
		}

		select {
		case <-tun.statusListenersShutdown:
			return
		default:
		}

		for remain := msg[:msgn]; len(remain) >= unix.SizeofNlMsghdr; {

			hdr := *(*unix.NlMsghdr)(unsafe.Pointer(&remain[0]))

			if int(hdr.Len) > len(remain) {
				break
			}

			switch hdr.Type {
			case unix.NLMSG_DONE:
				remain = []byte{}

			case unix.RTM_NEWLINK:
				info := *(*unix.IfInfomsg)(unsafe.Pointer(&remain[unix.SizeofNlMsghdr]))
				remain = remain[hdr.Len:]

				if info.Index != tun.index {
					// not our interface
					continue
				}

				if info.Flags&unix.IFF_RUNNING != 0 {
					tun.events <- TUNEventUp
				}

				if info.Flags&unix.IFF_RUNNING == 0 {
					tun.events <- TUNEventDown
				}

				tun.events <- TUNEventMTUUpdate

			default:
				remain = remain[hdr.Len:]
			}
		}
	}
}

func (tun *nativeTun) isUp() (bool, error) {
	inter, err := net.InterfaceByName(tun.name)
	return inter.Flags&net.FlagUp != 0, err
}

func getDummySock() (int, error) {
	return unix.Socket(
		unix.AF_INET,
		unix.SOCK_DGRAM,
		0,
	)
}

func getIFIndex(name string) (int32, error) {
	fd, err := getDummySock()
	if err != nil {
		return 0, err
	}

	defer unix.Close(fd)

	var ifr [ifReqSize]byte
	copy(ifr[:], name)
	_, _, errno := unix.Syscall(
		unix.SYS_IOCTL,
		uintptr(fd),
		uintptr(unix.SIOCGIFINDEX),
		uintptr(unsafe.Pointer(&ifr[0])),
	)

	if errno != 0 {
		return 0, errno
	}

	return *(*int32)(unsafe.Pointer(&ifr[unix.IFNAMSIZ])), nil
}

func (tun *nativeTun) setMTU(n int) error {

	// open datagram socket

	fd, err := unix.Socket(
		unix.AF_INET,
		unix.SOCK_DGRAM,
		0,
	)

	if err != nil {
		return err
	}

	defer unix.Close(fd)

	// do ioctl call

	var ifr [ifReqSize]byte
	copy(ifr[:], tun.name)
	*(*uint32)(unsafe.Pointer(&ifr[unix.IFNAMSIZ])) = uint32(n)
	_, _, errno := unix.Syscall(
		unix.SYS_IOCTL,
		uintptr(fd),
		uintptr(unix.SIOCSIFMTU),
		uintptr(unsafe.Pointer(&ifr[0])),
	)

	if errno != 0 {
		return errors.New("failed to set MTU of TUN device")
	}

	return nil
}

func (tun *nativeTun) MTU() (int, error) {

	// open datagram socket

	fd, err := unix.Socket(
		unix.AF_INET,
		unix.SOCK_DGRAM,
		0,
	)

	if err != nil {
		return 0, err
	}

	defer unix.Close(fd)

	// do ioctl call

	var ifr [ifReqSize]byte
	copy(ifr[:], tun.name)
	_, _, errno := unix.Syscall(
		unix.SYS_IOCTL,
		uintptr(fd),
		uintptr(unix.SIOCGIFMTU),
		uintptr(unsafe.Pointer(&ifr[0])),
	)
	if errno != 0 {
		return 0, errors.New("failed to get MTU of TUN device: " + strconv.FormatInt(int64(errno), 10))
	}

	return int(*(*int32)(unsafe.Pointer(&ifr[unix.IFNAMSIZ]))), nil
}

func (tun *nativeTun) Name() (string, error) {

	var ifr [ifReqSize]byte
	_, _, errno := unix.Syscall(
		unix.SYS_IOCTL,
		tun.fd,
		uintptr(unix.TUNGETIFF),
		uintptr(unsafe.Pointer(&ifr[0])),
	)
	if errno != 0 {
		return "", errors.New("failed to get name of TUN device: " + strconv.FormatInt(int64(errno), 10))
	}
	nullStr := ifr[:]
	i := bytes.IndexByte(nullStr, 0)
	if i != -1 {
		nullStr = nullStr[:i]
	}
	tun.name = string(nullStr)
	return tun.name, nil
}

func (tun *nativeTun) Write(buff []byte, offset int) (int, error) {

	if tun.nopi {
		buff = buff[offset:]
	} else {
		// reserve space for header

		buff = buff[offset-4:]

		// add packet information header

		buff[0] = 0x00
		buff[1] = 0x00

		if buff[4]>>4 == ipv6.Version {
			buff[2] = 0x86
			buff[3] = 0xdd
		} else {
			buff[2] = 0x08
			buff[3] = 0x00
		}
	}

	// write

	return tun.tunFile.Write(buff)
}

func (tun *nativeTun) doRead(buff []byte, offset int) (int, error) {
	select {
	case err := <-tun.errors:
		return 0, err
	default:
		if tun.nopi {
			return tun.tunFile.Read(buff[offset:])
		} else {
			buff := buff[offset-4:]
			n, err := tun.tunFile.Read(buff[:])
			if n < 4 {
				return 0, err
			}
			return n - 4, err
		}
	}
}

func (tun *nativeTun) Read(buff []byte, offset int) (int, error) {
	for {
		n, err := tun.doRead(buff, offset)
		if err == nil || !rwcancel.RetryAfterError(err) {
			return n, err
		}
		if !tun.fdCancel.ReadyRead() {
			return 0, errors.New("tun device closed")
		}
	}
}

func (tun *nativeTun) Events() chan TUNEvent {
	return tun.events
}

func (tun *nativeTun) Close() error {
	var err1 error
	if tun.statusListenersShutdown != nil {
		close(tun.statusListenersShutdown)
		if tun.netlinkCancel != nil {
			err1 = tun.netlinkCancel.Cancel()
		}
	} else if tun.events != nil {
		close(tun.events)
	}
	err2 := tun.tunFile.Close()
	err3 := tun.fdCancel.Cancel()

	if err1 != nil {
		return err1
	}
	if err2 != nil {
		return err2
	}
	return err3
}

func CreateTUN(name string, mtu int) (TUNDevice, error) {

	// open clone device

	// HACK: we open it as a raw Fd first, so that f.nonblock=false
	// when we make it into a file object.
	nfd, err := unix.Open(cloneDevicePath, os.O_RDWR, 0)
	if err != nil {
		return nil, err
	}

	err = unix.SetNonblock(nfd, true)
	if err != nil {
		return nil, err
	}

	fd := os.NewFile(uintptr(nfd), cloneDevicePath)
	if err != nil {
		return nil, err
	}

	// create new device

	var ifr [ifReqSize]byte
	var flags uint16 = unix.IFF_TUN // | unix.IFF_NO_PI (disabled for TUN status hack)
	nameBytes := []byte(name)
	if len(nameBytes) >= unix.IFNAMSIZ {
		return nil, errors.New("interface name too long")
	}
	copy(ifr[:], nameBytes)
	*(*uint16)(unsafe.Pointer(&ifr[unix.IFNAMSIZ])) = flags

	_, _, errno := unix.Syscall(
		unix.SYS_IOCTL,
		fd.Fd(),
		uintptr(unix.TUNSETIFF),
		uintptr(unsafe.Pointer(&ifr[0])),
	)
	if errno != 0 {
		return nil, errno
	}

	return CreateTUNFromFile(fd, mtu)
}

func CreateTUNFromFile(file *os.File, mtu int) (TUNDevice, error) {
	tun := &nativeTun{
		tunFile:                 file,
		fd:                      file.Fd(),
		events:                  make(chan TUNEvent, 5),
		errors:                  make(chan error, 5),
		statusListenersShutdown: make(chan struct{}),
		nopi:                    false,
	}
	var err error

	tun.fdCancel, err = rwcancel.NewRWCancel(int(tun.fd))
	if err != nil {
		tun.tunFile.Close()
		return nil, err
	}

	_, err = tun.Name()
	if err != nil {
		tun.tunFile.Close()
		return nil, err
	}

	// start event listener

	tun.index, err = getIFIndex(tun.name)
	if err != nil {
		return nil, err
	}

	tun.netlinkSock, err = createNetlinkSocket()
	if err != nil {
		tun.tunFile.Close()
		return nil, err
	}
	tun.netlinkCancel, err = rwcancel.NewRWCancel(tun.netlinkSock)
	if err != nil {
		tun.tunFile.Close()
		return nil, err
	}

	tun.hackListenerClosed.Lock()
	go tun.routineNetlinkListener()
	go tun.routineHackListener() // cross namespace

	err = tun.setMTU(mtu)
	if err != nil {
		tun.Close()
		return nil, err
	}

	return tun, nil
}
