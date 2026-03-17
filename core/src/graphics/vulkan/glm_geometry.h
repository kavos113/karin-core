#ifndef SRC_COMMON_GEOMETRY_GLM_GEOMETRY_H
#define SRC_COMMON_GEOMETRY_GLM_GEOMETRY_H

#include <glm/glm.hpp>

#include <karin/common/geometry/point.h>
#include <karin/common/geometry/size.h>

namespace karin
{
inline glm::vec2 toGlmVec2(const Point& point)
{
    return glm::vec2(point.x, point.y);
}

inline glm::vec2 toGlmVec2(const Size& size)
{
    return glm::vec2(size.width, size.height);
}
}

#endif //SRC_COMMON_GEOMETRY_GLM_GEOMETRY_H