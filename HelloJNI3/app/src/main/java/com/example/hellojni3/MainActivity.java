package com.example.hellojni3;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import org.fmod.FMOD;

/**
 * Android NDK 导入 C 库开发流程；以 FMOD 库为例
 */
public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";

    // 声音类型
    private static final int MODE_NORMAL = 0;
    private static final int MODE_LUOLI = 1;
    private static final int MODE_DASHU = 2;
    private static final int MODE_JINGSONG = 3;
    private static final int MODE_GAOGUAI = 4;
    private static final int MODE_KONGLING = 5;

    static {
        System.loadLibrary("native-lib");
    }

    // 播放声源路径
    private String path;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        path = "file:///android_asset/derry.mp3";
        // 初始化 FMOD 模块
        FMOD.init(this);
    }

    @Override
    public void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        // 关闭 FMOD 模块
        FMOD.close();
    }

    // native 函数
    public native String stringFromJNI();

    // native 变声函数
    public native void voiceChangeNative(int mode, String path);

    // 真实开发中，要用子线程；当前Demo使用的是主线程播放，播放时会卡顿，应优化在JNI中使用子线程播放
    public void test01(View view) {
        voiceChangeNative(MODE_NORMAL, path);
    }

    public void test02(View view) {
        voiceChangeNative(MODE_LUOLI, path);
    }

    public void test03(View view) {
        voiceChangeNative(MODE_DASHU, path);
    }

    public void test04(View view) {
        voiceChangeNative(MODE_JINGSONG, path);
    }

    public void test05(View view) {
        voiceChangeNative(MODE_GAOGUAI, path);
    }

    public void test06(View view) {
        voiceChangeNative(MODE_KONGLING, path);
    }

    // 给C++调用的函数
    // JNI 调用 Java函数的时候，会忽略 private/public 属性
    private void playerEnd(String msg) {
        Log.i(TAG, "MainActivity playerEnd");
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }
}
