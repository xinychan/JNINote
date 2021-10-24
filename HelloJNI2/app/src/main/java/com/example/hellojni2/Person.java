package com.example.hellojni2;

import android.util.Log;

public class Person {

    private static final String TAG = "Person";

    public Student student;

    public void setStudent(Student student) {
        Log.i(TAG, "setStudent:" + student.toString());
        this.student = student;
    }

    public static void putStudent(Student student) {
        Log.i(TAG, "putStudent:" + student.toString());
    }

}
