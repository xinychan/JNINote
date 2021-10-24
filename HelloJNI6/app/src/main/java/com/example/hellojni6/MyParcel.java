package com.example.hellojni6;

/**
 * 自定义 Parcel 功能
 */
public class MyParcel {

    static {
        System.loadLibrary("native-lib");
    }

    // 传给 C++ 层的指针；对应 C++ 层的 Parcel 对象的地址
    private long mNativePtr = 0;

    private MyParcel() {
        this.mNativePtr = nativeCreate();
    }

    private static class MyObject {
        private static MyParcel myParcel = new MyParcel();
    }

    public static MyParcel obtain() {
        return MyObject.myParcel;
    }

    public final void writeInt(int val) {
        nativeWriteInt(mNativePtr, val);
    }

    public final void setDataPosition(int pos) {
        nativeSetDataPosition(mNativePtr, pos);
    }

    public final int readInt() {
        return nativeReadInt(mNativePtr);
    }

    // 在native层构建 Parcel.cpp 对象指针地址
    private native long nativeCreate();

    // 写入数据
    private native void nativeWriteInt(long nativePtr, int val);

    // 指针回到起始位置
    private native void nativeSetDataPosition(long nativePtr, int pos);

    // 读取数据
    private native int nativeReadInt(long nativePtr);
}
