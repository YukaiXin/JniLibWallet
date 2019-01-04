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

public class MainActivity extends AppCompatActivity {
    private SecLinkJni secLinkJni;
    private  TextView tvBtn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tv =findViewById(R.id.sample_text);

        String aesKey = "39be522ee19d5d441d92e03639621a695ae1f56377397781dcf4243a36db9712";
        String pubKeyString = "0432973bcbc9bad169fe61497a69d2f4da62a2d2fef35b728a45fce76d1a7739831967a93d8bf614d33bf396f8f0d26e89f24571f93a3625fe72dd222044a8e2db";
        final String prikeyStr    = "15de9ffbdd26754a92b51c3cb771ecd82f62d674c0dada4dde2d45db04c806ef";
        //Session key
        final String ebcrypteData = "1095f46cb6a4b619e5f43b976928ff9369054e2ae31449922d50b686affa57cce0dcd86f5b3ac5ef1dba4dec449d8026";
        final String R = "0409871226f55c99d98bc3adf586e7d55a94b8d216a0df1340c6333267f577a15fc896d9955ca2d19a5d4ac690d0b917dc0db7f25b05b4fe0a266ea2258a0cefe6";
        final String sessionHash = "5d034e2c752f1b11ea012665e71cdc6800c27aac1afea1766399f78432651137";


        Log.i("kxyu","  session hash size  ：  "+Convert.hexStringToByte(sessionHash).length);
        Log.i("kxyu","  ebcrypteData size  ：  "+Convert.hexStringToByte(ebcrypteData).length);

        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                secLinkJni = new SecLinkJni();
                byte[] iv = new byte[16];
                Random random = new Random(System.currentTimeMillis());
                random.nextBytes(iv);
                byte[] obuf = secLinkJni.veritySession(Convert.hexStringToByte(prikeyStr), Convert.hexStringToByte(R) ,Convert.hexStringToByte(ebcrypteData), Convert.hexStringToByte(sessionHash));

//                Log.i("kxyu","   obuf  size  : "+obuf.length+"  ================    "+Convert.bytesToHexString(obuf));
            }
        });



    }
}
