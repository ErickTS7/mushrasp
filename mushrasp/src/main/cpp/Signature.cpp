#include <jni.h>
#include <android/log.h>
#include <string>
#include <vector>

#define LOG_TAG "MushroomRASP"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_mushroom_rasp_verify_validateAppSignatureN(JNIEnv *env, jclass clazz, jobject context, jstring expectedSignature) {
    // Pegando o package manager e package info
    jclass contextClass = env->GetObjectClass(context);
    jmethodID getPackageManager = env->GetMethodID(contextClass, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject packageManager = env->CallObjectMethod(context, getPackageManager);

    jmethodID getPackageName = env->GetMethodID(contextClass, "getPackageName", "()Ljava/lang/String;");
    jstring packageName = (jstring) env->CallObjectMethod(context, getPackageName);

    jclass packageManagerClass = env->GetObjectClass(packageManager);
    jmethodID getPackageInfo = env->GetMethodID(packageManagerClass, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jobject packageInfo = env->CallObjectMethod(packageManager, getPackageInfo, packageName, 0x40); // PackageManager.GET_SIGNATURES

    jclass packageInfoClass = env->GetObjectClass(packageInfo);
    jfieldID signaturesField = env->GetFieldID(packageInfoClass, "signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signaturesArray = (jobjectArray) env->GetObjectField(packageInfo, signaturesField);

    jobject signature = env->GetObjectArrayElement(signaturesArray, 0);
    jclass signatureClass = env->GetObjectClass(signature);
    jmethodID toByteArray = env->GetMethodID(signatureClass, "toByteArray", "()[B");
    jbyteArray signatureByteArray = (jbyteArray) env->CallObjectMethod(signature, toByteArray);

    // Convertendo a assinatura real para uma string (em formato hash, por exemplo)
    jclass messageDigestClass = env->FindClass("java/security/MessageDigest");
    jmethodID getInstance = env->GetStaticMethodID(messageDigestClass, "getInstance", "(Ljava/lang/String;)Ljava/security/MessageDigest;");
    jstring sha256 = env->NewStringUTF("SHA-256");
    jobject messageDigest = env->CallStaticObjectMethod(messageDigestClass, getInstance, sha256);

    jmethodID update = env->GetMethodID(messageDigestClass, "update", "([B)V");
    env->CallVoidMethod(messageDigest, update, signatureByteArray);

    jmethodID digest = env->GetMethodID(messageDigestClass, "digest", "()[B");
    jbyteArray hashByteArray = (jbyteArray) env->CallObjectMethod(messageDigest, digest);

    jsize hashLength = env->GetArrayLength(hashByteArray);
    std::vector<unsigned char> hash(hashLength);
    env->GetByteArrayRegion(hashByteArray, 0, hashLength, reinterpret_cast<jbyte *>(hash.data()));

    // Convertendo o hash para uma string hexadecimal
    std::string signatureHash;
    char hexBuffer[3];
    for (unsigned char byte : hash) {
        sprintf(hexBuffer, "%02X", byte);
        signatureHash += hexBuffer;
    }


    //LOGE("Hash gerado: %s", signatureHash.c_str());



    const char *expectedSignatureCStr = env->GetStringUTFChars(expectedSignature, nullptr);
    std::string expectedSignatureStr(expectedSignatureCStr);
    env->ReleaseStringUTFChars(expectedSignature, expectedSignatureCStr);

    // Comparando as assinaturas
    if (signatureHash == expectedSignatureStr) {
        LOGE("[-] Valid signature");
        return JNI_TRUE; // Assinatura válida
    } else {
        LOGE("[!] Invalid Signature");
        return JNI_FALSE; // Assinatura inválida
    }
}
