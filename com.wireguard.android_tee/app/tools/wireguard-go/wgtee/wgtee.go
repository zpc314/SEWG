
package wgtee

/*
#cgo CFLAGS: -I.
#cgo LDFLAGS: -llog
#cgo LDFLAGS: -L./ -lteeclient
#include <stdlib.h>
#include <android/log.h>
#include <stdio.h>
#include <string.h>
#include "tsca_comm.h"
#include "param.h"
#include "err.h"

const char *teeAppName = "sccore64";

ts_send_t mreq;
ts_rsp_t mrsp;

int32_t Core_Init()
{
    int32_t rst = ts_start_app(teeAppName,sizeof(ts_send_t)+sizeof(ts_rsp_t)+200);
    return rst;
}

int32_t Core_GenerateKeyPair(void **pubkey, int32_t *ID)
{
    mreq.cmd = GenerateKeyPair_CMD;
    *pubkey = (BYTE *)malloc(CUR25519_PUB_KEY_LENGTH);
    //memset(*pubkey, 1, CUR25519_PUB_KEY_LENGTH);
    int32_t ret = SAR_TEECOMMERR;
    mrsp.ret = SAR_TEECOMMERR;

    if (0 == ts_ca_ui(teeAppName, SET_PIN_UI, &mreq, sizeof(ts_send_t), &mrsp, sizeof(ts_rsp_t))) {
        if (SAR_OK == (ret = mrsp.ret)) {
            GenerateKeyPair_rsp_t *rsp = (GenerateKeyPair_rsp_t *) (mrsp.data);
            *ID = rsp->ID;
            memcpy(*pubkey, rsp->pubKey, CUR25519_PUB_KEY_LENGTH);
            ret = SAR_OK;
        }
    }
    //ts_shutdown_app();
    return ret;
}

int32_t Core_GetPublickeyByID(void **pubkey, int32_t ID)
{
    //ts_start_app(teeAppName,sizeof(ts_send_t)+sizeof(ts_rsp_t)+200);
    GetPublickeyByID_send_t *send = (GetPublickeyByID_send_t *) (mreq.data);
    mreq.cmd = GetPublickeyByID_CMD;
    send->ID = ID;
    *pubkey = (BYTE *)malloc(CUR25519_PUB_KEY_LENGTH);
    //memset(*pubkey, 1, CUR25519_PUB_KEY_LENGTH);
    int32_t ret = SAR_TEECOMMERR;
    mrsp.ret = SAR_TEECOMMERR;

    if (0 == ts_ca_send(teeAppName, &mreq, sizeof(ts_send_t), &mrsp, sizeof(ts_rsp_t))) {
        if (SAR_OK == (ret = mrsp.ret)) {
            GetPublickeyByID_rsp_t *rsp = (GetPublickeyByID_rsp_t *) (mrsp.data);
            memcpy(*pubkey, rsp->pubKey, CUR25519_PUB_KEY_LENGTH);
            //ret = SAR_OK;
        }
    }
    //ts_shutdown_app();
    return ret;
}

int32_t Core_CurveMul(void **rst, void *point, int32_t ID)
{
    //ts_start_app(teeAppName,sizeof(ts_send_t)+sizeof(ts_rsp_t)+200);
    CurveMul_send_t *send = (CurveMul_send_t *) (mreq.data);
    mreq.cmd = CurveMul_CMD;
    send->ID = ID;
    memcpy(send->point, point, CUR25519_PUB_KEY_LENGTH);
    uint8_t *mp = (uint8_t *)point;
    //for(int i = 0; i < CUR25519_PUB_KEY_LENGTH; i++) {
      //  __android_log_print(ANDROID_LOG_ERROR, "Core_CurveMul point", "point[%d]: %x", i, mp[i]);
    //}

    *rst = (BYTE *)malloc(CUR25519_PUB_KEY_LENGTH);
    //memset(*pubkey, 1, CUR25519_PUB_KEY_LENGTH);
    int32_t ret = SAR_TEECOMMERR;
    mrsp.ret = SAR_TEECOMMERR;

    if (0 == ts_ca_ui(teeAppName, INPUT_PIN_UI, &mreq, sizeof(ts_send_t), &mrsp, sizeof(ts_rsp_t))) {
        if (SAR_OK == (ret = mrsp.ret)) {
            CurveMul_rsp_t *rsp = (CurveMul_rsp_t *) (mrsp.data);
            memcpy(*rst, rsp->rst, CUR25519_PUB_KEY_LENGTH);
            ret = SAR_OK;
        }
    }
    //if (0 == ts_ca_send(teeAppName, &mreq, sizeof(ts_send_t), &mrsp, sizeof(ts_rsp_t))) {
    //        if (SAR_OK == (ret = mrsp.ret)) {
    //            CurveMul_rsp_t *rsp = (CurveMul_rsp_t *) (mrsp.data);
    //            memcpy(*rst, rsp->rst, CUR25519_PUB_KEY_LENGTH);
    //            ret = SAR_OK;
    //        }
    //    }
    //ts_shutdown_app();
    return ret;
}
*/
import "C"

import (
    "unsafe"
)

func cArrayToByteSlice(array unsafe.Pointer, size int) []byte {
    var arrayptr = uintptr(array)
    var byteSlice = make([]byte, size)
    //C.__android_log_write(C.ANDROID_LOG_ERROR, C.CString("WireGuard/GoBackend/Stacktrace"), C.CString("CArrayToByteSlice start"))

    for i := 0; i < len(byteSlice); i ++ {
            byteSlice[i] = byte(*(*C.uchar)(unsafe.Pointer(arrayptr)))
            //C.__android_log_write(C.ANDROID_LOG_ERROR, C.CString("WireGuard/GoBackend/Stacktrace"), C.CString("CArrayToByteSlice" + strconv.FormatInt(int64(byteSlice[i]),10)+ strconv.FormatInt(int64(i),10)))
            arrayptr ++
    }

    return byteSlice
}

func byteSliceToCArray(byteSlice []byte) unsafe.Pointer {
    var array = unsafe.Pointer(C.calloc(C.size_t(len(byteSlice)), 1))
    var arrayptr = uintptr(array)
    //C.__android_log_write(C.ANDROID_LOG_ERROR, C.CString("WireGuard/GoBackend/Stacktrace"), C.CString("ByteSliceToCArray start"))

    for i := 0; i < len(byteSlice); i ++ {
           //C.__android_log_write(C.ANDROID_LOG_ERROR, C.CString("WireGuard/GoBackend/Stacktrace"), C.CString("ByteSliceToCArray"+ strconv.FormatInt(int64(byteSlice[i]),10) + strconv.FormatInt(int64(i),10)))

           *(*C.uchar)(unsafe.Pointer(arrayptr)) = C.uchar(byteSlice[i])
           arrayptr ++
    }

    return array
}

func TeeInit() int32 {
    var rst = C.Core_Init()
    //C.__android_log_write(C.ANDROID_LOG_ERROR, C.CString("WireGuard/GoBackend/Stacktrace"), C.CString("Core_Init finished"))
    return int32(rst)
}

func TeeGetPubKeyByID(id int32) []byte {
    var pubkey unsafe.Pointer
    ret := C.Core_GetPublickeyByID(&pubkey, C.int(id))
    if ret != 0 {
        //return *C.uchar(0);
    }
    return cArrayToByteSlice(pubkey, 32)
}

func TeeGenerateKeyPair() ([]byte, int32) {
    var pubkey unsafe.Pointer
    var id C.int
    //C.__android_log_write(C.ANDROID_LOG_ERROR, C.CString("WireGuard/GoBackend/Stacktrace"), C.CString("Core_GenerateKeyPair started"))
    ret := C.Core_GenerateKeyPair(&pubkey, &id)
    //C.__android_log_write(C.ANDROID_LOG_ERROR, C.CString("WireGuard/GoBackend/Stacktrace"), C.CString("Core_GenerateKeyPair finished"))
    if ret != 0 {
        //return *C.uchar(0);
    }
    rst := cArrayToByteSlice(pubkey, 32)
    //for i:=0; i < len(rst); i++ {
      //  C.__android_log_write(C.ANDROID_LOG_ERROR, C.CString("WireGuard/TeeGenerateKeyPair"), C.CString("i:"+ strconv.FormatInt(int64(i),10) + " " + strconv.FormatInt(int64(rst[i]),16)))
    //}
    return rst, int32(id)
}

func TeeCurveMul(point []byte, id int32) []byte {
    var rst unsafe.Pointer
    //C.__android_log_write(C.ANDROID_LOG_ERROR, C.CString("WireGuard/GoBackend/Stacktrace"), C.CString("Core_CurveMul started"))
    //for i:=0; i < 32; i++ {
      //  C.__android_log_write(C.ANDROID_LOG_ERROR, C.CString("WireGuard/TeeCurveMul"), C.CString("i:"+ strconv.FormatInt(int64(i),10) + " " + strconv.FormatInt(int64(point[i]),16)))
    // }
    //ret := C.Core_GenerateKeyPair(&pubkey, &id)
    ret := C.Core_CurveMul(&rst, byteSliceToCArray(point), C.int(id))
    //C.__android_log_write(C.ANDROID_LOG_ERROR, C.CString("WireGuard/GoBackend/Stacktrace"), C.CString("Core_CurveMul finished"))
    if ret != 0 {
        //return *C.uchar(0);
    }
    rr := cArrayToByteSlice(rst, 32)
    //for i:=0; i < 32; i++ {
      //  C.__android_log_write(C.ANDROID_LOG_ERROR, C.CString("WireGuard/TeeCurveMul"), C.CString("i:"+ strconv.FormatInt(int64(i),10) + " " + strconv.FormatInt(int64(rr[i]),16)))
    //}

    return rr
}