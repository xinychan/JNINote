package com.example.hellojni2;

import android.util.Log;

public class Student {

    private static final String TAG = "Student";

    public String name;
    public int age;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        Log.i(TAG, "setName:" + name);
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        Log.i(TAG, "setAge:" + age);
        this.age = age;
    }

    public static void showInfo(String info) {
        Log.i(TAG, "showInfo:" + info);
    }

    @Override
    public String toString() {
        return "Student{" +
                "name='" + name + '\'' +
                ", age=" + age +
                '}';
    }
}
