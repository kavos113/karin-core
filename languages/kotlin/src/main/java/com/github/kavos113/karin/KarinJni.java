package com.github.kavos113.karin;

class KarinJni {
    static {
        System.loadLibrary("karin");
    }

    static native long applicationCreate();
    static native long applicationCreateWindow(long appPtr, String title, int x, int y, int width, int height);
    static native void applicationRun(long appPtr);
}
