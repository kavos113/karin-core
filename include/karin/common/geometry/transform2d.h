#ifndef KARIN_COMMON_GEOMETRY_TRANSFORM2D_H
#define KARIN_COMMON_GEOMETRY_TRANSFORM2D_H

#include "point.h"

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
    Transform2D& setTranslate(float tx, float ty);
    Point getTranslate() const;

    // radians. counter-clockwise
    Transform2D& rotate(float radian);
    Transform2D& rotateDeg(float degree);
    Transform2D& setRotate(float radian);
    Transform2D& setRotateDeg(float degree);
    float getRotate() const; // in radians
    float getRotateDeg() const; // in degrees

    Transform2D& scale(float sx, float sy);
    Transform2D& setScale(float sx, float sy);
    Point getScale() const;

    // column-major order. for OpenGL, Vulkan
    // translate * rotate * scale
    const float* colMajorData() const;
    // row-major order. for DirectX
    // scale * rotate * translate
    const float* rowMajorData() const;

private:
    std::unique_ptr<Transform2DImpl> m_impl;
};

std::ostream& operator<<(std::ostream& os, const Transform2D& transform);
} // karin

#endif //KARIN_COMMON_GEOMETRY_TRANSFORM2D_H
