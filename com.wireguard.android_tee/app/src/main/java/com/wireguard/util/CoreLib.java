package com.wireguard.util;

import android.content.Context;

import com.wireguard.android.util.SharedLibraryLoader;

public class CoreLib {

    private CoreLib(Context context) {
        SharedLibraryLoader.loadSharedLibrary(context, "wg-go");
        init();
    }

    private static CoreLib mInstance = null;

    public static synchronized CoreLib getInstance(Context context) {
        if (mInstance == null) {
            mInstance = new CoreLib(context);
        }
        return mInstance;
    }

    public native int wgGetSocketV4(int handle);

    public native int wgGetSocketV6(int handle);

    public native void wgTurnOff(int handle);

    public native int wgTurnOn(String ifName, int tunFd, String settings);

    public native String wgVersion();

    public native int init();

    public native byte[] getPublicKeyByID(int priKey);

    public native int generateKeyPair(byte[] pubkey);
}
