package com.github.kavos113.karin

class Window(internal var nativePtr: Long) {

    fun setRootView(view: ViewNode) {
        KarinJni.windowSetRootView(nativePtr, view.nativePtr)
    }
}