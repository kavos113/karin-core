#include "rectangle_node.h"

namespace karin::gui
{
void RectangleNode::drawInternal(GraphicsContext& gc, const Transform2D& parentTransform) const
{
    Rectangle layout = getLayout();
    Pattern pattern = SolidColorPattern(m_color);

    gc.fillRect(layout, pattern, parentTransform);
}
} // karin::gui