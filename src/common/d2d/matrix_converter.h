#ifndef SRC_COMMON_D2D_MATRIX_CONVERTER_H
#define SRC_COMMON_D2D_MATRIX_CONVERTER_H

#include <d2d1.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <karin/common/geometry/transform2d.h>

namespace karin
{

D2D1_MATRIX_3X2_F toD2DMatrix(const Transform2D& transform)
{
    const float* data = transform.data();
    const glm::mat4 mat = glm::make_mat4(data);

    return D2D1::Matrix3x2F(
        mat[0][0], mat[1][0],
        mat[0][1], mat[1][1],
        mat[3][0], mat[3][1]
    );
}
}


#endif //SRC_COMMON_D2D_MATRIX_CONVERTER_H