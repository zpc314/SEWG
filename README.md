# SEWG, contact us at [mail](zhangpengcheng@iie.ac.cn)
Security-enhanced WireGuard for Android

This work is still in progress, don't panic if you encounter any bugs or find it doesn’t work (actually, there are very strict requirements to make it work).

1, com.wireguard.android_443_orign：
Original version of WireGuard. We did some fix to fit the Chinese network environment.

2, com.wireguard.android_tee: 
The tee version. Don't bother to run it on a unprepared Android device, it won't do nothing without TA running in TEE.

3, gopath:
Some offline data required while building codes above. 

4, TrustApp:
Codes in TA.

5, wireguard_kernel:
The kernel version WireGuard for Android.
