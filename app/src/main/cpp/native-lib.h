#ifndef GETTINGSTARTEDNDK_NATIVE_LIB_H
#define GETTINGSTARTEDNDK_NATIVE_LIB_H

#include <jni.h>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jstring JNICALL Java_com_example_cosnita_gettingstartedndk_NativeWrapper_sayHello(JNIEnv *, jobject);

JNIEXPORT jstring JNICALL Java_com_example_cosnita_gettingstartedndk_MainActivity_sayHello(JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif

#endif //GETTINGSTARTEDNDK_NATIVE_LIB_H
