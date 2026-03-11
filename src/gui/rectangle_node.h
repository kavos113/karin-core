#ifndef SRC_GUI_RECTANGLE_NODE_H
#define SRC_GUI_RECTANGLE_NODE_H

#include "view_node.h"
#include "container_node.h"

namespace karin::gui
{
class RectangleNode : public ContainerNode
{
public:
    RectangleNode(Size size, Color color) : ContainerNode(size), m_color(color) {}
    ~RectangleNode() override = default;

    void drawInternal(GraphicsContext& gc, const Transform2D& parentTransform) const override;

private:
    Color m_color;
};
} // karin::gui

#endif //SRC_GUI_RECTANGLE_NODE_H