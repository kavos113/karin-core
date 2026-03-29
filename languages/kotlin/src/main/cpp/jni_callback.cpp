#include "jni_callback.h"

namespace karin::jni
{
JniVoidCallback::JniVoidCallback(JNIEnv* env, jobject listener)
{
    if (env->GetJavaVM(&m_jvm) != 0)
    {
        m_jvm = nullptr;
        m_globalObj = nullptr;
        return;
    }

    m_globalObj = env->NewGlobalRef(listener);
}

JniVoidCallback::~JniVoidCallback()
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

void JniVoidCallback::invoke(const std::string& methodName, const std::string& methodSig) const
{
    bool shouldDetach;
    JNIEnv* env = getEnv(shouldDetach);
    if (!env)
    {
        return;
    }

    jclass listenerClass = env->GetObjectClass(m_globalObj);
    jmethodID methodId = env->GetMethodID(listenerClass, methodName.c_str(), methodSig.c_str());
    if (methodId)
    {
        env->CallVoidMethod(m_globalObj, methodId);
        if (env->ExceptionCheck())
        {
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
    }

    env->DeleteLocalRef(listenerClass);

    if (shouldDetach)
    {
        m_jvm->DetachCurrentThread();
    }
}

JNIEnv* JniVoidCallback::getEnv(bool& shouldDetach) const
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