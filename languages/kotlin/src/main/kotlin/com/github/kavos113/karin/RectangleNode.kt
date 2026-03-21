package com.github.kavos113.karin

class RectangleNode(ptr: Long) : ContainerNode(ptr) {

    constructor(size: Size, color: Color)
        : this(KarinJni.rectangleNodeCreate(size.width, size.height, color.r, color.g, color.b, color.a))
}