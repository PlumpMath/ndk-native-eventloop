#include "event-loop.h"
#include <chrono>
#include <random>
#include <thread>

static JavaVM* JVM = nullptr;
static jclass EventEmitterCls = nullptr;
static jmethodID ConstructorId = nullptr;
static jmethodID GetEmitterPtrId = nullptr;
static std::map<std::thread::id, std::thread*> RunningThreads = std::map<std::thread::id, std::thread*>();

void EventEmitter::On(std::string evtName, EventCallback callback)
{
    std::unique_lock<std::mutex> lk(listenersMutex_);

    if (listeners_.find(evtName) == listeners_.end())
    {
        listeners_[evtName] = std::make_shared<std::vector<EventCallback>>();
    }

    listeners_[evtName]->push_back(callback);
}

void EventEmitter::Emit(std::string evtName, std::string evtData)
{
    for (auto& it : *listeners_[evtName]) {
        it(evtData);
    }
}

template<typename T>
static T MakeGlobal(JNIEnv *env, T obj) {
    return reinterpret_cast<T>(env->NewGlobalRef(obj));
}

JNIEXPORT jobject JNICALL Java_com_example_cosnita_gettingstartedndk_EventEmitter_getInstance(JNIEnv *env, jobject thisObj)
{
    if (EventEmitterCls == nullptr) {
        env->GetJavaVM(&JVM);
        EventEmitterCls = MakeGlobal<jclass>(env, env->FindClass("com/example/cosnita/gettingstartedndk/EventEmitter"));
        ConstructorId = env->GetMethodID(EventEmitterCls, "<init>", "(J)V");
        GetEmitterPtrId = env->GetMethodID(EventEmitterCls, "getEmitterPtrId", "()J");
    }

    auto evtEmitter = new EventEmitter();
    jlong emitterId = reinterpret_cast<std::uintptr_t>(evtEmitter);
    auto objInstance = env->NewObject(EventEmitterCls, ConstructorId, emitterId);

    return objInstance;
}

JNIEXPORT void JNICALL Java_com_example_cosnita_gettingstartedndk_EventEmitter_on(
        JNIEnv *env,
        jobject thisObj,
        jstring evtName,
        jobject callback)
{
    auto emitterPtr = env->CallLongMethod(thisObj, GetEmitterPtrId);
    auto emitter = reinterpret_cast<EventEmitter*>(emitterPtr);
    std::string evtNameStd(env->GetStringUTFChars(evtName, nullptr));

    auto callbackClassGlobal = MakeGlobal<jclass>(env, env->GetObjectClass(callback));
    auto callbackGlobal = MakeGlobal<jobject>(env, callback);
    emitter->On(evtNameStd, [callbackClassGlobal, callbackGlobal](std::string evtData) {
        auto thread = new std::thread([callbackClassGlobal, callbackGlobal, evtData]() {
            double sleepDuration = std::rand() % 2 * 1000;
            std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(sleepDuration));
            JNIEnv *env = nullptr;
            JVM->AttachCurrentThread(&env, nullptr);
            auto actionId = env->GetMethodID(callbackClassGlobal, "doAction", "(Ljava/lang/String;)V");
            auto evtDataJNI = env->NewStringUTF(evtData.c_str());
            env->CallVoidMethod(callbackGlobal, actionId, evtDataJNI);
            JVM->DetachCurrentThread();

            auto threadId = std::this_thread::get_id();
            auto thread = RunningThreads[threadId];
            thread->detach();
            delete thread;
            RunningThreads.erase(threadId);
        });

        RunningThreads[thread->get_id()] = thread;
    });
}

JNIEXPORT void JNICALL Java_com_example_cosnita_gettingstartedndk_EventEmitter_emit(
        JNIEnv *env,
        jobject thisObj,
        jstring evtName,
        jstring evtData)
{
    std::string evtNameStd(env->GetStringUTFChars(evtName, nullptr));
    std::string evtDataStd(env->GetStringUTFChars(evtData, nullptr));
    auto emitterPtr = env->CallLongMethod(thisObj, GetEmitterPtrId);
    auto emitter = reinterpret_cast<EventEmitter*>(emitterPtr);

    emitter->Emit(evtNameStd, evtDataStd);
}