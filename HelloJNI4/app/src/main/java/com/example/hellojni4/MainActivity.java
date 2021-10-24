package com.example.hellojni4;

import android.os.Bundle;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

/**
 * JNI的静态注册和动态注册
 * JNI的多线程
 */
public class MainActivity extends AppCompatActivity {

    private static final String TAG = "XYCJNI";

    static {
        // System.loadLibrary 会执行 jni.h 中的 JNI_OnLoad 函数
        Log.i(TAG, "MainActivity System.loadLibrary");
        System.loadLibrary("native-lib");
    }

    /**
     * JNI 静态注册 native 函数
     */
    public native String stringFromJNI();

    public native void staticRegister();

    /**
     * JNI 动态注册 native 函数
     */
    public native void dynamicJavaMethod01();

    public native int dynamicJavaMethod02(String value);

    /**
     * JNI 线程
     */
    public native void nativeThread();

    /**
     * JNI 中 JavaVM/JNIEnv/jobject 的区别
     */
    public native void nativeFun1();

    public native void nativeFun2();

    public static native void staticNativeFun3();

    public static native void staticNativeFun4();

    public static native void staticNativeFun5();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        staticRegister();
    }

    // 点击事件
    public void test01(View view) {
        dynamicJavaMethod01();
    }

    public void test02(View view) {
        int result = dynamicJavaMethod02("MainActivity");
        Log.i(TAG, "MainActivity test02 result:" + result);
    }

    public void test03(View view) {
        nativeThread();
    }

    public void test04(View view) {
        // 主线程调用
        nativeFun1();
        nativeFun2();
        staticNativeFun3();
        staticNativeFun5();

        // 工作线程调用
        new Thread() {
            @Override
            public void run() {
                super.run();
                staticNativeFun4();
            }
        }.start();
    }

    public void test05(View view) {
    }

    public void test06(View view) {
    }

    // 给 native 代码调用的 Java 函数
    public void updateActivityUI() {
        final AlertDialog.Builder dialog = new AlertDialog.Builder(this);
        if (Looper.myLooper() == Looper.getMainLooper()) {
            // native 主线程调用此函数
            dialog.setTitle("UI-Thread");
            dialog.setMessage("nativeThread is UI-Thread");
            dialog.setPositiveButton("OK", null);
            dialog.show();
        } else {
            // native 工作线程调用此函数
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    dialog.setTitle("Work-Thread");
                    dialog.setMessage("nativeThread is Work-Thread");
                    dialog.setPositiveButton("OK", null);
                    dialog.show();
                }
            });
        }
    }
}
