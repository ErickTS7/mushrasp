#include "root.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/system_properties.h>

bool checkBinary(const char* binary) {
    struct stat buffer;
    return (stat(binary, &buffer) == 0);
}

bool checkRootMethod1() {
    const char* paths[] = {
            "/sbin/su",
            "/system/bin/su",
            "/system/xbin/su",
            "/data/local/xbin/su",
            "/data/local/bin/su",
            "/system/sd/xbin/su",
            "/system/bin/failsafe/su",
            "/data/local/su"
    };

    for (const auto& path : paths) {
        if (checkBinary(path)) {
            return true;
        }
    }
    return false;
}

bool checkRootMethod2() {
    const char* paths[] = {
            "/system/app/Superuser.apk",
            "/sbin/su",
            "/system/bin/su",
            "/system/xbin/su",
            "/data/local/xbin/su",
            "/data/local/bin/su",
            "/system/sd/xbin/su",
            "/system/bin/failsafe/su",
            "/data/local/su",
            "/system/xbin/daemonsu"
    };

    for (const auto& path : paths) {
        if (checkBinary(path)) {
            return true;
        }
    }
    return false;
}

bool checkRootMethod3() {
    const char* commands[] = {
            "which su",
            "busybox which su",
            "/system/xbin/which su",
            "/system/bin/which su"
    };

    for (const auto& command : commands) {
        FILE* f = popen(command, "r");
        if (f != nullptr) {
            char result[128];
            if (fgets(result, sizeof(result), f) != nullptr) {
                pclose(f);
                return true;
            }
            pclose(f);
        }
    }
    return false;
}

bool checkBuildTags() {
    char tags[PROP_VALUE_MAX];
    __system_property_get("ro.build.tags", tags);
    return (strstr(tags, "test-keys") != nullptr);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_mushroom_rasp_verify_isDeviceRootedN(JNIEnv *env, jclass /* this */, jobject context) {
    return (checkRootMethod1() || checkRootMethod2() || checkRootMethod3() || checkBuildTags());
}

