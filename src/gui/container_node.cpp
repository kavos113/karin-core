#include "container_node.h"

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
} // karin::gui