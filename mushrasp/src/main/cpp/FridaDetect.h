#ifndef FRIDA_DETECT_H
#define FRIDA_DETECT_H

#include <jni.h> // Adicione esta linha para garantir que o JNI esteja incluído
#include <string>

extern "C" {
    JNIEXPORT jboolean JNICALL Java_com_mushroom_rasp_verify_detectFridaAllPorts(JNIEnv *, jclass, jobject context);
    JNIEXPORT jboolean JNICALL Java_com_mushroom_rasp_verify_detectFridaDefaultPort(JNIEnv *, jclass, jobject context);
}

class FridaDetect {
public:
    static bool detectFridaAllPorts();
    static bool detectFridaDefaultPort();
};

#endif // FRIDA_DETECT_H
