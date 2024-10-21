#include "EmulatorDetect.h"
#include <android/log.h>
#include <unistd.h>
#include <sys/system_properties.h>
#include <ctime>
#include <cstring>
#include <string>

#define LOG_TAG "EmulatorDetect"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

std::string stripQuotes(const std::string &str) {
    if (str.size() >= 2 && str.front() == '"' && str.back() == '"') {
        return str.substr(1, str.size() - 2);
    }
    return str;
}


extern "C" {

JNIEXPORT jboolean JNICALL Java_com_mushroom_rasp_verify_checkArchitecture(JNIEnv *env, jobject, jobject context) {
    return EmulatorDetect::checkArchitecture(env, context);
}

JNIEXPORT jboolean JNICALL Java_com_mushroom_rasp_verify_checkHostname(JNIEnv *env, jobject, jobject context) {
    return EmulatorDetect::checkHostname(env, context);
}

JNIEXPORT jboolean JNICALL Java_com_mushroom_rasp_verify_checkContacts(JNIEnv *env, jobject, jobject context) {
    return EmulatorDetect::checkContacts(env, context);
}

JNIEXPORT jboolean JNICALL Java_com_mushroom_rasp_verify_checkWifiSSID(JNIEnv *env, jobject, jobject context) {
    return EmulatorDetect::checkWifiSSID(env, context);
}

JNIEXPORT jboolean JNICALL Java_com_mushroom_rasp_verify_checkUptime(JNIEnv *env, jobject, jobject context) {
    return EmulatorDetect::checkUptime(env, context);
}

}


bool EmulatorDetect::checkArchitecture(JNIEnv *env, jobject context) {
    char value[PROP_VALUE_MAX];
    __system_property_get("ro.product.cpu.abi", value);
    LOGE("Emulator[Architecture]: %s", value);
    return strstr(value, "x86") != nullptr;
}

bool EmulatorDetect::checkHostname(JNIEnv *env, jobject context) {
    char value[PROP_VALUE_MAX];
    __system_property_get("ro.product.device", value);
    LOGE("Emulator[Name]: %s", value);
    if (strstr(value, "generic") || strstr(value, "sdk") || strstr(value, "x86")) {
        return true;
    }
    __system_property_get("ro.product.model", value);
    LOGE("Emulator[Model]: %s", value);
    return strstr(value, "generic") || strstr(value, "sdk") || strstr(value, "x86");
}

bool EmulatorDetect::checkContacts(JNIEnv *env, jobject context) {
    jclass contextClass = env->GetObjectClass(context);
    jmethodID getContentResolver = env->GetMethodID(contextClass, "getContentResolver", "()Landroid/content/ContentResolver;");
    jobject contentResolver = env->CallObjectMethod(context, getContentResolver);

    jclass contactsContractClass = env->FindClass("android/provider/ContactsContract$Contacts");
    jfieldID contentUriField = env->GetStaticFieldID(contactsContractClass, "CONTENT_URI", "Landroid/net/Uri;");
    jobject contentUri = env->GetStaticObjectField(contactsContractClass, contentUriField);

    jclass contentResolverClass = env->GetObjectClass(contentResolver);
    jmethodID queryMethod = env->GetMethodID(contentResolverClass, "query", "(Landroid/net/Uri;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)Landroid/database/Cursor;");

    jobject cursor = env->CallObjectMethod(contentResolver, queryMethod, contentUri, NULL, NULL, NULL, NULL);

    jclass cursorClass = env->GetObjectClass(cursor);
    jmethodID getCountMethod = env->GetMethodID(cursorClass, "getCount", "()I");
    jint count = env->CallIntMethod(cursor, getCountMethod);

    LOGE("Emulator[Contacts]: %d", count);
    return count <= 3;
}

bool EmulatorDetect::checkWifiSSID(JNIEnv *env, jobject context) {
    jclass contextClass = env->GetObjectClass(context);
    jmethodID getSystemService = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    jstring wifiServiceString = env->NewStringUTF("wifi");
    jobject wifiManager = env->CallObjectMethod(context, getSystemService, wifiServiceString);

    jclass wifiManagerClass = env->GetObjectClass(wifiManager);
    jmethodID getConnectionInfo = env->GetMethodID(wifiManagerClass, "getConnectionInfo", "()Landroid/net/wifi/WifiInfo;");
    jobject wifiInfo = env->CallObjectMethod(wifiManager, getConnectionInfo);

    jclass wifiInfoClass = env->GetObjectClass(wifiInfo);
    jmethodID getSSID = env->GetMethodID(wifiInfoClass, "getSSID", "()Ljava/lang/String;");
    jstring ssid = (jstring) env->CallObjectMethod(wifiInfo, getSSID);

    const char *ssidChars = env->GetStringUTFChars(ssid, 0);
    std::string ssidString = stripQuotes(ssidChars);
    LOGE("Emulator[WifiSSID]: %s", ssidString.c_str());

    bool result = ssidString == "AndroidWifi";
    env->ReleaseStringUTFChars(ssid, ssidChars);
    return result;
}

bool EmulatorDetect::checkUptime(JNIEnv *env, jobject context) {
    struct timespec ts;
    clock_gettime(CLOCK_BOOTTIME, &ts);
    long uptimeSeconds = ts.tv_sec;
    long uptimeHours = uptimeSeconds / 3600;
    LOGE("Emulator[Uptime]: %ld hours", uptimeHours);
    return uptimeHours < 1;
}
