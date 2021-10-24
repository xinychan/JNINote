package com.example.hellojni6;

import android.os.Bundle;
import android.os.Parcel;
import android.util.Log;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "XYCJNI";

    static {
        System.loadLibrary("native-lib");
    }

    public native String stringFromJNI();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
    }

    @Override
    protected void onResume() {
        super.onResume();
        testParcel();
        testMyParcel();
    }

    private void testParcel() {
        // 使用 Android 内置的 Parcel
        // 写入数据
        Parcel parcel = Parcel.obtain();
        parcel.writeInt(100);
        parcel.writeInt(200);
        // 指针回到起始位置
        parcel.setDataPosition(0);
        // 指针回到起始位置后，读取数据
        int result = parcel.readInt();
        Log.i(TAG, "testParcel:" + result);
        result = parcel.readInt();
        Log.i(TAG, "testParcel:" + result);
    }

    private void testMyParcel() {
        // 使用自定义的 Parcel
        MyParcel myParcel = MyParcel.obtain();
        myParcel.writeInt(10);
        myParcel.writeInt(20);
        myParcel.setDataPosition(0);
        int result = myParcel.readInt();
        Log.i(TAG, "testMyParcel:" + result);
        result = myParcel.readInt();
        Log.i(TAG, "testMyParcel:" + result);
    }

}
