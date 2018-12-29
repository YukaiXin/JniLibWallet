package com.iyich.wallet.lib.jni;

import android.util.Log;

import com.iyich.wallet.lib.Convert;

public class SecLinkJni {

    public byte[] S;



    static {
        System.loadLibrary("secLinkJni");
    }

    public SecLinkJni() {

    }


    public void setAesKey(byte[] aesKey, byte[] data){
        Log.i("kxyu"," data  size  : "+data.length+" hex str : "+Convert.bytesToHexString(data));
        Log.i("kxyu"," aeskey  size  : "+aesKey.length+" hex str : "+Convert.bytesToHexString(aesKey));
    }

    public native byte[] veritySession(byte[] hPriKey, byte[] R, byte[] encryptedData, byte[] essionHash);

}
