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
} // karin