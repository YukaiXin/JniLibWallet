package com.iyich.wallet.lib.jni;


import Format.FastModeCallback;

public class FastModeJNI {

    static {
        System.loadLibrary("fastmode");
    }

    FastModeCallback fastModeCallback;

    public FastModeJNI(FastModeCallback fastModeCallback) {
        this.fastModeCallback = fastModeCallback;
//        JniInit();
    }

    public void SendBuff(byte[] data){
        fastModeCallback.sendData(data);
    }

    public void RecieveBuff(byte[] data, int len, String mac){
        fastModeCallback.revice(data, len, mac);
    }


    public native boolean sendData(byte[] data, int length);
    public native boolean onReceive(byte[] data, int length, String bleMac);
//    public native void JniInit();
}
