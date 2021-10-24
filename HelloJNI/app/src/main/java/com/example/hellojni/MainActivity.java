package com.example.hellojni;

import android.os.Bundle;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

// 使用 javah 命令，创建头文件
// javah com.example.hellojni.MainActivity
public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    public static final int NUM = 100;
    public String name = "Android";
    public static int age = 10;

    // Native 函数
    public native String getStringPwd();

    // Native 静态函数
    public static native String getStringPwd2();

    // Native 修改Java对象属性
    public native void changeName();

    // Native 修改Java对象静态属性
    public static native void changeAge();

    // Native 调用Java对象方法
    public native void callAddMethod();

    // 提供函数，给 Native 层调用
    public int add(int x, int y) {
        return x + y;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView tv = findViewById(R.id.sample_text);
        changeName();
        tv.setText(name);
        changeAge();
        tv.setText(name + ":" + age);
        callAddMethod();
    }

}