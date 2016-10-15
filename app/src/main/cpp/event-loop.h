#ifndef GETTINGSTARTEDNDK_EVENT_LOOP_H
#define GETTINGSTARTEDNDK_EVENT_LOOP_H

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

class EventEmitter {
using EventCallback = std::function<void(std::string)>;
public:
    void On(std::string evtName, EventCallback callback);

    void Emit(std::string evtName, std::string evtData);

private:
    std::map<std::string, std::shared_ptr<std::vector<EventCallback>>> listeners_;
    std::mutex listenersMutex_;
};

#include <jni.h>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jobject JNICALL Java_com_example_cosnita_gettingstartedndk_EventEmitter_getInstance(JNIEnv *env, jobject thisObj);

JNIEXPORT void JNICALL Java_com_example_cosnita_gettingstartedndk_EventEmitter_on(
        JNIEnv *env,
        jobject thisObj,
        jstring evtName,
        jobject callback);

JNIEXPORT void JNICALL Java_com_example_cosnita_gettingstartedndk_EventEmitter_emit(
        JNIEnv *env,
        jobject thisObj,
        jstring evtName,
        jstring evtData);

#ifdef __cplusplus
}
#endif

#endif //GETTINGSTARTEDNDK_EVENT_LOOP_H
