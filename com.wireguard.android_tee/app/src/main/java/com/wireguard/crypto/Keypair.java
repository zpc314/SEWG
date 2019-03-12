/*
 * Copyright © 2017-2018 WireGuard LLC. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

package com.wireguard.crypto;

import android.content.Context;
import android.util.Log;

import com.wireguard.util.CoreLib;

import java.security.SecureRandom;

/**
 * Represents a Curve25519 keypair as used by WireGuard.
 */
public class Keypair {
    private int privateKey;
    private byte[] publicKey;
    private CoreLib coreLib;

    public Keypair(Context context) {
        coreLib = CoreLib.getInstance(context);
        publicKey = new byte[32];
        privateKey = coreLib.generateKeyPair(publicKey);
        Log.e("KeyPair construct 1", "" + privateKey + publicKey.length);
        for (int i = 0; i < publicKey.length; i++) {
            Log.e("KeyPair construct", "" + i + " " + Integer.toHexString(publicKey[i]));
        }
    }

    public Keypair(final String priKey, Context context) {
        coreLib = CoreLib.getInstance(context);
        privateKey = Integer.parseInt(priKey);
        Log.e("KeyPair construct 2", "" + privateKey);
        publicKey = coreLib.getPublicKeyByID(privateKey);
        for (int i = 0; i < publicKey.length; i++) {
            Log.e("KeyPair construct 2", "" + i + " " + Integer.toHexString(publicKey[i]));
        }
    }

    public String getPrivateKey() {
        return "" + privateKey;
    }

    public String getPublicKey() {
        return KeyEncoding.keyToBase64(publicKey);
    }
}

