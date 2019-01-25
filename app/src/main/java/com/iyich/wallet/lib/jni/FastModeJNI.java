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

    public void RecieveBuff(byte[] data, int len){
        fastModeCallback.revice(data, len);
    }


    public native boolean sendData(byte[] data, int length);
    public native boolean onReceive(byte[] data, int length);
//    public native void JniInit();
}
