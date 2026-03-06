#include "container_node.h"

namespace karin::gui
{
void ContainerNode::draw(GraphicsContext& gc) const
{
    for (const auto& child : m_children)
    {
        child->draw(gc);
    }
}
} // karin::gui