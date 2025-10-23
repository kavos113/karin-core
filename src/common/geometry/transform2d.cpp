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

    // unintended to be called with (isColMajor == false) and (isColMajor == true) from same process.
    // results of below code:
    //     auto m1 = matrix(true);
    //     auto m2 = matrix(false);
    // m1 and m2 will be same(m2 overrides m1)
    const glm::mat4& matrix(bool isColMajor)
    {
        if (isColMajor)
        {
            m_matrix = glm::translate(
                glm::rotate(
                    glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f)),
                    rotation,
                    glm::vec3(0.0f, 0.0f, 1.0f)
                ),
                glm::vec3(translation.x, translation.y, 0.0f)
            );
            return m_matrix;
        }


        m_matrix = glm::scale(
            glm::rotate(
                glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, 0.0f)),
                rotation,
                glm::vec3(0.0f, 0.0f, 1.0f)
            ),
            glm::vec3(scale.x, scale.y, 1.0f)
        );
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
    return *this;
}

Transform2D& Transform2D::setTranslate(float tx, float ty)
{
    m_impl->translation.x = tx;
    m_impl->translation.y = ty;
    return *this;
}

Point Transform2D::getTranslate() const
{
    return m_impl->translation;
}

Transform2D& Transform2D::rotate(float radian)
{
    m_impl->rotation += radian;
    return *this;
}

Transform2D& Transform2D::rotateDeg(float degree)
{
    m_impl->rotation += glm::radians(degree);
    return *this;
}

Transform2D& Transform2D::setRotate(float radian)
{
    m_impl->rotation = radian;
    return *this;
}

Transform2D& Transform2D::setRotateDeg(float degree)
{
    m_impl->rotation = glm::radians(degree);
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
    return *this;
}

Transform2D& Transform2D::setScale(float sx, float sy)
{
    m_impl->scale.x = sx;
    m_impl->scale.y = sy;
    return *this;
}

Point Transform2D::getScale() const
{
    return m_impl->scale;
}

const float* Transform2D::colMajorData() const
{
    return &m_impl->matrix(true)[0][0];
}

const float* Transform2D::rowMajorData() const
{
    return &m_impl->matrix(false)[0][0];
}


std::ostream& operator<<(std::ostream& os, const Transform2D& transform)
{
    return os << "Transform2D(\n"
              << "  translation: (" << transform.getTranslate().x << ", " << transform.getTranslate().y << ")\n"
              << "  rotation (radians): " << transform.getRotate() << "\n"
              << "  scale: (" << transform.getScale().x << ", " << transform.getScale().y << ")\n"
              << ")";
}
}
