#ifndef SRC_GUI_VIEW_NODE_H
#define SRC_GUI_VIEW_NODE_H

#include <karin/common/geometry/size.h>
#include <karin/common/geometry/rectangle.h>
#include <karin/graphics/graphics_context.h>

#include <yoga/Yoga.h>
#include <array>

namespace karin::gui
{
struct NodeBorder
{
    enum class LineStyle
    {
        None,
        Solid,
        Dashed,
        Dotted
    };

    float width;
    Color color;
    LineStyle style;
};

class ViewNode
{
public:
    enum class Side
    {
        Left,
        Top,
        Right,
        Bottom,
        Horizontal,
        Vertical,
        All
    };

    ViewNode();
    explicit ViewNode(Size size);
    virtual ~ViewNode();

    void draw(GraphicsContext& gc, const Transform2D& parentTransform) const;
    virtual const ViewNode* hitTest(const Point& point) const;

    void calculateLayout() const;
    Rectangle getLayout() const;

    void setSize(Size size);
    void setWidth(float width);
    void setHeight(float height);
    void setMargin(Side side, float margin);
    void setPadding(Side side, float padding);
    void setBorder(Side side, float width, Color color, NodeBorder::LineStyle style);

    YGNodeRef getYogaNode() const;

protected:
    virtual void drawInternal(GraphicsContext& gc, const Transform2D& parentTransform) const = 0;

    YGNodeRef m_yogaNode;

private:
    void drawBorder(GraphicsContext& gc, const Transform2D& transform) const;

    std::array<NodeBorder, 4> m_borders;
};
} // karin

#endif //SRC_GUI_VIEW_NODE_H