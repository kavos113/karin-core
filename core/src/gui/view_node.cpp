#include <karin/gui/view_node.h>

#include <karin/graphics/stroke_style.h>

namespace
{
using namespace karin::gui;
using namespace karin;

YGEdge toYogaEdge(ViewNode::Side side)
{
    switch (side)
    {
    case ViewNode::Side::Left:
        return YGEdgeLeft;
    case ViewNode::Side::Top:
        return YGEdgeTop;
    case ViewNode::Side::Right:
        return YGEdgeRight;
    case ViewNode::Side::Bottom:
        return YGEdgeBottom;
    case ViewNode::Side::Horizontal:
        return YGEdgeHorizontal;
    case ViewNode::Side::Vertical:
        return YGEdgeVertical;
    case ViewNode::Side::All:
        return YGEdgeAll;
    default:
        return YGEdgeAll; // Default to All if unknown
    }
}

StrokeStyle toStrokeStyle(NodeBorder::LineStyle style, float width)
{
    StrokeStyle strokeStyle;
    strokeStyle.width = width;

    switch (style)
    {
    case NodeBorder::LineStyle::None:
        strokeStyle.dash_pattern.clear();
        break;
    case NodeBorder::LineStyle::Solid:
        strokeStyle.dash_pattern.clear();
        break;
    case NodeBorder::LineStyle::Dashed:
        strokeStyle.dash_pattern = { 4.0f * width, 4.0f * width };
        break;
    case NodeBorder::LineStyle::Dotted:
        strokeStyle.dash_pattern = { width, width };
        strokeStyle.start_cap_style = StrokeStyle::CapStyle::Round;
        strokeStyle.end_cap_style = StrokeStyle::CapStyle::Round;
        strokeStyle.dash_cap_style = StrokeStyle::CapStyle::Round;
        break;
    }

    return strokeStyle;
}
}

namespace karin::gui
{
ViewNode::ViewNode()
{
    m_yogaNode = YGNodeNew();

    m_borders.fill({ 0.0f, Color(), NodeBorder::LineStyle::None });
}

ViewNode::ViewNode(Size size)
{
    m_yogaNode = YGNodeNew();
    YGNodeStyleSetWidth(m_yogaNode, size.width);
    YGNodeStyleSetHeight(m_yogaNode, size.height);

    m_borders.fill({ 0.0f, Color(), NodeBorder::LineStyle::None });
}

ViewNode::~ViewNode()
{
    YGNodeFree(m_yogaNode);
}

void ViewNode::draw(GraphicsContext& gc, const Transform2D& parentTransform) const
{
    drawInternal(gc, parentTransform);
    drawBorder(gc, parentTransform);
}

void ViewNode::calculateLayout() const
{
    YGNodeCalculateLayout(m_yogaNode, YGUndefined, YGUndefined, YGDirectionLTR);
}

Rectangle ViewNode::getLayout() const
{
    return {
        YGNodeLayoutGetLeft(m_yogaNode),
        YGNodeLayoutGetTop(m_yogaNode),
        YGNodeLayoutGetWidth(m_yogaNode),
        YGNodeLayoutGetHeight(m_yogaNode)
    };
}

void ViewNode::setSize(Size size)
{
    YGNodeStyleSetWidth(m_yogaNode, size.width);
    YGNodeStyleSetHeight(m_yogaNode, size.height);
}

void ViewNode::setWidth(float width)
{
    YGNodeStyleSetWidth(m_yogaNode, width);
}

void ViewNode::setHeight(float height)
{
    YGNodeStyleSetHeight(m_yogaNode, height);
}

void ViewNode::setMargin(Side side, float margin)
{
    YGNodeStyleSetMargin(m_yogaNode, toYogaEdge(side), margin);
}

void ViewNode::setPadding(Side side, float padding)
{
    YGNodeStyleSetPadding(m_yogaNode, toYogaEdge(side), padding);
}

void ViewNode::setBorder(Side side, float width, Color color, NodeBorder::LineStyle style)
{
    YGNodeStyleSetBorder(m_yogaNode, toYogaEdge(side), width);

    switch (side)
    {
    case Side::Left:
        m_borders[0] = { width, color, style };
        break;
    case Side::Top:
        m_borders[1] = { width, color, style };
        break;
    case Side::Right:
        m_borders[2] = { width, color, style };
        break;
    case Side::Bottom:
        m_borders[3] = { width, color, style };
        break;
    case Side::Horizontal:
        m_borders[0] = { width, color, style };
        m_borders[2] = { width, color, style };
        break;
    case Side::Vertical:
        m_borders[1] = { width, color, style };
        m_borders[3] = { width, color, style };
        break;
    case Side::All:
        for (auto& border : m_borders)
        {
            border = { width, color, style };
        }
        break;
    }
}

YGNodeRef ViewNode::getYogaNode() const
{
    return m_yogaNode;
}

void ViewNode::setOnClick(std::function<void(Point point)> onClick)
{
    m_onClick = std::move(onClick);
}

void ViewNode::triggerClick(Point point) const
{
    if (m_onClick)
    {
        m_onClick(point);
    }
}

void ViewNode::drawBorder(GraphicsContext& gc, const Transform2D& transform) const
{
    Rectangle layout = getLayout();

    Point topLeft = { layout.pos.x, layout.pos.y };
    Point topRight = { layout.pos.x + layout.size.width, layout.pos.y };
    Point bottomLeft = { layout.pos.x, layout.pos.y + layout.size.height };
    Point bottomRight = { layout.pos.x + layout.size.width, layout.pos.y + layout.size.height };

    if (m_borders[0].style != NodeBorder::LineStyle::None)
    {
        Pattern pattern = SolidColorPattern(m_borders[0].color);
        gc.drawLine(
            topLeft,
            bottomLeft,
            pattern,
            toStrokeStyle(m_borders[0].style, m_borders[0].width),
            transform
        );
    }

    if (m_borders[1].style != NodeBorder::LineStyle::None)
    {
        Pattern pattern = SolidColorPattern(m_borders[1].color);
        gc.drawLine(
            topLeft,
            topRight,
            pattern,
            toStrokeStyle(m_borders[1].style, m_borders[1].width),
            transform
        );
    }

    if (m_borders[2].style != NodeBorder::LineStyle::None)
    {
        Pattern pattern = SolidColorPattern(m_borders[2].color);
        gc.drawLine(
            topRight,
            bottomRight,
            pattern,
            toStrokeStyle(m_borders[2].style, m_borders[2].width),
            transform
        );
    }

    if (m_borders[3].style != NodeBorder::LineStyle::None)
    {
        Pattern pattern = SolidColorPattern(m_borders[3].color);
        gc.drawLine(
            bottomLeft,
            bottomRight,
            pattern,
            toStrokeStyle(m_borders[3].style, m_borders[3].width),
            transform
        );
    }
}

ViewNode* ViewNode::hitTest(const Point& point)
{
    float width = YGNodeLayoutGetWidth(m_yogaNode);
    float height = YGNodeLayoutGetHeight(m_yogaNode);

    if (point.x < 0 || point.y < 0 || point.x > width || point.y > height)
    {
        return nullptr;
    }

    return this;
}
} // karin