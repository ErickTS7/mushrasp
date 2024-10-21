#include "MushEnvironment.h"
#include "EmulatorDetect.h"
#include <android/log.h>

#define LOG_TAG "MushEnvironment"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_mushroom_rasp_verify_isEmulatedN(JNIEnv *env, jclass /* this */, jobject context) {
    MushEnvironment mushEnvironment(env, context);
    return mushEnvironment.isEmulatedN();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_mushroom_rasp_verify_isRootedN(JNIEnv *env, jclass /* this */, jobject context) {
    MushEnvironment mushEnvironment(env, context);
    return mushEnvironment.isRootedN();
}

MushEnvironment::MushEnvironment(JNIEnv *env, jobject context) {
    this->env = env;
    this->context = context;
}


bool MushEnvironment::isEmulatedN() {
    int pts = 0;

    if (EmulatorDetect::checkArchitecture(env, context)) {
        pts += 2;
        LOGE("+ Architecture check detected emulator");
    }
    if (EmulatorDetect::checkHostname(env, context)) {
        pts += 3;
        LOGE("+ Hostname check detected emulator");
    }
    if (EmulatorDetect::checkContacts(env, context)) {
        pts += 2;
        LOGE("+ Contacts check detected emulator");
    }
    if (EmulatorDetect::checkWifiSSID(env, context)) {
        pts += 3;
        LOGE("+ WifiSSID check detected emulator");
    }
    if (EmulatorDetect::checkUptime(env, context)) {
        pts += 1;
        LOGE("+ Uptime check detected emulator");
    }

    LOGE("Emulator[Points]: %d", pts);
    return (pts >= 3);
}

bool MushEnvironment::isRootedN() {
    jclass rootBeerClass = env->FindClass("com/scottyab/rootbeer/RootBeer");
    if (!rootBeerClass) {
        LOGE("Failed to find RootBeer class");
        return false;
    }

    jmethodID constructor = env->GetMethodID(rootBeerClass, "<init>", "(Landroid/content/Context;)V");
    if (!constructor) {
        LOGE("Failed to find RootBeer constructor");
        return false;
    }

    jobject rootBeerObject = env->NewObject(rootBeerClass, constructor, context);
    if (!rootBeerObject) {
        LOGE("Failed to create RootBeer object");
        return false;
    }

    jmethodID isRootedMethod = env->GetMethodID(rootBeerClass, "isRooted", "()Z");
    if (!isRootedMethod) {
        LOGE("Failed to find isRooted method");
        return false;
    }

    jboolean isRooted = env->CallBooleanMethod(rootBeerObject, isRootedMethod);
    env->DeleteLocalRef(rootBeerClass);
    env->DeleteLocalRef(rootBeerObject);

    if (isRooted) {
        LOGE("+ [Rootbeer]: Rooted!");
    } else {
        LOGE("- [Rootbeer]: Not rooted.");
    }

    return isRooted;
}
