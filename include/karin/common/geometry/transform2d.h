#ifndef KARIN_COMMON_GEOMETRY_TRANSFORM2D_H
#define KARIN_COMMON_GEOMETRY_TRANSFORM2D_H

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "point.h"

namespace karin
{
class Transform2DImpl;

class Transform2D
{
public:
    Transform2D() = default;
    ~Transform2D() = default;

    Transform2D& translate(float tx, float ty)
    {
        m_translation.x += tx;
        m_translation.y += ty;
        return *this;
    }
    Transform2D& setTranslation(float tx, float ty)
    {
        m_translation.x = tx;
        m_translation.y = ty;
        return *this;
    }

    // radians. counter-clockwise
    Transform2D& rotate(float radian)
    {
        m_rotation += radian;
        return *this;
    }
    Transform2D& rotateDeg(float degree)
    {
        return rotate(glm::radians(degree));
    }
    Transform2D& setRotation(float radian)
    {
        m_rotation = radian;
        return *this;
    }
    Transform2D& setRotationDeg(float degree)
    {
        return setRotation(glm::radians(degree));
    }

    Transform2D& scale(float sx, float sy)
    {
        m_scale.x *= sx;
        m_scale.y *= sy;
        return *this;
    }
    Transform2D& setScale(float sx, float sy)
    {
        m_scale.x = sx;
        m_scale.y = sy;
        return *this;
    }

    glm::mat4 toMatrix() const
    {
        return glm::scale(glm::mat4(1.0f), glm::vec3(m_scale.x, m_scale.y, 1.0f))
            * glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::translate(glm::mat4(1.0f), glm::vec3(m_translation.x, m_translation.y, 0.0f));
    }

private:
    Point m_translation{0.0f, 0.0f};
    float m_rotation = 0.0f; // radians
    Point m_scale{1.0f, 1.0f};
};

std::ostream& operator<<(std::ostream& os, const Transform2D& transform)
{
    glm::mat4 mat = transform.toMatrix();
    const float* d = &mat[0][0];
    return os << "Transform Matrix:\n"
       << d[0] << " " << d[4] << " " << d[8]  << " " << d[12] << "\n"
       << d[1] << " " << d[5] << " " << d[9]  << " " << d[13] << "\n"
       << d[2] << " " << d[6] << " " << d[10] << " " << d[14] << "\n"
       << d[3] << " " << d[7] << " " << d[11] << " " << d[15] << "\n";
}
} // karin

#endif //KARIN_COMMON_GEOMETRY_TRANSFORM2D_H
