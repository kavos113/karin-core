#include "view_node.h"

namespace karin::gui
{
ViewNode::ViewNode()
{
    m_yogaNode = YGNodeNew();
}

ViewNode::ViewNode(Size size)
{
    m_yogaNode = YGNodeNew();
    YGNodeStyleSetWidth(m_yogaNode, size.width);
    YGNodeStyleSetHeight(m_yogaNode, size.height);
}

ViewNode::~ViewNode()
{
    YGNodeFree(m_yogaNode);
}

void ViewNode::calculateLayout() const
{
    YGNodeCalculateLayout(m_yogaNode, YGUndefined, YGUndefined, YGDirectionLTR);
}

Rectangle ViewNode::getLayout() const
{
    return {
        YGNodeLayoutGetLeft(m_yogaNode),
        YGNodeLayoutGetTop(m_yogaNode),
        YGNodeLayoutGetWidth(m_yogaNode),
        YGNodeLayoutGetHeight(m_yogaNode)
    };
}

void ViewNode::setSize(Size size)
{
    YGNodeStyleSetWidth(m_yogaNode, size.width);
    YGNodeStyleSetHeight(m_yogaNode, size.height);
}

void ViewNode::setWidth(float width)
{
    YGNodeStyleSetWidth(m_yogaNode, width);
}

void ViewNode::setHeight(float height)
{
    YGNodeStyleSetHeight(m_yogaNode, height);
}

YGNodeRef ViewNode::getYogaNode() const
{
    return m_yogaNode;
}
} // karin