#include "text_node.h"

namespace karin::gui
{
TextNode::TextNode(TextLayout layout)
    : m_layout(std::move(layout))
{
}

void TextNode::drawInternal(GraphicsContext& gc, const Transform2D& parentTransform) const
{
}

YGSize TextNode::measure(Size availableSize) const
{

}
} // karin::gui