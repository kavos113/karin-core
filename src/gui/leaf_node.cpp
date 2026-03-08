#include "leaf_node.h"

namespace karin::gui
{
LeafNode::LeafNode()
{
    YGNodeSetMeasureFunc(m_yogaNode, reinterpret_cast<YGMeasureFunc>(staticMeasureFunc));
}

YGSize LeafNode::staticMeasureFunc(
    YGNodeRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
{
    auto *self = static_cast<LeafNode*>(YGNodeGetContext(node));

    if (self)
    {
        return self->measure({width, height});
    }

    return {0, 0};
}
} // karin::gui