#include <karin/common/geometry/transform2d.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

Transform2D& Transform2D::rotate(float angle)
{
    m_impl->mat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * m_impl->mat;
    return *this;
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
}
