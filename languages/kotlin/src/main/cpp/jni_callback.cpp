#include "jni_callback.h"

namespace karin::jni
{
JniGlobalRef::JniGlobalRef(JNIEnv* env, jobject listener)
{
    if (env->GetJavaVM(&m_jvm) != 0)
    {
        m_jvm = nullptr;
        m_globalObj = nullptr;
        return;
    }

    m_globalObj = env->NewGlobalRef(listener);
}

JniGlobalRef::~JniGlobalRef()
{
    if (m_globalObj)
    {
        bool shouldDetach;
        JNIEnv* env = getEnv(shouldDetach);
        if (env)
        {
            env->DeleteGlobalRef(m_globalObj);
        }

        if (shouldDetach)
        {
            m_jvm->DetachCurrentThread();
        }
    }
}

template <typename Func>
void JniGlobalRef::invoke(Func&& func) const
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

JNIEnv* JniGlobalRef::getEnv(bool& shouldDetach) const
{
    JNIEnv* env = nullptr;
    shouldDetach = false;

    jint getEnvResult = m_jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    if (getEnvResult == JNI_EDETACHED)
    {
        if (m_jvm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr) == 0)
        {
            shouldDetach = true;
        }
        else
        {
            return nullptr;
        }
    }

    return env;
}
} // karin::jni