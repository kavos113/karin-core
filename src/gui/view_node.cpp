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

void ViewNode::addChild(std::unique_ptr<ViewNode> child)
{
    YGNodeInsertChild(m_yogaNode, child->m_yogaNode, YGNodeGetChildCount(m_yogaNode));

    m_children.push_back(std::move(child));
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
} // karin