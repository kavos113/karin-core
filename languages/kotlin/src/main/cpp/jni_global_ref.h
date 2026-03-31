#ifndef LANGUAGES_KOTLIN_SRC_MAIN_CPP_JNI_CALLBACK_H
#define LANGUAGES_KOTLIN_SRC_MAIN_CPP_JNI_CALLBACK_H

#include <jni.h>

#include <string>

namespace karin::jni
{

class JniGlobalRef
{
public:
    JniGlobalRef(JNIEnv* env, jobject listener);
    ~JniGlobalRef();

    JniGlobalRef(const JniGlobalRef&) = delete;
    JniGlobalRef& operator=(const JniGlobalRef&) = delete;
    JniGlobalRef(JniGlobalRef&&) = delete;
    JniGlobalRef& operator=(JniGlobalRef&&) = delete;

    template<typename Func>
    void invoke(Func&& func) const
    {
        bool shouldDetach;
        JNIEnv* env = getEnv(shouldDetach);
        if (!env)
        {
            return;
        }

        func(env, m_globalObj);
        if (env->ExceptionCheck())
        {
            env->ExceptionDescribe();
            env->ExceptionClear();
        }

        if (shouldDetach)
        {
            m_jvm->DetachCurrentThread();
        }
    }

private:
    JNIEnv* getEnv(bool& shouldDetach) const;

    JavaVM* m_jvm = nullptr;
    jobject m_globalObj;
};

} // karin::jni

#endif //LANGUAGES_KOTLIN_SRC_MAIN_CPP_JNI_CALLBACK_H
