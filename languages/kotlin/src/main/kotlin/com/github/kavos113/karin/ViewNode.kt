package com.github.kavos113.karin

abstract class ViewNode(internal var nativePtr: Long) {

    private var onClick: (() -> Unit)? = null

    fun setOnClickListener(listener: () -> Unit) {
        onClick = listener
        KarinJni.viewNodeSetClickListener(nativePtr, this)
    }

    @JvmName("dispatchClickEvent")
    internal fun dispatchClickEvent() {
        onClick?.invoke()
    }
}