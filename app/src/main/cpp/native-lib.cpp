#include <iostream>
#include <jni.h>
#include <string>
#include "native-lib.h"

static const char* ANA = "Ana are mere ...";

JNIEXPORT jstring JNICALL Java_com_example_cosnita_gettingstartedndk_NativeWrapper_sayHello(
        JNIEnv *env, jobject) {
    return env->NewStringUTF(ANA);
}

JNIEXPORT jstring JNICALL Java_com_example_cosnita_gettingstartedndk_MainActivity_sayHello(JNIEnv *env, jobject, jstring msg)
{
    jboolean isCopy;
    auto input = env->GetStringChars(msg, &isCopy);
    std::cout << "Received from Java: " << input << std::endl;

    return env->NewStringUTF(ANA);
}