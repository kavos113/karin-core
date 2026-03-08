#ifndef SRC_GUI_LEAF_NODE_H
#define SRC_GUI_LEAF_NODE_H

#include "view_node.h"

#include <yoga/Yoga.h>

namespace karin::gui
{
class LeafNode : public ViewNode
{
public:
    LeafNode();
    explicit LeafNode(Size size) : ViewNode(size) {}
    ~LeafNode() override = default;

    void draw(GraphicsContext& gc, const Transform2D& parentTransform) const override = 0;

protected:
    virtual YGSize measure(Size availableSize) const = 0;

private:
    static YGSize staticMeasureFunc(YGNodeRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
};
} // karin::gui

#endif //SRC_GUI_LEAF_NODE_H