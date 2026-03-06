#ifndef SRC_GUI_VIEW_NODE_H
#define SRC_GUI_VIEW_NODE_H

#include <karin/common/geometry/size.h>
#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/graphics_context.h>

#include <yoga/Yoga.h>
#include <vector>
#include <memory>

namespace karin::gui
{
class ViewNode
{
public:
    ViewNode();
    explicit ViewNode(Size size);
    virtual ~ViewNode();

    virtual void draw(GraphicsContext& gc) const = 0;

    void addChild(std::unique_ptr<ViewNode> child);
    void calculateLayout() const;
    Rectangle getLayout() const;

private:
    YGNodeRef m_yogaNode;
    std::vector<std::unique_ptr<ViewNode>> m_children;
};
} // karin

#endif //SRC_GUI_VIEW_NODE_H