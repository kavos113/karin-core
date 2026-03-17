#include "com_github_kavos113_karin_KarinJni.h"

#include <karin/gui.h>

using namespace karin::gui;

JNIEXPORT jlong JNICALL Java_com_github_kavos113_karin_KarinJni_applicationCreate
    (JNIEnv *env, jclass cls)
{
    auto *app = new Application();
    return reinterpret_cast<jlong>(app);
}

JNIEXPORT jlong JNICALL Java_com_github_kavos113_karin_KarinJni_applicationCreateWindow
    (JNIEnv *env, jclass cls, jlong appPtr, jstring title, jint x, jint y, jint width, jint height)
{
    auto *app = reinterpret_cast<Application *>(appPtr);

    const char *titleChars = env->GetStringUTFChars(title, nullptr);
    auto window = app->createWindow(titleChars, x, y, width, height);
    env->ReleaseStringUTFChars(title, titleChars);

    return reinterpret_cast<jlong>(window.get());
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_KarinJni_applicationRun
    (JNIEnv *env, jclass cls, jlong appPtr)
{
    auto *app = reinterpret_cast<Application *>(appPtr);
    app->run();
}