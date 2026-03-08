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

    virtual void draw(GraphicsContext& gc, const Transform2D& parentTransform) const = 0;

    void calculateLayout() const;
    Rectangle getLayout() const;

    void setSize(Size size);
    void setWidth(float width);
    void setHeight(float height);

    YGNodeRef getYogaNode() const;

protected:
    YGNodeRef m_yogaNode;
};
} // karin

#endif //SRC_GUI_VIEW_NODE_H