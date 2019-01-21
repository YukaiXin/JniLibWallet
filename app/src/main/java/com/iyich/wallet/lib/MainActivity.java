package com.iyich.wallet.lib;

import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.iyich.wallet.lib.jni.FastModeJNI;
import com.iyich.wallet.lib.jni.SecLinkJni;

import java.security.SecureRandom;
import java.util.Random;

public class MainActivity extends AppCompatActivity implements FastModeCallback{
    private SecLinkJni secLinkJni;
    private  TextView tvSec;
    private  TextView tvBle;

    private String aesKey = "ab46a794d099ed090321d8878dc873228539796d6b84b777ba193fb3dceb561d";
    private String pubKeyString = "04f0a5e28e9d01dd893f3c824b637363db06a2745855ace9b879b512f661f0e593974c8e783c8b4a5e04b5d74dd8e58994528bc072c19b94ded6ad5ded097e46c5";
    private  String prikeyStr    = "60db6d68e204948a27d685b41e376faf16f36698f80cb4abc0f0b736ba264454";
    //Session key
    private  String ebcrypteData = "87bd102fb497e1f2f2642dd141b8034c6ac6e5c6fd532cc8bb8fde721333e08cb890f3ee25a1f4e7a523a2f5cc40c153";
    private  String R1 = "041415248fbf0084b0eb505ea8f6b66751758236ccbfe534dbe4df9cad99db424262e8fd67fd0df74ce3d6d37cebc8d2172088060c7c5d32232b45afe74ea03d79";
    private  String sessionHash = "fe8ad6107527b276ee97cf378d287e9ce06e2b4c3e705c101d6575186ca85721";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tvSec =findViewById(R.id.tv_sec);
        tvBle = findViewById(R.id.tv_ble);
        secLinkJni = new SecLinkJni(this);

        tvBle.setOnClickListener(onClickListener);
        tvSec.setOnClickListener(onClickListener);

    }


    View.OnClickListener onClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            switch (v.getId()){
                case R.id.tv_sec:
                    byte[] iv = new byte[16];
                    Random random = new Random(System.currentTimeMillis());
                    random.nextBytes(iv);
                    byte[] aesKey = secLinkJni.veritySession(Convert.hexStringToByte(prikeyStr), Convert.hexStringToByte(R1) ,Convert.hexStringToByte(ebcrypteData), Convert.hexStringToByte(sessionHash));
                    Log.i("kxyu_sha",Convert.bytesToHexString(aesKey));
                    break;
                case R.id.tv_ble:
                    Log.i("kxyu_ble","  测试蓝牙协议  ");
                    secLinkJni.SendBuff(Convert.hexStringToByte(R1));
                    break;
            }
        }
    };


    @Override
    public void sendData(byte[] data) {
        Log.i("kxyu_ble","   callback  sendData  :  "+Convert.bytesToHexString(data));
    }

    @Override
    public void revice(byte[] data, int len) {
        Log.i("kxyu_ble","   callback  revice  :  "+Convert.bytesToHexString(data));
    }
}
