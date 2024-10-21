#ifndef EMULATOR_DETECT_H
#define EMULATOR_DETECT_H

#include <jni.h>


extern "C" {
JNIEXPORT jboolean JNICALL Java_com_mushroom_rasp_EmulatorDetect_checkArchitecture(JNIEnv *, jobject, jobject context);
JNIEXPORT jboolean JNICALL Java_com_mushroom_rasp_EmulatorDetect_checkHostname(JNIEnv *, jobject, jobject context);
JNIEXPORT jboolean JNICALL Java_com_mushroom_rasp_EmulatorDetect_checkContacts(JNIEnv *, jobject, jobject context);
JNIEXPORT jboolean JNICALL Java_com_mushroom_rasp_EmulatorDetect_checkWifiSSID(JNIEnv *, jobject, jobject context);
JNIEXPORT jboolean JNICALL Java_com_mushroom_rasp_EmulatorDetect_checkUptime(JNIEnv *, jobject, jobject context);
}


class EmulatorDetect {
public:
    static bool checkArchitecture(JNIEnv *env, jobject context);
    static bool checkHostname(JNIEnv *env, jobject context);
    static bool checkContacts(JNIEnv *env, jobject context);
    static bool checkWifiSSID(JNIEnv *env, jobject context);
    static bool checkUptime(JNIEnv *env, jobject context);
};

#endif // EMULATOR_DETECT_H
