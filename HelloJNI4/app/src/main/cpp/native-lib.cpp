#include <jni.h>
#include <string>
#include <pthread.h>

#include <android/log.h>

#define TAG "XYCJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG,  __VA_ARGS__);

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_hellojni4_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "HelloJNI4";
    return env->NewStringUTF(hello.c_str());
}

// 默认情况下，就是静态注册，静态注册是最简单的方式，NDK开发过程中，基本上使用静态注册
// Android 系统的C++源码：基本上都是动态注册

/**
 * 静态注册
 */

// 静态注册
// 优点:开发简单
// 缺点:
// 1.JNI函数名非常长
// 2.JNI函数捆绑Java层，包名+类名+函数名=JNI函数名
// 3.函数在调用时（运行期），才会去匹配JNI函数，性能上低于动态注册；
// （动态注册在System.loadLibrary时就匹配）
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni4_MainActivity_staticRegister(JNIEnv *env, jobject thiz) {
    LOGI("staticRegister");
}

/**
 * 动态注册
 */

// 动态注册步骤
// 1-重写jni.h中的JNI_OnLoad函数，JNI_OnLoad函数在System.loadLibrary调用时执行
// 2-创建 JNINativeMethod 数组，标记要动态注册的函数
// 3-在JNI_OnLoad函数中，对 JNINativeMethod 数组中的函数进行动态注册

// 全局可调用的 JavaVM
JavaVM *jvm = nullptr;

// 需要动态注册 native 函数的Java类
const char *mainActivityClassName = "com/example/hellojni4/MainActivity";

//Java 层函数：public native void dynamicJavaMethod01();
void dynamicJavaMethod01(JNIEnv *env, jobject thiz) {
    LOGI("dynamicJavaMethod01");
}

//Java 层函数：public native int dynamicJavaMethod02(String value);
int dynamicJavaMethod02(JNIEnv *env, jobject thiz, jstring value) {
    const char *valueChar = env->GetStringUTFChars(value, nullptr);
    LOGI("dynamicJavaMethod02 valueChar:%s", valueChar);
    env->ReleaseStringUTFChars(value, valueChar);
    return 100;
}

// 创建 JNINativeMethod 数组，标记要动态注册的函数
//typedef struct {
//    const char* name; // 函数名
//    const char* signature; // 函数签名
//    void*       fnPtr; // 函数指针
//} JNINativeMethod;
static const JNINativeMethod jniNativeMethod[] = {
        {"dynamicJavaMethod01", "()V",                   (void *) (dynamicJavaMethod01)},
        {"dynamicJavaMethod02", "(Ljava/lang/String;)I", (int *) (dynamicJavaMethod02)},
};

// 动态注册：在 Java 层的 System.loadLibrary 执行时，就开始注册 native 函数
// Java：类似Java的构造函数，如果你不写构造函数，默认就有构造函数，如果你写构造函数，覆盖默认的构造函数
// JNI：JNI_OnLoad函数，如果你不写JNI_OnLoad，默认就有JNI_OnLoad，如果你写JNI_OnLoad，覆盖默认的JNI_OnLoad
JNIEXPORT jint JNI_OnLoad(JavaVM *javaVm, void *reserved) {
    LOGI("System.loadLibrary ==> JNI_OnLoad");
    // 把 javaVm 赋值给全局可调用的 jvm；相当于 Java 中 this.jvm = javaVm;
    ::jvm = javaVm;

    // 从 JavaVM 中获取 JNIEnv
    JNIEnv *jniEnv = nullptr;
    jint envResult = javaVm->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6);

    // 没有正常获取到 JNIEnv，则返回 -1 ，直接崩溃结束
    if (envResult != JNI_OK) {
        return -1;
    }
    LOGI("System.loadLibrary ==> JNI_OnLoad init");

    // 注册 MainActivity 类中的 native 函数
    jclass mainActivityJclass = jniEnv->FindClass(mainActivityClassName);
    // clazz == 注册native函数的Java类；methods == 要注册的函数数组；nMethods == 要注册的函数个数
    // jint RegisterNatives(jclass clazz, const JNINativeMethod* methods,jint nMethods)
    jniEnv->RegisterNatives(mainActivityJclass, jniNativeMethod,
                            sizeof(jniNativeMethod) / sizeof(JNINativeMethod));
    LOGI("System.loadLibrary ==> RegisterNatives");

    // AS的JDK在JNI默认最高1.6
    return JNI_VERSION_1_6;
}

/**
 * JNI 线程
 */

// 封装 JNIEnv *env 和 jobject；用于跨线程传递
class MyContext {
public:
    JNIEnv *env = nullptr;
    jobject jobj = nullptr;
};

// 工作线程执行函数
void *myThreadTaskAction(void *pVoid) {
    LOGI("myThreadTaskAction run");

    // 获取传来的 MyContext 指针
    MyContext *myContext = static_cast<MyContext *>(pVoid);

    // 通过能跨越线程，能跨越函数的全局 JavaVM，在工作线程中创建新JNIEnv
    JNIEnv *jniEnv = nullptr;
    // JavaVM 附加到当前工作线程，并创建新的 JNIEnv
    jint attachResult = ::jvm->AttachCurrentThread(&jniEnv, nullptr);
    if (attachResult != JNI_OK) {
        LOGI("AttachCurrentThread Error");
        return nullptr;
    }

    // 调用Java层的UI函数，通知Java层界面变更
    jclass mainActivityJclass = jniEnv->GetObjectClass(myContext->jobj);
    jmethodID updateActivityUI = jniEnv->GetMethodID(mainActivityJclass, "updateActivityUI", "()V");
    jniEnv->CallVoidMethod(myContext->jobj, updateActivityUI);

    // 附加到当前线程后，必须解除，否则报错
    ::jvm->DetachCurrentThread();
    LOGI("myThreadTaskAction end");
    return nullptr;
}

// JNIEnv *env:不能跨越线程，否则崩溃；可以跨越函数【解决方式：使用全局的JavaVM附加到当前工作线程，在工作线程中创建新JNIEnv再操作】
// jobject thiz:不能跨越线程，否则崩溃；不能跨越函数，否则崩溃 【解决方式：默认是局部引用，提升全局引用，可解决此问题】
// JavaVM *javaVm:能跨越线程，能跨越函数
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni4_MainActivity_nativeThread(JNIEnv *env, jobject thiz) {
    // 如下方式创建工作线程；只能执行工作线程逻辑，不能进行跨线程调用
    /*pthread_t pid;
    pthread_create(&pid, nullptr, myThreadTaskAction, nullptr);
    pthread_join(pid, nullptr);*/

    // 把当前的 JNIEnv *env, jobject thiz 赋值给 MyContext
    MyContext *myContext = new MyContext;
    myContext->env = env;
    // myContext->jobj = thiz; // 默认是局部引用，传给 myThreadTaskAction 会崩溃
    myContext->jobj = env->NewGlobalRef(thiz); // 提升为全局引用，解决 jobject 跨线程/跨函数崩溃问题

    // 把 myContext 指针传给 myThreadTaskAction 函数
    pthread_t pid;
    pthread_create(&pid, nullptr, myThreadTaskAction, myContext);
    pthread_join(pid, nullptr);
}

/**
 * JNI 中 JavaVM/JNIEnv/jobject 的区别
 */

extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni4_MainActivity_nativeFun1(JNIEnv *env, jobject thiz) {
    // Java层主线程调用
    JavaVM *javaVm = nullptr;
    env->GetJavaVM(&javaVm);
    // 打印：当前函数JNIEnv地址，当前函数JavaVM地址，当前函数jobject地址，JNI_OnLoad的JavaVM地址
    LOGI("nativeFun1 JNIEnv:%p,JavaVM:%p,jobject:%p,JNI_OnLoad_JavaVM:%p\n", env, javaVm, thiz,
         ::jvm);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni4_MainActivity_nativeFun2(JNIEnv *env, jobject thiz) {
    // Java层主线程调用
    JavaVM *javaVm = nullptr;
    env->GetJavaVM(&javaVm);
    // 打印：当前函数JNIEnv地址，当前函数JavaVM地址，当前函数jobject地址，JNI_OnLoad的JavaVM地址
    LOGI("nativeFun2 JNIEnv:%p,JavaVM:%p,jobject:%p,JNI_OnLoad_JavaVM:%p\n", env, javaVm, thiz,
         ::jvm);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni4_MainActivity_staticNativeFun3(JNIEnv *env, jclass clazz) {
    // Java层主线程调用
    JavaVM *javaVm = nullptr;
    env->GetJavaVM(&javaVm);
    // 打印：当前函数JNIEnv地址，当前函数JavaVM地址，当前函数jobject地址，JNI_OnLoad的JavaVM地址
    LOGI("staticNativeFun3 JNIEnv:%p,JavaVM:%p,jclass:%p,JNI_OnLoad_JavaVM:%p\n", env, javaVm,
         clazz, ::jvm);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni4_MainActivity_staticNativeFun5(JNIEnv *env, jclass clazz) {
    // Java层主线程调用
    JavaVM *javaVm = nullptr;
    env->GetJavaVM(&javaVm);
    // 打印：当前函数JNIEnv地址，当前函数JavaVM地址，当前函数jobject地址，JNI_OnLoad的JavaVM地址
    LOGI("staticNativeFun5 JNIEnv:%p,JavaVM:%p,jclass:%p,JNI_OnLoad_JavaVM:%p\n", env, javaVm,
         clazz, ::jvm);
}

// JNI工作中线程执行
void *jniThreadRun(void *) {
    JNIEnv *jniEnv = nullptr;
    ::jvm->AttachCurrentThread(&jniEnv, nullptr);
    // 打印：当前函数JNIEnv地址，当前函数JavaVM地址，当前函数jobject地址，JNI_OnLoad的JavaVM地址
    LOGI("jniThreadRun JNIEnv:%p,JavaVM:%p\n", jniEnv, ::jvm);
    ::jvm->DetachCurrentThread();
    return nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni4_MainActivity_staticNativeFun4(JNIEnv *env, jclass clazz) {
    // Java层工作线程调用
    JavaVM *javaVm = nullptr;
    env->GetJavaVM(&javaVm);
    // 打印：当前函数JNIEnv地址，当前函数JavaVM地址，当前函数jobject地址，JNI_OnLoad的JavaVM地址
    LOGI("staticNativeFun4 JNIEnv:%p,JavaVM:%p,jclass:%p,JNI_OnLoad_JavaVM:%p\n", env, javaVm,
         clazz, ::jvm);
    // JNI工作中线程执行
    pthread_t pid;
    pthread_create(&pid, nullptr, jniThreadRun, nullptr);
    pthread_join(pid, nullptr);
}

// LOGI 打印：
// 1992-1992/com.example.hellojni4 I/XYCJNI: nativeFun1 JNIEnv:0xc3b99a80,JavaVM:0xc3b020c0,jobject:0xcfff4bdc,JNI_OnLoad_JavaVM:0xc3b020c0
// 1992-1992/com.example.hellojni4 I/XYCJNI: nativeFun2 JNIEnv:0xc3b99a80,JavaVM:0xc3b020c0,jobject:0xcfff4bdc,JNI_OnLoad_JavaVM:0xc3b020c0
// 1992-1992/com.example.hellojni4 I/XYCJNI: staticNativeFun3 JNIEnv:0xc3b99a80,JavaVM:0xc3b020c0,jclass:0xcfff4bfc,JNI_OnLoad_JavaVM:0xc3b020c0
// 1992-1992/com.example.hellojni4 I/XYCJNI: staticNativeFun5 JNIEnv:0xc3b99a80,JavaVM:0xc3b020c0,jclass:0xcfff4bfc,JNI_OnLoad_JavaVM:0xc3b020c0
// 1992-2026/com.example.hellojni4 I/XYCJNI: staticNativeFun4 JNIEnv:0xbe25dd00,JavaVM:0xc3b020c0,jclass:0xb1dc010c,JNI_OnLoad_JavaVM:0xc3b020c0
// 1992-2027/com.example.hellojni4 I/XYCJNI: jniThreadRun JNIEnv:0xbdff70a0,JavaVM:0xc3b020c0

// LOGI 结论：
// 1-JavaVM 与进程绑定，同一进程中共享一个JavaVM地址
// 2-JNIEnv 与线程绑定，同一线程中共享一个JNIEnv地址
// 3-jobject 与实例对象绑定，同一个实例对象有相同的jobject地址
// 4-jclass 与类对象绑定，同一个类对象有相同的jclass地址