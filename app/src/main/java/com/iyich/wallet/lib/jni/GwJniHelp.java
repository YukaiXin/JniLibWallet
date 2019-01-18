package com.iyich.wallet.lib.jni;

import com.iyich.wallet.lib.FastModeCallback;

public class GwJniHelp {
    static {
        System.loadLibrary("GorillaJNI");
    }

    FastModeCallback fastModeCallback;

    public GwJniHelp(FastModeCallback fastModeCallback) {
        this.fastModeCallback = fastModeCallback;

    }

    /** 蓝牙传输协议 */

    public void SendBuff(byte[] data){
        fastModeCallback.sendData(data);
    }

    public void RecieveBuff(byte[] data, int len){
        fastModeCallback.revice(data, len);
    }

    public native boolean sendData(byte[] data, int length);
    public native boolean onReceive(byte[] data, int length);

    /** 签名验证 */
    public native byte[] veritySession(byte[] hPriKey, byte[] R, byte[] encryptedData, byte[] essionHash);
}
