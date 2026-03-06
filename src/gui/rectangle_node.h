#ifndef SRC_GUI_RECTANGLE_NODE_H
#define SRC_GUI_RECTANGLE_NODE_H

#include "view_node.h"

namespace karin::gui
{
class RectangleNode : public ViewNode
{
public:
    RectangleNode(Size size, Color color) : ViewNode(size), m_color(color) {}
    ~RectangleNode() override = default;

    void draw(GraphicsContext& gc, const Transform2D& parentTransform) const override;

private:
    Color m_color;
};
} // karin::gui

#endif //SRC_GUI_RECTANGLE_NODE_H