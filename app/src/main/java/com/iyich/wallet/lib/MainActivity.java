package com.iyich.wallet.lib;

import android.net.wifi.WifiManager;
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
import java.util.HashMap;
import java.util.Random;

import Format.FastModeCallback;

public class MainActivity extends AppCompatActivity implements FastModeCallback {
    private SecLinkJni secLinkJni;
    private  TextView tvSec, tvBle, tvServerKey, tvDeviceKey, tvGetModelKey;

    private String aesKey = "ab46a794d099ed090321d8878dc873228539796d6b84b777ba193fb3dceb561d";
    private String pubKeyString = "04f0a5e28e9d01dd893f3c824b637363db06a2745855ace9b879b512f661f0e593974c8e783c8b4a5e04b5d74dd8e58994528bc072c19b94ded6ad5ded097e46c5";
    private  String prikeyStr    = "60db6d68e204948a27d685b41e376faf16f36698f80cb4abc0f0b736ba264454";
    //Session key
    private  String ebcrypteData = "87bd102fb497e1f2f2642dd141b8034c6ac6e5c6fd532cc8bb8fde721333e08cb890f3ee25a1f4e7a523a2f5cc40c153";
    private  String R1 = "041415248fbf0084b0eb505ea8f6b66751758236ccbfe534dbe4df9cad99db424262e8fd67fd0df74ce3d6d37cebc8d2172088060c7c5d32232b45afe74ea03d79";
    private  String sessionHash = "fe8ad6107527b276ee97cf378d287e9ce06e2b4c3e705c101d6575186ca85721";
    public static String PRIVATE_KEY = "MIICdQIBADANBgkqhkiG9w0BAQEFAASCAl8wggJbAgEAAoGBAJDfexhe6i0o3q7/jMRkJ3w5UWgQ\n" +
            "dqgXFUlz3cVBmFWPIWLSvil9TuhbS/TmWTlUpdizkq8nuF2hU+BsLJUkKpOSI5cWiWgf79TCrw3g\n" +
            "O0VTLa3rxCuXFT3xaPIEnJSLK7ZKK4xm+mWWkgQgZS8spm6GWw29PsJMb5lQEE2APcnJAgMBAAEC\n" +
            "gYBUY0lRzd/8xETKjSNTNr/lVYH86zIjAactrSdugqIYnJIm/Xk1s79mC+A33IH2qj8c9qUoECwN\n" +
            "pi60mG5zxahif1Vt1WQaZSEz/NyfHhtZo6phEIkG7oxOki3gpVy8FCG92lEOmkz9/F5aHMi6nWvA\n" +
            "Vmq4MLz5Hc5FqE/1yfK67QJBAOe3lGd3eJbvb34fYpA/44PK8mXGFt0c11eHnwIoF3SgMGRzAjy5\n" +
            "7xEHp3dgn2lgjd2nBFRVnBFBYlGkD04fHTcCQQCgDhQ4nc2l/MYIzOGefgCgkj1FWLBSKsuW7ull\n" +
            "VPRJkmWRSWcCj9wPlJQf0lg8hxTL9MPrahDQVqzrwYkOFdD/AkAKM02oJF09n7qzvaP0H6vfUF7N\n" +
            "ViQJhcyBAOpiCOvHaYnX0ixGChVZFZIssicyV1rHaCPLpp0hMVSnrWYzFTGPAkAbtPQ5LNAZyN07\n" +
            "TsRL3IIdSdODeqvjPXdK5IbQvNkhF0+vy+YiImc050KRE6ekqrml6PQ1f4Hznt2GXlVsc8hfAkAR\n" +
            "DO06vvqdIt1osz7PFFX9Ms7J5Q2U8Bt2v9i0xPmahi25MKb+Nyv/Fdd2syt+b2YP+eAkO6OyUXqe\n" +
            "dGz65rwC";

    public static HashMap<Integer, String> pubkMapDebug = new HashMap<>();

    public static HashMap<Integer, String> pubkMapModel = new HashMap<>();

    static {
        pubkMapDebug.put(1, "02b0f062e9344770b0acdfeabafb3d870ed202fe2e8416fd9d840a6440964446e4");
        pubkMapDebug.put(2, "023f110f9d41878dd11eb59c06305f351c7d6ce416c6f98c484bfee1a5f32b3e69");
        pubkMapDebug.put(3, "026f84b7f28b2e642175f8f27178f89e36c419d9180d1c7c87dd7a39cd51c1c074");
        pubkMapDebug.put(4, "0278ae6205afdcd635d2cce74ebd66c02d901d29163fa162ac2b54e389481761f2");
        pubkMapDebug.put(5, "0288e098ba998835bcd39a48fe33f1ba6c568811cf0629e93ecc252a150e395225");
        pubkMapDebug.put(6, "023f0c51ea52c7fc344a34767186b7cccf0eec99fae7de579f65b397c7f1daa0dc");
    }


    static {
        pubkMapModel.put(1, "036f3dc1195204f9ef94f39b3c8f0a9f82cde6e24324132fab9604beb78c744e43");
        pubkMapModel.put(2, "024e9c342b05f40b7fee06d21010c37ec36a599d6941394d20c63d20fb22fa282f");
        pubkMapModel.put(3, "0293466e61ecc3398de8038399157e45775b93881836fc4cc51d1cc0f34651559e");
        pubkMapModel.put(4, "035f7fc950ede95b0c365a44d7be0cffd96c1600da147dacc87320e33308c24564");
        pubkMapModel.put(5, "02600744be5ad4cc0864b4da8c1a92870ec4e09f1bbf2f45d31a056de6fdadbe1a");
        pubkMapModel.put(6, "0247f59687e6453957b0b7b2881c4dcf2999fbf69470b8e225e6ddf1bd81b84e4a");
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tvSec =findViewById(R.id.tv_sec);
        tvBle = findViewById(R.id.tv_ble);
        tvDeviceKey = findViewById(R.id.tv_device_key);
        tvServerKey = findViewById(R.id.tv_server_key);
        tvGetModelKey = findViewById(R.id.tv_model_key);

        tvGetModelKey.setOnClickListener(onClickListener);
        tvServerKey.setOnClickListener(onClickListener);
        tvDeviceKey.setOnClickListener(onClickListener);
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
//                    new Thread(){
//                        @Override
//                        public void run() {
////                            secLinkJni.sendData(Convert.hexStringToByte(R1), Convert.hexStringToByte(R1).length);
//                        }
//                    }.start();
//                    secLinkJni.SendBuff(Convert.hexStringToByte(R1));
                    break;
                case R.id.tv_device_key:
                    String deviceKey = SecLinkJni.getDevicePublicKey();
                    Toast.makeText(getBaseContext(), deviceKey, Toast.LENGTH_SHORT).show();
                    break;
                case R.id.tv_server_key:

                    String serverKey = SecLinkJni.getRsaPrivateKey();
//                    Toast.makeText(getBaseContext(), serverKey, Toast.LENGTH_SHORT).show();
                    if(serverKey.equals(PRIVATE_KEY)){
                        Log.i("kxyu_jni","  相等      ");
                    }else {
                        Log.i("kxyu_jni","  不想等    ");
                    }
                    Log.i("kxyu_jni"," jni  serverKey  "+serverKey +" 长度  ：  "+serverKey.length());

                    Log.i("kxyu_jni"," local  serverKey  "+PRIVATE_KEY +" 长度  ：  "+PRIVATE_KEY.length());

                    break;
                case R.id.tv_model_key:
                    String[] modelKeys =  SecLinkJni.getModelKey();
                    int eqCount = 0;

                    for (int i = 0; i < 6; i ++){
                        if(modelKeys[i].equals(pubkMapModel.get(i + 1))){
                            eqCount += 1;
                        }
                    }
                    if(eqCount == 6){
                        Log.i("kxyu_jni", " Model keys 相等   ！！！！！ ");
                    }else {
                        Log.i("kxyu_jni", " Model keys 不相等 ！！！！！ ");
                    }
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
