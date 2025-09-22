#ifndef KARIN_COMMON_GEOMETRY_TRANSFORM2D_H
#define KARIN_COMMON_GEOMETRY_TRANSFORM2D_H

#include <memory>

namespace karin
{
class Transform2DImpl;

class Transform2D
{
public:
    Transform2D();
    ~Transform2D();

    Transform2D& translate(float tx, float ty);

    // radians. counter-clockwise
    Transform2D& rotate(float angle);

    Transform2D& scale(float sx, float sy);

    const float* data() const;

private:
    std::unique_ptr<Transform2DImpl> m_impl;
};
} // karin

#endif //KARIN_COMMON_GEOMETRY_TRANSFORM2D_H
