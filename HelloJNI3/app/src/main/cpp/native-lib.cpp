#include <jni.h>
#include <string>
#include <unistd.h>

// TODO 007--导入 FMOD 模块头文件
#include <fmod.hpp>

#include <android/log.h>

// 打印log
#define TAG "XYCJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__);

// MainActivity 定义的声音类型
#undef com_example_hellojni3_MainActivity_MODE_NORMAL
#define com_example_hellojni3_MainActivity_MODE_NORMAL 0L
#undef com_example_hellojni3_MainActivity_MODE_LUOLI
#define com_example_hellojni3_MainActivity_MODE_LUOLI 1L
#undef com_example_hellojni3_MainActivity_MODE_DASHU
#define com_example_hellojni3_MainActivity_MODE_DASHU 2L
#undef com_example_hellojni3_MainActivity_MODE_JINGSONG
#define com_example_hellojni3_MainActivity_MODE_JINGSONG 3L
#undef com_example_hellojni3_MainActivity_MODE_GAOGUAI
#define com_example_hellojni3_MainActivity_MODE_GAOGUAI 4L
#undef com_example_hellojni3_MainActivity_MODE_KONGLING
#define com_example_hellojni3_MainActivity_MODE_KONGLING 5L

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_hellojni3_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "HelloJNI3";
    return env->NewStringUTF(hello.c_str());
}

/*
 * Class:     com_example_hellojni3_MainActivity
 * Method:    voiceChangeNative
 * Signature: (ILjava/lang/String;)V
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_hellojni3_MainActivity_voiceChangeNative
        (JNIEnv *env, jobject thiz, jint mode, jstring path) {
    LOGI("voiceChangeNative start");
    // 音源路径
    const char *audioPath = env->GetStringUTFChars(path, NULL);
    // 初始化音效系统引擎
    FMOD::System *system = NULL;
    // 初始化声音
    FMOD::Sound *sound = NULL;
    // 初始化音轨
    FMOD::Channel *channel = NULL;
    // 初始化DSP：digital signal process  == 数字信号处理
    FMOD::DSP *dsp = NULL;

    // TODO action001--创建音效系统
    FMOD::System_Create(&system);
    // TODO action002--初始化系统参数
    // maxchannels == 最大音轨数；flags == 系统初始化标记；extradriverdata == 额外数据
    // init(int maxchannels, FMOD_INITFLAGS flags, void *extradriverdata);
    system->init(32, FMOD_INIT_NORMAL, 0);
    // TODO action003--创建声音
    // name_or_data == 音源路径；mode == 声音模式；exinfo == 额外数据；sound == 声音指针
    // createSound(const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound);
    system->createSound(audioPath, FMOD_DEFAULT, 0, &sound);
    // TODO action004--播放声音
    // sound == 声音指针；channelgroup == 分组音轨；paused == 控制；channel == 音轨
    // playSound(Sound *sound, ChannelGroup *channelgroup, bool paused, Channel **channel);
    system->playSound(sound, 0, false, &channel);
    // TODO action005--增加特效
    char *palyerEndTip = NULL; // 播放结束提示语
    switch (mode) {
        case com_example_hellojni3_MainActivity_MODE_NORMAL:
            LOGI("playSound MODE_NORMAL");
            palyerEndTip = const_cast<char *>("playEnd MODE_NORMAL");
            break;
        case com_example_hellojni3_MainActivity_MODE_LUOLI:
            LOGI("playSound MODE_LUOLI");
            palyerEndTip = const_cast<char *>("playEnd MODE_LUOLI");
            // 音调高 -- 萝莉 2.0
            // 1.创建DSP类型的Pitch，调节音调
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            // 2.设置Pitch音调调节到2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 2.0f);
            // 3.添加音效到音轨
            channel->addDSP(0, dsp);
            break;
        case com_example_hellojni3_MainActivity_MODE_DASHU:
            LOGI("playSound MODE_DASHU");
            palyerEndTip = const_cast<char *>("playEnd MODE_DASHU");
            // 音调低 -- 大叔 0.7
            // 1.创建DSP类型的Pitch，调节音调
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            // 2.设置Pitch音调调节到2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7f);
            // 3.添加音效到音轨
            channel->addDSP(0, dsp);
            break;
        case com_example_hellojni3_MainActivity_MODE_JINGSONG:
            LOGI("playSound MODE_JINGSONG");
            palyerEndTip = const_cast<char *>("playEnd MODE_JINGSONG");
            // 惊悚音效：特点：很多声音的拼接
            // 通过多个音频拼接多个特效

            // 音调低 -- 大叔 0.7
            // 1.创建DSP类型的Pitch，调节音调
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            // 2.设置Pitch音调调节到2.0
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7f);
            // 3.添加音效到音轨
            channel->addDSP(0, dsp); // 第1个音轨

            // 增加回音音效 ECHO
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200); // 回音 延时
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 10); // 回音 衰减度
            channel->addDSP(1, dsp); // 第2个音轨

            // 增加颤抖音效 TREMOLO
            system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_FREQUENCY, 20);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 0.8f);
            channel->addDSP(2, dsp); // 第3个音轨

            break;
        case com_example_hellojni3_MainActivity_MODE_GAOGUAI:
            LOGI("playSound MODE_GAOGUAI");
            palyerEndTip = const_cast<char *>("playEnd MODE_GAOGUAI");
            // 小黄人声音 频率快
            // 从音轨获取当前声音频率
            float mFrequency;
            channel->getFrequency(&mFrequency);
            // 修改音轨的频率
            channel->setFrequency(mFrequency * 1.5f);
            break;
        case com_example_hellojni3_MainActivity_MODE_KONGLING:
            LOGI("playSound MODE_KONGLING");
            palyerEndTip = const_cast<char *>("playEnd MODE_KONGLING");
            // 增加回音音效 ECHO
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200); // 回音 延时
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 10); // 回音 衰减度
            channel->addDSP(0, dsp);
            break;
        default:
            LOGI("playSound default");
            break;
    }

    // 判断是否播放完毕；播放完毕释放资源
    bool isPlayer = true;
    while (isPlayer) {
        // 每隔1秒判断是否播放完毕
        channel->isPlaying(&isPlayer);
        usleep(1000 * 1000);
    }

    // 释放资源
    sound->release();
    system->close();
    system->release();
    env->ReleaseStringUTFChars(path, audioPath);

    // 调用Java层函数，告知播放完毕
    jclass mainActivityClass = env->GetObjectClass(thiz);
    jmethodID playerEndMethod = env->GetMethodID(mainActivityClass, "playerEnd",
                                                 "(Ljava/lang/String;)V");
    jstring tip = env->NewStringUTF(palyerEndTip);
    env->CallVoidMethod(thiz, playerEndMethod, tip);
};