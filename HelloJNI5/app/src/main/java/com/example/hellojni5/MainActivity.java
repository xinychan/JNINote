package com.example.hellojni5;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "XYCJNI";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /**
     * 对数组进行排序
     */
    public native void sort(int[] arr);

    /**
     * 普通局部缓存
     */
    public static native void localCache(String name);

    /**
     * 静态缓存的使用
     */
    static String name1 = "CLion1";
    static String name2 = "CLion2";
    static String name3 = "CLion3";

    // 初始化静态缓存
    public static native void initStaticCache();

    // 操作静态缓存
    public static native void actionStaticCache(String name);

    // 清除静态缓存
    public static native void clearStaticCache();

    /**
     * 异常处理
     */

    // C++层处理异常
    public static native void exception();

    // 处理C++层抛出来的异常
    public static native void exception2() throws NoSuchFieldException;

    // C++层处理Java层抛出的异常
    public static native void exception3();

    // 供C++层调用的函数，并抛出异常给C++层
    public static void show() throws Exception {
        Log.i(TAG, "show()");
        throw new NullPointerException();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // 清除静态缓存
        clearStaticCache();
    }

    public void test01(View view) {
        int[] arr = {10, 50, 30, 60, 20, 40};
        sort(arr);
        for (int value : arr) {
            Log.i(TAG, "value:" + value);
        }
    }

    public void test02(View view) {
        // 操作局部缓存
        localCache("localCache");
        Log.i(TAG, "name1:" + name1);
        Log.i(TAG, "name2:" + name2);
        Log.i(TAG, "name3:" + name3);
    }

    public void test03(View view) {
        // 初始化静态缓存
        initStaticCache();
        // 操作静态缓存
        actionStaticCache("staticCache");
        Log.i(TAG, "name1:" + name1);
        Log.i(TAG, "name2:" + name2);
        Log.i(TAG, "name3:" + name3);
    }

    public void test04(View view) {
        exception();
    }

    public void test05(View view) {
        try {
            exception2();
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
            Log.i(TAG, "exception2() try catch");
        }
    }

    public void test06(View view) {
        exception3();
    }


}
