#include "com_github_kavos113_karin_KarinJni.h"

#include <karin/gui.h>
#include <karin/common.h>
#include <memory>

#include "jni_global_ref.h"

using namespace karin::gui;
using namespace karin::jni;

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

    auto *windowPtr = new std::shared_ptr(window);

    return reinterpret_cast<jlong>(windowPtr);
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_KarinJni_applicationRun
    (JNIEnv *env, jclass cls, jlong appPtr)
{
    auto *app = reinterpret_cast<Application *>(appPtr);
    app->run();
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_KarinJni_applicationDestroy
    (JNIEnv *env, jclass cls, jlong appPtr)
{
    auto *app = reinterpret_cast<Application *>(appPtr);
    delete app;
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_KarinJni_windowSetRootView
    (JNIEnv *env, jclass cls, jlong windowPtr, jlong viewPtr)
{
    auto *window = reinterpret_cast<std::shared_ptr<Window> *>(windowPtr);
    auto *view = reinterpret_cast<ViewNode *>(viewPtr);

    (*window)->setRootView(std::unique_ptr<ViewNode>(view));
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_KarinJni_windowDestroy
    (JNIEnv *env, jclass cls, jlong windowPtr)
{
    auto *window = reinterpret_cast<std::shared_ptr<Window> *>(windowPtr);
    delete window;
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_KarinJni_viewNodeSetClickListener
    (JNIEnv *env, jclass cls, jlong viewPtr, jobject listener)
{
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    auto callback = std::make_shared<JniGlobalRef>(env, listener);

    node->setOnClick(
        [callback](karin::Point point)
        {
            callback->invoke(
                [](JNIEnv* env, jobject obj)
                {
                    jclass listenerClass = env->GetObjectClass(obj);
                    jmethodID methodId = env->GetMethodID(listenerClass, "dispatchClickEvent", "()V");
                    if (methodId)
                    {
                        env->CallVoidMethod(obj, methodId);
                    }

                    env->DeleteLocalRef(listenerClass);
                }
            );
        }
    );
}

JNIEXPORT jlong JNICALL Java_com_github_kavos113_karin_KarinJni_containerNodeCreate__
    (JNIEnv *, jclass)
{
    auto *container = new ContainerNode();
    return reinterpret_cast<jlong>(container);
}

JNIEXPORT jlong JNICALL Java_com_github_kavos113_karin_KarinJni_containerNodeCreate__FF
    (JNIEnv *env, jclass cls, jfloat width, jfloat height)
{
    karin::Size size(width, height);
    auto *container = new ContainerNode(size);
    return reinterpret_cast<jlong>(container);
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_KarinJni_containerNodeAddChild
    (JNIEnv *env, jclass cls, jlong containerPtr, jlong childPtr)
{
    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);
    auto *child = reinterpret_cast<ViewNode *>(childPtr);
    container->addChild(std::unique_ptr<ViewNode>(child));
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_KarinJni_containerNodeSetLayoutDirection
    (JNIEnv *env, jclass cls, jlong containerPtr, jint direction)
{
    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);
    container->setLayoutDirection(static_cast<ContainerNode::LayoutDirection>(direction));
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_KarinJni_containerNodeSetGap
    (JNIEnv *env, jclass cls, jlong containerPtr, jfloat gap)
{
    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);
    container->setGap(gap);
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_KarinJni_containerNodeSetWrapMode
    (JNIEnv *env, jclass cls, jlong containerPtr, jint mode)
{
    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);
    container->setWrapMode(static_cast<ContainerNode::WrapMode>(mode));
}

JNIEXPORT jlong JNICALL Java_com_github_kavos113_karin_KarinJni_rectangleNodeCreate
    (JNIEnv *env, jclass cls, jfloat width, jfloat height, jfloat r, jfloat g, jfloat b, jfloat a)
{
    karin::Size size(width, height);
    karin::Color color(r, g, b, a);
    auto *rectangle = new RectangleNode(size, color);
    return reinterpret_cast<jlong>(rectangle);
}