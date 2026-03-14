#ifndef SRC_GUI_TEXT_NODE_H
#define SRC_GUI_TEXT_NODE_H

#include <karin/graphics/text_style.h>
#include <karin/graphics/paragraph_style.h>
#include <karin/graphics/pattern.h>

#include "leaf_node.h"

#include <string>

namespace karin::gui
{
class TextNode : public LeafNode
{
public:
    explicit TextNode(
        std::string text,
        TextStyle textStyle,
        ParagraphStyle paragraphStyle,
        Pattern pattern
    );
    ~TextNode() override = default;

    void drawInternal(GraphicsContext& gc, const Transform2D& parentTransform) const override;
    YGSize measure(Size availableSize) const override;

private:
    std::string m_text;
    TextStyle m_textStyle;
    ParagraphStyle m_paragraphStyle;
    Pattern m_pattern;
};
} // karin::gui

#endif //SRC_GUI_TEXT_NODE_H