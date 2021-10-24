#include <android/log.h>

// 打印log
#define TAG "XYCJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__);


int age = 100;

// 函数的实现，供其他 cpp 文件调用
void show(int num) {
    LOGI("Test.cpp show:%d\n", num);
}

