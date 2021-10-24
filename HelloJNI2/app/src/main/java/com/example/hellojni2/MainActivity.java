package com.example.hellojni2;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity_XYCJNI";

    static {
        // System.load("xxx/xxx/xxx/native-lib"); // 绝对路径加载动态链接库文件
        System.loadLibrary("native-lib"); // 库目录按层级去找，在编译后apk的 lib/libnative-lib.so
    }

    // native 函数
    public native String stringFromJNI();

    // 基本数据类型，引用类型，基本类型数组，引用类型数组；在JNI中的转换
    public native void testArrayAction(int count, String info, int[] counts, String[] manyInfo);

    // 引用类型/对象；在JNI中的转换
    public native void putObject(Student student, String info);

    // 在JNI中直接创建Java对象
    public native void insertObject();

    // 在JNI中引用类型的释放处理-局部引用
    public native void testQuote();

    // 在JNI中引用类型的释放处理-全局引用
    public native void testQuote2();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
    }

    public void test01(View view) {
        int count = 100;
        String info = "CLion";
        int[] counts = new int[]{10, 20, 30, 40};
        String[] manyInfo = new String[]{"AAA", "BBB", "CCC"};
        testArrayAction(count, info, counts, manyInfo);
        for (int num : counts) {
            Log.i(TAG, "test01 counts:" + num);
        }
    }

    public void test02(View view) {
        Student student = new Student();
        student.setName("StudentName");
        student.setAge(10);
        putObject(student, "StudentInfo");
    }

    public void test03(View view) {
        insertObject();
    }

    public void test04(View view) {
        testQuote();
    }

    public void test05(View view) {
        testQuote2();
    }
}
