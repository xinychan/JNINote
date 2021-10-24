#include <jni.h>
#include <string>

#include <android/log.h>

// 打印log
#define TAG "XYCJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__);

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_hellojni2_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "HelloJNI2";
    return env->NewStringUTF(hello.c_str());
}

// 基本数据类型，引用类型，基本类型数组，引用类型数组；在JNI中的转换
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni2_MainActivity_testArrayAction(JNIEnv *env, jobject thiz, jint count,
                                                        jstring info, jintArray counts,
                                                        jobjectArray many_info) {
    // Java int ==> JNI jint ==> Native int
    int countInt = count; // jint 是 int 的别名，本质上 JNI jint == Native int
    LOGI("testArrayAction countInt:%d\n", countInt);

    // Java String ==> JNI jstring ==> Native char*
    // const char* GetStringUTFChars(jstring string, jboolean* isCopy)
    const char *infoStr = env->GetStringUTFChars(info, NULL);
    LOGI("testArrayAction infoStr:%s\n", infoStr);

    // Java int[] ==> JNI jintArray ==> JNI jint* == Native int*
    // jint* GetIntArrayElements(jintArray array, jboolean* isCopy)
    jint *countsIntArr = env->GetIntArrayElements(counts, NULL);
    // 获取Java数组的长度；jsize ==> jint ==> int；jsize 本质上就是 int
    // jsize GetArrayLength(jarray array)
    jsize countsSize = env->GetArrayLength(counts);
    for (int i = 0; i < countsSize; ++i) {
        // 这里的修改，只是Native层修改，不影响Java层数据
        // 还需要调用 ReleaseIntArrayElements 函数，才会刷新Java数据
        countsIntArr[i] += 100;
        LOGI("testArrayAction countsIntArr:%d\n", countsIntArr[i]);
    }
    /**
     * jint mode:
     * 0 == 提交刷新Java数据，并释放Native层数据
     * JNI_COMMIT == 只提交刷新Java数据，不释放Native层数据
     * JNI_ABORT == 不刷新Java数据，只释放Native层数据
     */
    // ReleaseIntArrayElements(jintArray array, jint* elems, jint mode)
    env->ReleaseIntArrayElements(counts, countsIntArr, NULL);

    // 获取Java数组的长度
    // jsize GetArrayLength(jarray array)
    jsize manyInfoSize = env->GetArrayLength(many_info);
    for (int i = 0; i < manyInfoSize; ++i) {
        // 获取引用类型数组中的引用对象，并强转成指定对象
        // jobject GetObjectArrayElement(jobjectArray array, jsize index)
        jstring manyInfoJstr = static_cast<jstring>(env->GetObjectArrayElement(many_info, i));
        // JNI jstring ==> Native char*
        // const char* GetStringUTFChars(jstring string, jboolean* isCopy)
        const char *manyInfoStr = env->GetStringUTFChars(manyInfoJstr, NULL);
        LOGI("testArrayAction manyInfoStr:%s\n", manyInfoStr);
        // 释放jstring和char*
        // void ReleaseStringUTFChars(jstring string, const char* utf)
        env->ReleaseStringUTFChars(manyInfoJstr, manyInfoStr);
    }
}

// 引用类型/对象；在JNI中的转换和函数调用
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni2_MainActivity_putObject(JNIEnv *env, jobject thiz,
                                                  jobject student, jstring info) {
    /**
     * Java String 的转换
     */
    // Java String ==> JNI jstring ==> Native char*
    // const char* GetStringUTFChars(jstring string, jboolean* isCopy)
    const char *infoStr = env->GetStringUTFChars(info, NULL);
    LOGI("putObject infoStr:%s\n", infoStr);
    // 释放jstring和char*
    env->ReleaseStringUTFChars(info, infoStr);

    /**
     * Java 对象的转换，和函数调用
     */
    // Java Object ==> JNI jobject ==> jclass
    // 方式1：通过包名类名获取JavaClass
    // jclass FindClass(const char* name)
    // jclass studentJclass = env->FindClass("com/example/hellojni2/Student");
    // 方式2：通过jobject获取JavaClass
    // jclass GetObjectClass(jobject obj)
    jclass studentJclass = env->GetObjectClass(student);

    // 获取对象中的函数
    // 普通函数-GetMethodID，静态函数-GetStaticMethodID
    // jmethodID GetMethodID(jclass clazz, const char* name, const char* sig)
    jmethodID setNameJmethod = env->GetMethodID(studentJclass, "setName", "(Ljava/lang/String;)V");
    jmethodID getNameJmethod = env->GetMethodID(studentJclass, "getName", "()Ljava/lang/String;");
    jmethodID showInfoJmethod = env->GetStaticMethodID(studentJclass, "showInfo",
                                                       "(Ljava/lang/String;)V");

    // 调用对象中的函数
    // 调用无返回值函数-setName
    // void CallVoidMethod(jobject obj, jmethodID methodID, ...)
    jstring name = env->NewStringUTF("JNIName");
    env->CallVoidMethod(student, setNameJmethod, name);

    // 调用返回值为引用类型的函数-getName
    // jobject (*CallObjectMethod)(JNIEnv*, jobject, jmethodID, ...);
    jstring getNameJstr = static_cast<jstring>(env->CallObjectMethod(student, getNameJmethod));
    const char *getNameValue = env->GetStringUTFChars(getNameJstr, NULL);
    LOGI("putObject getNameValue:%s\n", getNameValue);

    // 调用无返回值静态函数-showInfo
    // void CallStaticVoidMethod(jclass clazz, jmethodID methodID, ...)
    jstring showInfoJstr = env->NewStringUTF("JNIshowInfo");
    env->CallStaticVoidMethod(studentJclass, showInfoJmethod, showInfoJstr);
}

// 在JNI中直接创建Java对象
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni2_MainActivity_insertObject(JNIEnv *env, jobject thiz) {
    /**
     * 创建 Student 对象
     */
    // 通过包名类名获取JavaClass
    const char *studentClsName = "com/example/hellojni2/Student";
    jclass studentJclass = env->FindClass(studentClsName);
    // 通过JavaClass实例化对象
    // jobject AllocObject(jclass clazz)
    // AllocObject() 函数只实例化对象，不会调用对象构造函数
    // NewObject() 函数实例化对象，会调用对象构造函数
    jobject studentJobj = env->AllocObject(studentJclass);
    // 获取JavaClass中的函数
    jmethodID setNameJmethod = env->GetMethodID(studentJclass, "setName", "(Ljava/lang/String;)V");
    jmethodID setAgeJmethod = env->GetMethodID(studentJclass, "setAge", "(I)V");
    // 调用实例化对象中的函数
    jstring nameJstr = env->NewStringUTF("CLion");
    env->CallVoidMethod(studentJobj, setNameJmethod, nameJstr);
    env->CallVoidMethod(studentJobj, setAgeJmethod, 10);

    /**
     * 创建 Person 对象
     */
    // 通过包名类名获取JavaClass
    const char *personClsName = "com/example/hellojni2/Person";
    jclass personJclass = env->FindClass(personClsName);
    // 通过JavaClass实例化对象
    jobject personJobj = env->AllocObject(personJclass);
    // 获取和调用JavaClass中的普通函数
    jmethodID setStudentJmethod = env->GetMethodID(personJclass, "setStudent",
                                                   "(Lcom/example/hellojni2/Student;)V");
    env->CallVoidMethod(personJobj, setStudentJmethod, studentJobj);
    // 获取和调用JavaClass中的静态函数
    jmethodID putStudentJmethod = env->GetStaticMethodID(personJclass, "putStudent",
                                                         "(Lcom/example/hellojni2/Student;)V");
    env->CallStaticVoidMethod(personJclass, putStudentJmethod, studentJobj);

    // 当前函数中；jclass/jobject/jstring 等都是局部引用；局部引用在函数结束后，会自动释放
    // 但是仍然优先主动释放，主动优化函数调用后的内存
    env->DeleteLocalRef(studentJclass);
    env->DeleteLocalRef(studentJobj);
    env->DeleteLocalRef(personJclass);
    env->DeleteLocalRef(personJobj);
    // jstring 的释放，调用 GetStringUTFChars 后，要释放 ReleaseStringUTFChars
    // env->GetStringUTFChars()
    // env->ReleaseStringUTFChars()
}

jclass dogJclass;

// 在JNI中引用类型的释放处理-局部引用
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni2_MainActivity_testQuote(JNIEnv *env, jobject thiz) {
    if (NULL == dogJclass) {
        // 通过包名类名获取JavaClass
        const char *dogClsName = "com/example/hellojni2/Dog";
        // 此时的 dogJclass 是局部引用，函数执行完就被自动释放
        // 内存被释放，但是指针仍有指向地址，dogJclass != NULL
        // 需要手动 dogJclass == NULL，否则下次进入函数不会初始化 dogJclass
        dogJclass = env->FindClass(dogClsName);
    }
    // 获取JavaClass的构造函数；构造函数的函数名都是"<init>"
    jmethodID dogInit = env->GetMethodID(dogJclass, "<init>", "()V");
    jmethodID dogInit1 = env->GetMethodID(dogJclass, "<init>", "(I)V");
    jmethodID dogInit2 = env->GetMethodID(dogJclass, "<init>", "(II)V");
    jmethodID dogInit3 = env->GetMethodID(dogJclass, "<init>", "(III)V");
    // 通过构造函数实例化对象
    jobject dogJobj = env->NewObject(dogJclass, dogInit);
    jobject dogJobj1 = env->NewObject(dogJclass, dogInit1, 10);
    jobject dogJobj2 = env->NewObject(dogJclass, dogInit2, 20, 20);
    jobject dogJobj3 = env->NewObject(dogJclass, dogInit3, 30, 30, 30);
    // 主动释放局部引用
    env->DeleteLocalRef(dogJobj);
    env->DeleteLocalRef(dogJobj1);
    env->DeleteLocalRef(dogJobj2);
    env->DeleteLocalRef(dogJobj3);
    env->DeleteLocalRef(dogJclass);
    // 手动 dogJclass == NULL
    dogJclass = NULL;
}

// 声明在其他 cpp 文件实现的属性和函数
extern int age;

// 声明在其他 cpp 文件实现的属性和函数
extern void show(int);

// 在JNI中引用类型的释放处理-全局引用
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hellojni2_MainActivity_testQuote2(JNIEnv *env, jobject thiz) {
    // 调用其他 cpp 文件实现的函数
    show(age);

    if (NULL == dogJclass) {
        // 通过包名类名获取JavaClass
        const char *dogClsName = "com/example/hellojni2/Dog";
        jclass tempJclass = env->FindClass(dogClsName);
        // 此时的 dogJclass 是全局引用，函数执行完不会自动释放，需要手动释放
        dogJclass = static_cast<jclass>(env->NewGlobalRef(tempJclass));
        // 主动释放局部引用
        env->DeleteLocalRef(tempJclass);
        tempJclass = NULL;
    }
    // 获取JavaClass的构造函数；构造函数的函数名都是"<init>"
    jmethodID dogInit = env->GetMethodID(dogJclass, "<init>", "()V");
    jmethodID dogInit1 = env->GetMethodID(dogJclass, "<init>", "(I)V");
    jmethodID dogInit2 = env->GetMethodID(dogJclass, "<init>", "(II)V");
    jmethodID dogInit3 = env->GetMethodID(dogJclass, "<init>", "(III)V");
    // 通过构造函数实例化对象
    jobject dogJobj = env->NewObject(dogJclass, dogInit);
    jobject dogJobj1 = env->NewObject(dogJclass, dogInit1, 10);
    jobject dogJobj2 = env->NewObject(dogJclass, dogInit2, 20, 20);
    jobject dogJobj3 = env->NewObject(dogJclass, dogInit3, 30, 30, 30);
    // 主动释放局部引用
    env->DeleteLocalRef(dogJobj);
    env->DeleteLocalRef(dogJobj1);
    env->DeleteLocalRef(dogJobj2);
    env->DeleteLocalRef(dogJobj3);
    // 主动释放全局引用
    if (dogJclass != NULL) {
        env->DeleteGlobalRef(dogJclass);
        dogJclass = NULL;
    }
}