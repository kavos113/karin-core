#ifndef LANGUAGES_KOTLIN_SRC_MAIN_CPP_JNI_CALLBACK_H
#define LANGUAGES_KOTLIN_SRC_MAIN_CPP_JNI_CALLBACK_H

#include <jni.h>

#include <string>

namespace karin::jni
{

class JniVoidCallback
{
public:
    JniVoidCallback(JNIEnv* env, jobject listener);
    ~JniVoidCallback();

    JniVoidCallback(const JniVoidCallback&) = delete;
    JniVoidCallback& operator=(const JniVoidCallback&) = delete;

    void invoke(const std::string& methodName, const std::string& methodSig) const;

private:
    JNIEnv* getEnv(bool& shouldDetach) const;

    JavaVM* m_jvm = nullptr;
    jobject m_globalObj;
};

} // karin::jni

#endif //LANGUAGES_KOTLIN_SRC_MAIN_CPP_JNI_CALLBACK_H
