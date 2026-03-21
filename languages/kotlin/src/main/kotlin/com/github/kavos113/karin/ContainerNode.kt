package com.github.kavos113.karin

open class ContainerNode(ptr: Long) : ViewNode(ptr) {

    enum class LayoutDirection(val value: Int) {
        Row(0),
        Column(1)
    }

    enum class WrapMode(val value: Int) {
        NoWrap(0),
        Wrap(1),
        WrapReverse(2)
    }

    constructor(): this(KarinJni.containerNodeCreate())
    constructor(size: Size) : this(KarinJni.containerNodeCreate(size.width, size.height))

    fun addChild(child: ViewNode) {
        KarinJni.containerNodeAddChild(nativePtr, child.nativePtr)
    }

    fun setLayoutDirection(direction: LayoutDirection) {
        KarinJni.containerNodeSetLayoutDirection(nativePtr, direction.value)
    }

    fun setWrapMode(wrapMode: WrapMode) {
        KarinJni.containerNodeSetWrapMode(nativePtr, wrapMode.value)
    }

    fun setGap(gap: Float) {
        KarinJni.containerNodeSetGap(nativePtr, gap)
    }
}