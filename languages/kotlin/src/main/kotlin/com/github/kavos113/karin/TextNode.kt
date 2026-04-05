package com.github.kavos113.karin

class TextNode internal constructor(ptr: Long) : ViewNode(ptr) {

    constructor(
        text: String = "",
        style: TextStyle = TextStyle(),
        paragraphStyle: ParagraphStyle = ParagraphStyle(),
        color: Color = Color.Black
    ) : this(
        KarinJni.textNodeCreate(
            text,
            style.fontFamily,
            style.fontStyle.toInt(),
            style.fontStretch.toInt(),
            style.fontWeight.toInt(),
            style.fontSize,
            style.locale,
            false, // TODO
            false,
            paragraphStyle.horizontalAlignment.toInt(),
            paragraphStyle.verticalAlignment.toInt(),
            paragraphStyle.lineSpacingValue,
            paragraphStyle.baseLineOffset,
            paragraphStyle.lineSpacingMode.toInt(),
            paragraphStyle.trimMode.toInt(),
            paragraphStyle.wrapMode.toInt(),
            paragraphStyle.flowDirection.toInt(),
            paragraphStyle.readingDirection.toInt(),
            color.r, color.g, color.b, color.a
        )
    )
}