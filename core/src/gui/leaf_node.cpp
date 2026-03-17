#include <karin/gui/leaf_node.h>

#include <limits>

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

    float availableWidth = (widthMode == YGMeasureModeUndefined) ? std::numeric_limits<float>::infinity() : width;
    float availableHeight = (heightMode == YGMeasureModeUndefined) ? std::numeric_limits<float>::infinity() : height;

    if (self)
    {
        return self->measure({availableWidth, availableHeight});
    }

    return {0, 0};
}
} // karin::gui