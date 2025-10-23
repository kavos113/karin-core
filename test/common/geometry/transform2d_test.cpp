#include <karin/common/geometry/transform2d.h>

#include <gtest/gtest.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace karin;

class Transform2DTest : public testing::Test
{
protected:
    void SetUp() override {
        // 必要に応じてセットアップ
    }
};

TEST_F(Transform2DTest, InitialMatrixIsIdentity) {
    Transform2D t;
    glm::mat4 mat = t.toMatrix();
    const float* d = &mat[0][0];
    glm::mat4 expected(1.0f);
    for (int i = 0; i < 16; ++i) {
        EXPECT_FLOAT_EQ(d[i], (&expected[0][0])[i]);
    }
}

TEST_F(Transform2DTest, Translate) {
    Transform2D t;
    t.translate(1.0f, 2.0f);
    glm::mat4 mat = t.toMatrix();
    const float* d = &mat[0][0];
    glm::mat4 expected = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 0.0f));
    for (int i = 0; i < 16; ++i) {
        EXPECT_FLOAT_EQ(d[i], (&expected[0][0])[i]);
    }
}

TEST_F(Transform2DTest, Rotate) {
    Transform2D t;
    float angle = glm::radians(45.0f);
    t.rotate(angle);
    glm::mat4 mat = t.toMatrix();
    const float* d = &mat[0][0];
    glm::mat4 expected = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
    for (int i = 0; i < 16; ++i) {
        EXPECT_FLOAT_EQ(d[i], (&expected[0][0])[i]);
    }
}

TEST_F(Transform2DTest, Scale) {
    Transform2D t;
    t.scale(2.0f, 3.0f);
    glm::mat4 mat = t.toMatrix();
    const float* d = &mat[0][0];
    glm::mat4 expected = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 1.0f));
    for (int i = 0; i < 16; ++i) {
        EXPECT_FLOAT_EQ(d[i], (&expected[0][0])[i]);
    }
}

TEST_F(Transform2DTest, CombinedTransformations) {
    Transform2D t;
    t.translate(1.0f, 2.0f).rotate(glm::radians(90.0f)).scale(2.0f, 2.0f);
    glm::mat4 mat = t.toMatrix();
    const float* d = &mat[0][0];
    glm::mat4 expected = glm::translate(
        glm::scale(
            glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            glm::vec3(2.0f, 2.0f, 1.0f)
        ),
        glm::vec3(1.0f, 2.0f, 0.0f)
    );
    for (int i = 0; i < 16; ++i) {
        EXPECT_FLOAT_EQ(d[i], (&expected[0][0])[i]);
    }
}

TEST_F(Transform2DTest, CombinedTransformationsIsNoParticularOrder) {
    Transform2D t1;
    t1.translate(1.0f, 2.0f).rotate(glm::radians(90.0f)).scale(2.0f, 2.0f);
    glm::mat4 mat = t1.toMatrix();
    const float* d1 = &mat[0][0];

    Transform2D t2;
    t2.scale(2.0f, 2.0f).rotate(glm::radians(90.0f)).translate(1.0f, 2.0f);
    glm::mat4 mat2 = t2.toMatrix();
    const float* d2 = &mat[0][0];

    Transform2D t3;
    t3.rotate(glm::radians(90.0f)).scale(2.0f, 2.0f).translate(1.0f, 2.0f);
    glm::mat4 mat3 = t3.toMatrix();
    const float* d3 = &mat[0][0];

    for (int i = 0; i < 16; ++i) {
        EXPECT_FLOAT_EQ(d1[i], d2[i]);
        EXPECT_FLOAT_EQ(d1[i], d3[i]);
    }
}