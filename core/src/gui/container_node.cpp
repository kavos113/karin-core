#include <karin/gui/container_node.h>

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

YGWrap toYogaWrap(karin::gui::ContainerNode::WrapMode mode)
{
    switch (mode)
    {
    case karin::gui::ContainerNode::WrapMode::No:
        return YGWrapNoWrap;
    case karin::gui::ContainerNode::WrapMode::Wrap:
        return YGWrapWrap;
    case karin::gui::ContainerNode::WrapMode::WrapReverse:
        return YGWrapWrapReverse;
    default:
        return YGWrapNoWrap; // Default to No Wrap if unknown
    }
}
}

namespace karin::gui
{
void ContainerNode::drawInternal(GraphicsContext& gc, const Transform2D& parentTransform) const
{
    drawBackground(gc, parentTransform);

    Rectangle layout = getLayout();
    Transform2D transform = parentTransform;
    transform.translate(layout.pos.x, layout.pos.y);
    for (const auto& child : m_children)
    {
        child->draw(gc, transform);
    }

    drawForeground(gc, parentTransform);
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

void ContainerNode::setWrapMode(WrapMode mode)
{
    YGNodeStyleSetFlexWrap(m_yogaNode, toYogaWrap(mode));
}

ViewNode* ContainerNode::hitTest(const Point& point)
{
    float width = YGNodeLayoutGetWidth(m_yogaNode);
    float height = YGNodeLayoutGetHeight(m_yogaNode);

    if (point.x < 0 || point.y < 0 || point.x > width || point.y > height)
    {
        return nullptr;
    }

    for (const auto & child : m_children)
    {
        float childX = YGNodeLayoutGetLeft(child->getYogaNode());
        float childY = YGNodeLayoutGetTop(child->getYogaNode());
        Point childPoint = { point.x - childX, point.y - childY };

        const ViewNode* hitNode = child->hitTest(childPoint);
        if (hitNode)
        {
            return hitNode;
        }
    }

    return this;
}
} // karin::gui