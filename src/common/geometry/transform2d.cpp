#include <karin/common/geometry/transform2d.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <karin/common/geometry/point.h>

#include <iostream>

namespace karin
{
class Transform2DImpl
{
public:
    Point translation = Point(0.0f, 0.0f);
    float rotation = 0.0f; // in radians
    Point scale = Point(1.0f, 1.0f);

    bool m_dirty = true;

    const glm::mat4& matrix()
    {
        if (m_dirty)
        {
            m_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, 0.0f))
                * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f))
                * glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));
            m_dirty = false;
        }
        return m_matrix;
    }

private:
    glm::mat4 m_matrix = glm::mat4(1.0f);
};

Transform2D::Transform2D()
    : m_impl(std::make_unique<Transform2DImpl>())
{
}

Transform2D::~Transform2D() = default;

Transform2D& Transform2D::translate(float tx, float ty)
{
    m_impl->translation.x += tx;
    m_impl->translation.y += ty;
    m_impl->m_dirty = true;
    return *this;
}

Transform2D& Transform2D::setTranslate(float tx, float ty)
{
    m_impl->translation.x = tx;
    m_impl->translation.y = ty;
    m_impl->m_dirty = true;
    return *this;
}

Point Transform2D::getTranslate() const
{
    return m_impl->translation;
}

Transform2D& Transform2D::rotate(float radian)
{
    m_impl->rotation += radian;
    m_impl->m_dirty = true;
    return *this;
}

Transform2D& Transform2D::rotateDeg(float degree)
{
    m_impl->rotation += glm::radians(degree);
    m_impl->m_dirty = true;
    return *this;
}

Transform2D& Transform2D::setRotate(float radian)
{
    m_impl->rotation = radian;
    m_impl->m_dirty = true;
    return *this;
}

Transform2D& Transform2D::setRotateDeg(float degree)
{
    m_impl->rotation = glm::radians(degree);
    m_impl->m_dirty = true;
    return *this;
}

float Transform2D::getRotate() const
{
    return m_impl->rotation;
}

float Transform2D::getRotateDeg() const
{
    return glm::degrees(m_impl->rotation);
}

Transform2D& Transform2D::scale(float sx, float sy)
{
    m_impl->scale.x *= sx;
    m_impl->scale.y *= sy;
    m_impl->m_dirty = true;
    return *this;
}

Transform2D& Transform2D::setScale(float sx, float sy)
{
    m_impl->scale.x = sx;
    m_impl->scale.y = sy;
    m_impl->m_dirty = true;
    return *this;
}

Point Transform2D::getScale() const
{
    return m_impl->scale;
}

const float* Transform2D::data() const
{
    return &m_impl->matrix()[0][0];
}


std::ostream& operator<<(std::ostream& os, const Transform2D& transform)
{
    const float* d = transform.data();
    return os << "Transform Matrix:\n"
       << d[0] << " " << d[4] << " " << d[8]  << " " << d[12] << "\n"
       << d[1] << " " << d[5] << " " << d[9]  << " " << d[13] << "\n"
       << d[2] << " " << d[6] << " " << d[10] << " " << d[14] << "\n"
       << d[3] << " " << d[7] << " " << d[11] << " " << d[15] << "\n";
}
}
