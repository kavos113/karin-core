package com.github.kavos113.karin

internal class RectangleNode(ptr: Long) : ViewNode(ptr) {

    constructor(size: Size, color: Color)
        : this(KarinJni.rectangleNodeCreate(size.width, size.height, color.r, color.g, color.b, color.a))
}