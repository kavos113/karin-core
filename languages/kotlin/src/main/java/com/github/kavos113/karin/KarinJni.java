package com.github.kavos113.karin;

class KarinJni {
    static {
        System.loadLibrary("karin_languages_java");
    }

    static native long applicationCreate();
    static native long applicationCreateWindow(long appPtr, String title, int x, int y, int width, int height);
    static native void applicationRun(long appPtr);
    static native void applicationDestroy(long appPtr);

    static native void windowSetRootView(long windowPtr, long viewPtr);
    static native void windowDestroy(long windowPtr);

    static native void viewNodeSetClickListener(long nodePtr, ViewNode node);

    static native long containerNodeCreate();
    static native long containerNodeCreate(float width, float height);
    static native void containerNodeAddChild(long containerPtr, long childPtr);
    static native void containerNodeSetLayoutDirection(long containerPtr, int direction);
    static native void containerNodeSetGap(long containerPtr, float gap);
    static native void containerNodeSetWrapMode(long containerPtr, int wrapMode);

    static native long rectangleNodeCreate(float width, float height, float r, float g, float b, float a);
}
