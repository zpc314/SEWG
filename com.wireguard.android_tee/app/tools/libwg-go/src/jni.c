/* SPDX-License-Identifier: Apache-2.0
 *
 * Copyright © 2017-2018 Jason A. Donenfeld <Jason@zx2c4.com>. All Rights Reserved.
 */

#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <android/log.h>
#define LOG_TAG  "C_TAG"
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

struct go_string { const char *str; long n; };
extern int wgTurnOn(struct go_string ifname, int tun_fd, struct go_string settings);
extern void wgTurnOff(int handle);
extern int wgGetSocketV4(int handle);
extern int wgGetSocketV6(int handle);
extern char *wgVersion();
extern unsigned char *getPublicKeyByID(int id);
extern unsigned char *generateKeyPair(int *id);

JNIEXPORT jint JNICALL Java_com_wireguard_util_CoreLib_wgTurnOn(JNIEnv *env, jclass c, jstring ifname, jint tun_fd, jstring settings)
{
	const char *ifname_str = (*env)->GetStringUTFChars(env, ifname, 0);
	size_t ifname_len = (*env)->GetStringUTFLength(env, ifname);
	const char *settings_str = (*env)->GetStringUTFChars(env, settings, 0);
	size_t settings_len = (*env)->GetStringUTFLength(env, settings);
	int ret = wgTurnOn((struct go_string){
			.str = ifname_str,
			.n = ifname_len
	}, tun_fd, (struct go_string){
			.str = settings_str,
			.n = settings_len
	});
	(*env)->ReleaseStringUTFChars(env, ifname, ifname_str);
	(*env)->ReleaseStringUTFChars(env, settings, settings_str);
	return ret;
}

JNIEXPORT void JNICALL Java_com_wireguard_util_CoreLib_wgTurnOff(JNIEnv *env, jclass c, jint handle)
{
	wgTurnOff(handle);
}

JNIEXPORT jint JNICALL Java_com_wireguard_util_CoreLib_wgGetSocketV4(JNIEnv *env, jclass c, jint handle)
{
	return wgGetSocketV4(handle);
}

JNIEXPORT jint JNICALL Java_com_wireguard_util_CoreLib_wgGetSocketV6(JNIEnv *env, jclass c, jint handle)
{
	return wgGetSocketV6(handle);
}

JNIEXPORT jstring JNICALL Java_com_wireguard_util_CoreLib_wgVersion(JNIEnv *env, jclass c)
{
	jstring ret;
	char *version = wgVersion();
	if (!version)
		return NULL;
	ret = (*env)->NewStringUTF(env, version);
	free(version);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_wireguard_util_CoreLib_init(JNIEnv *env, jclass c)
{
	int rst = wginit();
	LOGE("CoreLib_init fin");
	return rst;
}

JNIEXPORT jbyteArray JNICALL Java_com_wireguard_util_CoreLib_getPublicKeyByID(JNIEnv *env, jclass c, jint priKey)
{
	int length = 32;
	//memset(&gs_raw_data,1,32);
    LOGE("CoreLib_getPublicKeyByID PRIKEY:%d", priKey);
	jbyte *rst = (jbyte *)getPublicKeyByID(priKey);
    LOGE("CoreLib_getPublicKeyByID fin:%d", priKey);
    for(int i = 0; i < 32; i++) {
        LOGE("CoreLib_generateKeyPair pubkey:%d,%d", i, rst[i]);
    }
	jbyteArray pubkey =(*env)->NewByteArray(env, length);
	(*env)->ReleaseByteArrayElements(env, pubkey, rst, 0);
    LOGE("CoreLib_getPublicKeyByID ReleaseByteArrayElements finished");
	return pubkey;
}

JNIEXPORT jint JNICALL Java_com_wireguard_util_CoreLib_generateKeyPair(JNIEnv *env, jclass c, jbyteArray pubkey)
{
	int priKey = 0;
	LOGE("CoreLib_generateKeyPair PRIKEY:%d", priKey);
	jbyte *rst = (jbyte *)generateKeyPair(&priKey);
	LOGE("CoreLib_generateKeyPair PRIKEY:%d", priKey);
	for(int i = 0; i < 32; i++) {
		LOGE("CoreLib_generateKeyPair pubkey:%d,%d", i, rst[i]);
	}
	(*env)->ReleaseByteArrayElements(env, pubkey, rst, 0);
	LOGE("CoreLib_generateKeyPair ReleaseByteArrayElements finished");
	return priKey;
}