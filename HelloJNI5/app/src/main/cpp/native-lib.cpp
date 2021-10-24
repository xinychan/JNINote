#include <jni.h>
#include <string>

#include <android/log.h>

#define TAG "XYCJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG,  __VA_ARGS__);


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_hellojni5_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "HelloJNI5";
    return env->NewStringUTF(hello.c_str());
}

/**
 * 对数组进行排序
 */

int compare(const jint *n1, const jint *n2) {
    return *n1 - *n2;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni5_MainActivity_sort(JNIEnv *env, jobject thiz, jintArray arr) {
    // 使用内置函数，对数组排序
    jint *intArray = env->GetIntArrayElements(arr, nullptr);
    int length = env->GetArrayLength(arr);
    // __base == 数组首地址；__nmemb == 数组的长度；__size == 元素的大小；__comparator == 函数指针
    // void qsort(void* __base, size_t __nmemb, size_t __size, int (*__comparator)(const void* __lhs, const void* __rhs));
    qsort(intArray, static_cast<size_t>(length), sizeof(int),
          reinterpret_cast<int (*)(const void *, const void *)>(compare));
    // 释放数组元素
    env->ReleaseIntArrayElements(arr, intArray, 0);
}

/**
 * 局部缓存
 */

extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni5_MainActivity_localCache(JNIEnv *env, jclass clazz, jstring name) {
    // 处理局部缓存
    jfieldID fid_name1 = nullptr;
    jfieldID fid_name2 = nullptr;
    jfieldID fid_name3 = nullptr;
    fid_name1 = env->GetStaticFieldID(clazz, "name1", "Ljava/lang/String;");
    fid_name2 = env->GetStaticFieldID(clazz, "name2", "Ljava/lang/String;");
    fid_name3 = env->GetStaticFieldID(clazz, "name3", "Ljava/lang/String;");
    env->SetStaticObjectField(clazz, fid_name1, name);
    env->SetStaticObjectField(clazz, fid_name2, name);
    env->SetStaticObjectField(clazz, fid_name3, name);
    fid_name1 = nullptr;
    fid_name2 = nullptr;
    fid_name3 = nullptr;
}

/**
 * 静态缓存
 */

static jfieldID fid_name1 = nullptr;
static jfieldID fid_name2 = nullptr;
static jfieldID fid_name3 = nullptr;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni5_MainActivity_initStaticCache(JNIEnv *env, jclass clazz) {
    // 初始化静态缓存
    fid_name1 = env->GetStaticFieldID(clazz, "name1", "Ljava/lang/String;");
    fid_name2 = env->GetStaticFieldID(clazz, "name2", "Ljava/lang/String;");
    fid_name3 = env->GetStaticFieldID(clazz, "name3", "Ljava/lang/String;");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni5_MainActivity_actionStaticCache(JNIEnv *env, jclass clazz, jstring name) {
    // 操作静态缓存
    env->SetStaticObjectField(clazz, fid_name1, name);
    env->SetStaticObjectField(clazz, fid_name2, name);
    env->SetStaticObjectField(clazz, fid_name3, name);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni5_MainActivity_clearStaticCache(JNIEnv *env, jclass clazz) {
    // 清除静态缓存
    fid_name1 = nullptr;
    fid_name2 = nullptr;
    fid_name3 = nullptr;
}

/**
 * 异常处理
 */

// C++ 层处理自己产生的异常
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni5_MainActivity_exception(JNIEnv *env, jclass clazz) {
    jfieldID fid = env->GetStaticFieldID(clazz, "name100", "Ljava/lang/String;");
    // 获取当前是否有异常
    jthrowable jthrow = env->ExceptionOccurred();
    if (jthrow) {
        // 有异常则清理异常
        LOGI("native exception has jthrowable");
        env->ExceptionClear();
        // 补救措施
        fid = env->GetStaticFieldID(clazz, "name1", "Ljava/lang/String;");
    }
}

// C++ 层产生的异常抛给Java层处理
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni5_MainActivity_exception2(JNIEnv *env, jclass clazz) {
    jfieldID fid = env->GetStaticFieldID(clazz, "name100", "Ljava/lang/String;");
    // 获取当前是否有异常
    jthrowable jthrow = env->ExceptionOccurred();
    if (jthrow) {
        // 有异常则清理异常
        LOGI("native exception2 has jthrowable");
        env->ExceptionClear();
        // 把异常抛给 Java 层处理
        jclass exceptionJcls = env->FindClass("java/lang/NoSuchFieldException");
        env->ThrowNew(exceptionJcls, "native exception2 throw NoSuchFieldException");
    }
}

// C++ 层处理Java层抛来的异常
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni5_MainActivity_exception3(JNIEnv *env, jclass clazz) {
    // 调用Java层抛异常的函数
    jmethodID showMethod = env->GetStaticMethodID(clazz, "show", "()V");
    env->CallStaticVoidMethod(clazz, showMethod);
    // 检查调用Java层函数是否有异常，有异常则输出信息，并清除异常
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}