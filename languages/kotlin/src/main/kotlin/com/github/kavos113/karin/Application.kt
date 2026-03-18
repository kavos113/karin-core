package com.github.kavos113.karin

internal class Application {
    var nativePtr: Long = KarinJni.applicationCreate()

    fun createWindow(title: String, x: Int, y: Int, width: Int, height: Int): Window {
        val windowPtr = KarinJni.applicationCreateWindow(nativePtr, title, x, y, width, height)
        return Window(windowPtr)
    }

    fun run() {
        KarinJni.applicationRun(nativePtr)
    }
}