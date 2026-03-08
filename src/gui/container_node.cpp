#include "container_node.h"

namespace
{
YGFlexDirection toYogaFlexDirection(karin::gui::ContainerNode::LayoutDirection direction)
{
    switch (direction)
    {
    case karin::gui::ContainerNode::LayoutDirection::Row:
        return YGFlexDirectionRow;
    case karin::gui::ContainerNode::LayoutDirection::Column:
        return YGFlexDirectionColumn;
    default:
        return YGFlexDirectionRow; // Default to Row if unknown
    }
}
}

namespace karin::gui
{
void ContainerNode::draw(GraphicsContext& gc, const Transform2D& parentTransform) const
{
    for (const auto& child : m_children)
    {
        child->draw(gc, parentTransform);
    }
}

void ContainerNode::addChild(std::unique_ptr<ViewNode> child)
{
    YGNodeInsertChild(m_yogaNode, child->getYogaNode(), YGNodeGetChildCount(m_yogaNode));

    m_children.push_back(std::move(child));
}

void ContainerNode::setLayoutDirection(LayoutDirection direction)
{
    YGNodeStyleSetFlexDirection(m_yogaNode,  toYogaFlexDirection(direction));
}

void ContainerNode::setGap(float gap)
{
    YGNodeStyleSetGap(m_yogaNode, YGGutterAll, gap);
}
} // karin::gui