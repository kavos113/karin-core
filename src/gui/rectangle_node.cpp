#include "rectangle_node.h"

namespace karin::gui
{
void RectangleNode::draw(GraphicsContext& gc) const
{
    Rectangle layout = getLayout();
    Pattern pattern = SolidColorPattern(m_color);

    gc.drawRect(layout, pattern);
}
} // karin::gui