package com.github.kavos113.karin

class Application : AutoCloseable {
    internal var nativePtr: Long = KarinJni.applicationCreate()

    fun createWindow(title: String, x: Int, y: Int, width: Int, height: Int): Window {
        val windowPtr = KarinJni.applicationCreateWindow(nativePtr, title, x, y, width, height)
        return Window(windowPtr)
    }

    fun run() {
        KarinJni.applicationRun(nativePtr)
    }

    override fun close() {
        if (nativePtr != 0L) {
            KarinJni.applicationDestroy(nativePtr)
            nativePtr = 0L
        }
    }
}