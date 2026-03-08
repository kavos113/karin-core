#ifndef SRC_GUI_CONTAINER_NODE_H
#define SRC_GUI_CONTAINER_NODE_H

#include "view_node.h"

namespace karin::gui
{
class ContainerNode : public ViewNode
{
public:
    enum class LayoutDirection
    {
        Row,
        Column
    };

    ContainerNode() = default;
    explicit ContainerNode(Size size) : ViewNode(size) {}
    ~ContainerNode() override = default;

    void draw(GraphicsContext& gc, const Transform2D& parentTransform) const override;

    void addChild(std::unique_ptr<ViewNode> child);
    void setLayoutDirection(LayoutDirection direction);
    void setGap(float gap);

protected:
    std::vector<std::unique_ptr<ViewNode>> m_children;
};
} // karin::gui

#endif //SRC_GUI_CONTAINER_NODE_H