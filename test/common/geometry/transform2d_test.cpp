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
    const float* d = t.data();
    glm::mat4 expected(1.0f);
    for (int i = 0; i < 16; ++i) {
        EXPECT_FLOAT_EQ(d[i], (&expected[0][0])[i]);
    }
}

TEST_F(Transform2DTest, Translate) {
    Transform2D t;
    t.translate(1.0f, 2.0f);
    const float* d = t.data();
    glm::mat4 expected = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 0.0f));
    for (int i = 0; i < 16; ++i) {
        EXPECT_FLOAT_EQ(d[i], (&expected[0][0])[i]);
    }
}

TEST_F(Transform2DTest, Rotate) {
    Transform2D t;
    float angle = glm::radians(45.0f);
    t.rotate(angle);
    const float* d = t.data();
    glm::mat4 expected = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
    for (int i = 0; i < 16; ++i) {
        EXPECT_FLOAT_EQ(d[i], (&expected[0][0])[i]);
    }
}

TEST_F(Transform2DTest, Scale) {
    Transform2D t;
    t.scale(2.0f, 3.0f);
    const float* d = t.data();
    glm::mat4 expected = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 1.0f));
    for (int i = 0; i < 16; ++i) {
        EXPECT_FLOAT_EQ(d[i], (&expected[0][0])[i]);
    }
}

TEST_F(Transform2DTest, CombinedTransformations) {
    Transform2D t;
    t.translate(1.0f, 2.0f).rotate(glm::radians(90.0f)).scale(2.0f, 2.0f);
    const float* d = t.data();
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
    const float* d1 = t1.data();

    Transform2D t2;
    t2.scale(2.0f, 2.0f).rotate(glm::radians(90.0f)).translate(1.0f, 2.0f);
    const float* d2 = t2.data();

    Transform2D t3;
    t3.rotate(glm::radians(90.0f)).scale(2.0f, 2.0f).translate(1.0f, 2.0f);
    const float* d3 = t3.data();

    for (int i = 0; i < 16; ++i) {
        EXPECT_FLOAT_EQ(d1[i], d2[i]);
        EXPECT_FLOAT_EQ(d1[i], d3[i]);
    }
}