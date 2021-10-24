#include <jni.h>
#include <string>
#include "com_example_hellojni6_MyParcel.h"
#include "MyParcel.h"

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_hellojni6_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "HelloJNI6";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_hellojni6_MyParcel_nativeCreate
        (JNIEnv *jniEnv, jobject jobj) {
    MyParcel *myParcel = new MyParcel();
    return reinterpret_cast<jlong>(myParcel);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni6_MyParcel_nativeWriteInt
        (JNIEnv *jniEnv, jobject jobj, jlong nativePtr, jint val) {
    MyParcel *myParcel = reinterpret_cast<MyParcel *>(nativePtr);
    myParcel->writeInt(val);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni6_MyParcel_nativeSetDataPosition
        (JNIEnv *jniEnv, jobject jobj, jlong nativePtr, jint pos) {
    MyParcel *myParcel = reinterpret_cast<MyParcel *>(nativePtr);
    myParcel->setDataPosition(pos);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_example_hellojni6_MyParcel_nativeReadInt
        (JNIEnv *jniEnv, jobject jobj, jlong nativePtr) {
    MyParcel *myParcel = reinterpret_cast<MyParcel *>(nativePtr);
    return myParcel->readInt();
}
