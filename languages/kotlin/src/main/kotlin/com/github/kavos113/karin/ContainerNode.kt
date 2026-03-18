package com.github.kavos113.karin

internal open class ContainerNode(ptr: Long) : ViewNode(ptr) {

    constructor(): this(KarinJni.containerNodeCreate())
    constructor(size: Size) : this(KarinJni.containerNodeCreate(size.width, size.height))

    fun addChild(child: ViewNode) {
        KarinJni.containerNodeAddChild(nativePtr, child.nativePtr)
    }


}