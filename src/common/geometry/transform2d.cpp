#include <karin/common/geometry/transform2d.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace karin
{
class Transform2DImpl
{
public:
    glm::mat4 mat = glm::mat4(1.0f);
};

Transform2D::Transform2D()
    : m_impl(std::make_unique<Transform2DImpl>())
{
}

Transform2D::~Transform2D() = default;

Transform2D& Transform2D::translate(float tx, float ty)
{
    m_impl->mat = glm::translate(m_impl->mat, glm::vec3(tx, ty, 0.0f));
    return *this;
}

Transform2D& Transform2D::rotate(float radian)
{
    m_impl->mat = glm::rotate(glm::mat4(1.0f), radian, glm::vec3(0.0f, 0.0f, 1.0f)) * m_impl->mat;
    return *this;
}

Transform2D& Transform2D::rotateDeg(float degree)
{
    return rotate(glm::radians(degree));
}

Transform2D& Transform2D::scale(float sx, float sy)
{
    m_impl->mat = glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, 1.0f)) * m_impl->mat;
    return *this;
}

const float* Transform2D::data() const
{
    return &m_impl->mat[0][0];
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
