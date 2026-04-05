package com.github.kavos113.karin

data class ParagraphStyle(
    val horizontalAlignment: HorizontalAlignment = HorizontalAlignment.Left,
    val verticalAlignment: VerticalAlignment = VerticalAlignment.Top,
    val trimMode: TrimMode = TrimMode.None,
    val wrapMode: WrapMode = WrapMode.None,
    val flowDirection: Direction = Direction.TopToBottom,
    val readingDirection: Direction = Direction.LeftToRight,
    val lineSpacingMode: LineSpacingMode = LineSpacingMode.Proportional,
    val lineSpacingValue: Float = 1f,
    val baseLineOffset: Float = 0f
)

enum class HorizontalAlignment {
    Left,
    Center,
    Right,
    Justify;

    fun toInt(): Int {
        return when (this) {
            Left -> 0
            Center -> 2
            Right -> 1
            Justify -> 3
        }
    }
}

enum class VerticalAlignment {
    Top,
    Center,
    Bottom;

    fun toInt(): Int {
        return when (this) {
            Top -> 0
            Center -> 1
            Bottom -> 2
        }
    }
}

enum class TrimMode {
    None,
    Character,
    Word;

    fun toInt(): Int {
        return when (this) {
            None -> 0
            Character -> 2
            Word -> 1
        }
    }
}

enum class WrapMode {
    None,
    Character,
    Word;

    fun toInt(): Int {
        return when (this) {
            None -> 0
            Character -> 2
            Word -> 1
        }
    }
}

enum class Direction {
    LeftToRight,
    RightToLeft,
    TopToBottom,
    BottomToTop;

    fun toInt(): Int {
        return when (this) {
            LeftToRight -> 2
            RightToLeft -> 3
            TopToBottom -> 0
            BottomToTop -> 1
        }
    }
}

enum class LineSpacingMode {
    Proportional,
    Fixed;

    fun toInt(): Int {
        return when (this) {
            Proportional -> 2
            Fixed -> 1
        }
    }
}