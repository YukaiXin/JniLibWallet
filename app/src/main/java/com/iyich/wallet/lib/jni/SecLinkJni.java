package com.iyich.wallet.lib.jni;

import android.content.Context;

import Format.FastModeCallback;

public class SecLinkJni {



    FastModeCallback fastModeCallback;

    public SecLinkJni(FastModeCallback fastModeCallback) {
        this.fastModeCallback = fastModeCallback;
    }

    static {
        System.loadLibrary("secLinkJni");
    }

    public static native boolean  initLib(Context context);

    public static native String getRsaPrivateKey();
    public static native String getDevicePublicKey();
    public static native byte[] veritySession(byte[] hPriKey, byte[] R, byte[] encryptedData, byte[] essionHash);

//    /*** 蓝牙链接 */
//    public void SendBuff(byte[] data){
//        fastModeCallback.sendData(data);
//    }
//    public void RecieveBuff(byte[] data, int len){
//        fastModeCallback.revice(data, len);
//    }
//    public native boolean sendData(byte[] data, int length);
//    public native boolean onReceive(byte[] data, int length);

    /** sha3  */

    /** model key */
    public static native String[] getModelKey();
}
