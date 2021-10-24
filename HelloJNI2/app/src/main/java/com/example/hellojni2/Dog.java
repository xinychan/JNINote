package com.example.hellojni2;

import android.util.Log;

public class Dog {
    private static final String TAG = "Dog";

    public Dog() {
        Log.i(TAG, "Dog init...");
    }

    public Dog(int n1) {
        Log.i(TAG, "Dog init... n1:" + n1);
    }

    public Dog(int n1, int n2) {
        Log.i(TAG, "Dog init... n1:" + n1 + " n2:" + n2);
    }

    public Dog(int n1, int n2, int n3) {
        Log.i(TAG, "Dog init... n1:" + n1 + " n2:" + n2 + " n3:" + n3);
    }
}
