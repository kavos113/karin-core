package com.github.kavos113.karin

internal class Window(var nativePtr: Long) {

    fun setRootView(view: ViewNode) {
        KarinJni.windowSetRootView(nativePtr, view.nativePtr)
    }
}