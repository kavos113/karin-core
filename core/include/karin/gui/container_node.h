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
        Row = 0,
        Column = 1
    };

    enum class WrapMode
    {
        No = 0,
        Wrap = 1,
        WrapReverse = 2
    };

    ContainerNode() = default;
    explicit ContainerNode(Size size) : ViewNode(size) {}
    ~ContainerNode() override = default;

    void drawInternal(GraphicsContext& gc, const Transform2D& parentTransform) const final;

    virtual void drawBackground(GraphicsContext& gc, const Transform2D& parentTransform) const {}
    virtual void drawForeground(GraphicsContext& gc, const Transform2D& parentTransform) const {}

    void addChild(std::unique_ptr<ViewNode> child);
    void setLayoutDirection(LayoutDirection direction);
    void setGap(float gap);
    void setWrapMode(WrapMode mode);

    const ViewNode* hitTest(const Point& point) const override;

protected:
    std::vector<std::unique_ptr<ViewNode>> m_children;
};
} // karin::gui

#endif //SRC_GUI_CONTAINER_NODE_H