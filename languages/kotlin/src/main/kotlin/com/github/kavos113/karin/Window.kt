package com.github.kavos113.karin

class Window internal constructor(
    internal var nativePtr: Long
) : AutoCloseable {

    // 子ノードとして追加されたviewの所有権はC++側で持つ
    fun setRootView(view: ViewNode) {
        require(view.nativePtr != 0L) { "ViewNode has been destroyed or already add child of others" }

        KarinJni.windowSetRootView(nativePtr, view.nativePtr)
        view.nativePtr = 0L
    }

    override fun close() {
        if (nativePtr != 0L) {
            KarinJni.windowDestroy(nativePtr)
            nativePtr = 0L
        }
    }
}