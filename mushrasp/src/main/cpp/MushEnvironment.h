#ifndef MUSHENVIRONMENT_H
#define MUSHENVIRONMENT_H

#include <jni.h>
#include <string>


class MushEnvironment {
public:
    MushEnvironment(JNIEnv *env, jobject context);
    bool isEmulatedN();
    bool isRootedN();

private:
    JNIEnv *env;
    jobject context;
};

#endif // MUSHENVIRONMENT_H
