#ifndef SRC_COMMON_GEOMETRY_D2D_RECTANGLE_H
#define SRC_COMMON_GEOMETRY_D2D_RECTANGLE_H

#include <karin/common/geometry/rectangle.h>
#include <d2d1_1.h>

namespace karin
{

inline D2D1_RECT_F toD2DRect(const Rectangle& rect)
{
    return D2D1::RectF(
        rect.pos.x,
        rect.pos.y,
        rect.pos.x + rect.size.width,
        rect.pos.y + rect.size.height
    );
}

}

#endif //SRC_COMMON_GEOMETRY_D2D_RECTANGLE_H
