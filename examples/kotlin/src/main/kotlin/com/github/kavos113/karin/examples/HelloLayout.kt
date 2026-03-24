package com.github.kavos113.karin.examples

import com.github.kavos113.karin.Application
import com.github.kavos113.karin.Color
import com.github.kavos113.karin.ContainerNode
import com.github.kavos113.karin.RectangleNode
import com.github.kavos113.karin.Size

fun randomColor(): Color {
    return Color(
        r = Math.random().toFloat(),
        g = Math.random().toFloat(),
        b = Math.random().toFloat(),
        a = 1f
    )
}

fun main() {
    val app = Application()
    val win = app.createWindow("Hello Layout", 100, 100, 800, 600)

    val root = ContainerNode(Size(800f, 600f))

    for (i in 0 until 10) {
        val child = RectangleNode(Size(100f, 100f), randomColor())
        root.addChild(child)
    }

    val bigRect = RectangleNode(Size(200f, 200f), randomColor())
    for (i in 0 until 5) {
        val child = RectangleNode(Size(50f, 50f), randomColor())
        bigRect.addChild(child)
    }
    bigRect.setLayoutDirection(ContainerNode.LayoutDirection.Column)
    bigRect.setGap(5.0f)
    bigRect.setWrapMode(ContainerNode.WrapMode.Wrap)
    root.addChild(bigRect)

    root.setLayoutDirection(ContainerNode.LayoutDirection.Row)
    root.setGap(10.0f)
    root.setWrapMode(ContainerNode.WrapMode.Wrap)

    win.setRootView(root)
    app.run()
}