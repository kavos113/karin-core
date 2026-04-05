package com.github.kavos113.karin

data class TextStyle(
    val fontSize: Float = 16f,
    val fontFamily: String = "Arial",
    val fontWeight: FontWeight = FontWeight.Normal,
    val fontStyle: FontStyle = FontStyle.Normal,
    val fontStretch: FontStretch = FontStretch.Normal,
    val color: Color = Color.Black,
    val locale: String = "en-US"
)

enum class FontStyle {
    Normal,
    Italic,
    Oblique;

    fun toInt(): Int {
        return when (this) {
            Normal -> 0
            Italic -> 1
            Oblique -> 2
        }
    }
}

enum class FontStretch {
    UltraCondensed,
    ExtraCondensed,
    Condensed,
    SemiCondensed,
    Normal,
    SemiExpanded,
    Expanded,
    ExtraExpanded,
    UltraExpanded;

    fun toInt(): Int {
        return when (this) {
            UltraCondensed -> 0
            ExtraCondensed -> 1
            Condensed -> 2
            SemiCondensed -> 3
            Normal -> 4
            SemiExpanded -> 5
            Expanded -> 6
            ExtraExpanded -> 7
            UltraExpanded -> 8
        }
    }
}

enum class FontWeight {
    Thin,
    ExtraLight,
    Light,
    SemiLight,
    Normal,
    Medium,
    SemiBold,
    Bold,
    ExtraBold,
    Black,
    ExtraBlack;

    fun toInt(): Int {
        return when (this) {
            Thin -> 100
            ExtraLight -> 200
            Light -> 300
            SemiLight -> 350
            Normal -> 400
            Medium -> 500
            SemiBold -> 600
            Bold -> 700
            ExtraBold -> 800
            Black -> 900
            ExtraBlack -> 950
        }
    }
}
