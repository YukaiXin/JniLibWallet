package com.iyich.wallet.lib;

public interface FastModeCallback {

    void sendData(byte[] data);
    void revice(byte[] data, int len);

}