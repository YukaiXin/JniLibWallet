package com.iyich.wallet.lib;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.iyich.wallet.lib.jni.FastModeJNI;

public class MainActivity extends AppCompatActivity implements FastModeCallback, View.OnClickListener {

    // Used to load the 'native-lib' library on application startup.

    FastModeJNI fastModeJNI;
    private byte[] testData;
    private TextView tvTXT;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tvTXT = findViewById(R.id.sample_text);
        fastModeJNI = new FastModeJNI(this);

        testData = new byte[100];

        for (int i =0; i < testData.length ; i++){
            testData[i] = (byte) (i & 0xFF);
        }
        Log.d("kxyu","   "+Convert.bytesToHexString(testData));
        tvTXT.setOnClickListener(this);
        // Example of a call to a native method
    }



    @Override
    public void sendData(byte[] data) {
        Toast.makeText(this, "  C -> java   ",Toast.LENGTH_LONG).show();
        Log.d("kxyu"," callbaok    "+Convert.bytesToHexString(data));
//        tvTXT.setText(Convert.bytesToHexString(data));
    }

    @Override
    public void revice(byte[] data, int len) {

//        tvTXT.setText(Convert.bytesToHexString(data));
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.sample_text:

                fastModeJNI.sendData(testData, 100);
                break;
        }
    }
}
