#ifndef SRC_GUI_TEXT_NODE_H
#define SRC_GUI_TEXT_NODE_H

#include <karin/graphics/text_layout.h>

#include "leaf_node.h"

namespace karin::gui
{
class TextNode : public LeafNode
{
public:
    explicit TextNode(TextLayout layout);
    ~TextNode() override = default;

    void drawInternal(GraphicsContext& gc, const Transform2D& parentTransform) const override;
    YGSize measure(Size availableSize) const override;

private:
    TextLayout m_layout;
};
} // karin::gui

#endif //SRC_GUI_TEXT_NODE_H