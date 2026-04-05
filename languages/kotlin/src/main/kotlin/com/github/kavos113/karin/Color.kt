package com.github.kavos113.karin

data class Color(
    val r: Float,
    val g: Float,
    val b: Float,
    val a: Float
) {
    constructor(r: Int, g: Int, b: Int, a: Int = 255) : this(
        r / 255f,
        g / 255f,
        b / 255f,
        a / 255f
    )

    constructor(rgba: Int) : this(
        (rgba shr 16) and 0xFF,
        (rgba shr 8) and 0xFF,
        rgba and 0xFF,
        (rgba shr 24) and 0xFF
    )

    companion object {
        val Black = Color(0, 0, 0)
        val White = Color(255, 255, 255)
        val Red = Color(255, 0, 0)
        val Green = Color(0, 255, 0)
        val Blue = Color(0, 0, 255)
        val Transparent = Color(0, 0, 0, 0)
    }
}
