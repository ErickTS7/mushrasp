#ifndef ROOT_H
#define ROOT_H

#include <jni.h>

extern "C" JNIEXPORT jboolean JNICALL
Java_com_mush_rasp_verify_isDeviceRootedN(JNIEnv* env, jobject /* this */);



#endif // ROOT_H
