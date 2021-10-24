#include "com_example_hellojni_MainActivity.h"

// NDK工具链中的 log 库
#include <android/log.h>
// log 打印的 TAG
#define TAG "XYCJNI"
// log 打印宏函数
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// 函数的实现
// extern "C"==必须采用C语言编译方式，因为 JNIEnv 源码(JNINativeInterface)是使用C语言结构体
// JNIEXPORT==标记该函数可以被外部调用
// jstring==[JavaString<==>jstring<==>NativeString转换的桥梁]
// JNICALL==表示是JNI的标记
// Java_com_example_hellojni_MainActivity_getStringPwd==Java_包名_类名_方法名；若Java包名包含_，则在Native中_转换为1
// JNIEnv *env==JNI的调用环境；所有JNI操作，必须依赖JNIEnv
// jobject==Java类的实例；一般用于对象传递；比如：MainActivity.this
// jclass==Java类的Class；一般用于静态函数；比如：MainActivity.class
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_hellojni_MainActivity_getStringPwd(JNIEnv *env, jobject jobj) {
    // TODO: implement getStringPwd()
}

// 静态函数；使用 jclass
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_hellojni_MainActivity_getStringPwd2(JNIEnv *env, jclass clazz) {
    // TODO: implement getStringPwd2()
}

// 获取Java对象的属性，并修改
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni_MainActivity_changeName(JNIEnv *env, jobject thiz) {
    // 获取 Java 对象的 Class
    jclass mainActivityCls = env->GetObjectClass(thiz);
    // 获取Class的属性
    // jclass==Java对象的Class，const char* name==属性的名称，const char* sig==属性的签名
    // GetFieldID(jclass clazz, const char* name, const char* sig)
    jfieldID nameField = env->GetFieldID(mainActivityCls, "name", "Ljava/lang/String;");
    // 把name属性转换成 jstring
    jstring nameJstr = static_cast<jstring>(env->GetObjectField(thiz, nameField));
    // 把 jstring 转为 char*
    char *nameCstr = const_cast<char *>(env->GetStringUTFChars(nameJstr, NULL));
    // native 中可以打印 char*，不能打印 jstring
    LOGI("native:%s\n", nameCstr);
    // 把 char* 转为 jstring
    jstring newNameJstr = env->NewStringUTF("CLion");
    // 给Java对象的属性赋值
    env->SetObjectField(thiz, nameField, newNameJstr);
}

// 获取Java对象的静态属性，并修改
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni_MainActivity_changeAge(JNIEnv *env, jclass clazz) {
    // 获取Class的静态属性
    // jclass==Java对象的Class，const char* name==属性的名称，const char* sig==属性的签名
    // GetStaticFieldID(jclass clazz, const char* name, const char* sig)
    jfieldID ageField = env->GetStaticFieldID(clazz, "age", "I");
    // 把age属性转换成 jint
    jint ageJint = env->GetStaticIntField(clazz, ageField);
    // 修改 jint 的值；jint 本质上是 int
    ageJint += 100;
    // 给Java对象的静态int属性赋值
    env->SetStaticIntField(clazz, ageField, ageJint);
}

// 调用Java对象方法
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni_MainActivity_callAddMethod(JNIEnv *env, jobject thiz) {
    // 获取 Java 对象的 Class
    jclass mainActivityCls = env->GetObjectClass(thiz);
    // 获取Class的函数
    // jclass==Java对象的Class，const char* name==函数的名称，const char* sig==函数的签名
    // GetMethodID(jclass clazz, const char* name, const char* sig)
    jmethodID addJmethod = env->GetMethodID(mainActivityCls, "add", "(II)I");
    // 调用 Java 函数；传入参数，并获取返回值
    jint result = env->CallIntMethod(thiz, addJmethod, 100, 100);
    LOGI("add result:%d\n", result);
}

/**
 * Java类型对应在JNI中的签名
 * Java的boolean === Z
 * Java的byte === B
 * Java的char === C
 * Java的short === S
 * Java的int === I
 * Java的long === J
 * Java的float === F
 * Java的double === D
 * Java的void === V
 * Java的object/引用类型 === Lxxx/xxx/xx/类名;
 * Java的String === Ljava/lang/String;
 * Java的int[] === [I
 * Java的double[][][][] === [[[D
 * 函数int add(char c1, char c2) === (CC)I
 * 函数void add() === ()V
 *
 * 可以通过 javap 命令查看 class 中所有属性和方法的签名
 * javap -s -p Test
 * Test 必须是 Test.class
 */