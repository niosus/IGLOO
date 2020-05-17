#include "gl/core/uniform.h"
#include "gl/ui/glfw/viewer.h"
#include "gl/utils/eigen_traits.h"
#include "utils/eigen_utils.h"
#include "gtest/gtest.h"

using namespace gl;

TEST(UniformDeathTest, InitWithoutProgram) {
  EXPECT_DEBUG_DEATH(
      Uniform("some_name", 0),
      "GL ERROR message: 'GL_INVALID_VALUE in glGetUniformLocation'");
}

TEST(UniformTest, Init) {
  Uniform uniform{"some_name", 0};
  EXPECT_EQ("some_name", uniform.name());
  EXPECT_NE(0u, uniform.id());
}

TEST(UniformTest, UpdateValueFromPack) {
  Uniform uniform{"some_name", 0};
  uniform.UpdateValue(1.0f);
  uniform.UpdateValue(1.0f, 2.0f);
  uniform.UpdateValue(1.0f, 2.0f, 3.0f);
  uniform.UpdateValue(1.0f, 2.0f, 3.0f, 4.0f);
}

TEST(UniformTest, UpdateValueFromSingleEigenMat) {
  Uniform uniform{"some_name", 0};
  uniform.UpdateValue(Eigen::Vector3f{1.0f, 2.0f, 3.0f});
}

TEST(UniformTest, UpdateValueFromPackOfIntegers) {
  Uniform uniform{"some_name", 0};
  uniform.UpdateValue(1);
  uniform.UpdateValue(1, 2);
  uniform.UpdateValue(1, 2, 3);
  uniform.UpdateValue(1, 2, 3, 4);
}

TEST(UniformTest, UpdateValueFromArray) {
  Uniform uniform{"some_name", 0};
  // 2 elements
  uniform.UpdateValue(eigen::vector<Eigen::Vector2f>{{1.0f, 2.0f}});
  uniform.UpdateValue(
      eigen::vector<Eigen::Vector2f>{{1.0f, 2.0f}, {2.0f, 3.0f}});
  // 3 elements
  uniform.UpdateValue(eigen::vector<Eigen::Vector3f>{{1.0f, 2.0f, 3.0f}});
  uniform.UpdateValue(
      eigen::vector<Eigen::Vector3f>{{1.0f, 2.0f, 3.0f}, {1.0f, 2.0f, 3.0f}});
  uniform.UpdateValue(eigen::vector<Eigen::Vector3f>(10, {1.0f, 2.0f, 3.0f}));
  // 4 elements
  uniform.UpdateValue(eigen::vector<Eigen::Vector4f>{{1.0f, 2.0f, 3.0f, 4.0f}});
  uniform.UpdateValue(
      eigen::vector<Eigen::Vector4f>(10, {1.0f, 2.0f, 3.0f, 4.0f}));
}

TEST(UniformTest, UpdateValueFromMatrix) {
  Uniform uniform{"some_name", 0};
  // 2x2
  Eigen::Matrix2f small_mat;
  small_mat << 1.0f, 2.0f, 3.0f, 4.0f;
  uniform.UpdateValue(eigen::vector<Eigen::Matrix2f>{small_mat});
  uniform.UpdateValue(eigen::vector<Eigen::Matrix2f>{small_mat, small_mat});
  // 3x3
  Eigen::Matrix3f some_matrix;
  some_matrix << 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f;
  uniform.UpdateValue(eigen::vector<Eigen::Matrix3f>{some_matrix});
  uniform.UpdateValue(eigen::vector<Eigen::Matrix3f>{some_matrix, some_matrix});
  uniform.UpdateValue(eigen::vector<Eigen::Matrix3f>(10, some_matrix));
  // 2x3
  using Mat2x3T = Eigen::Matrix<float, 2, 3>;
  Mat2x3T some_matrix_2_3;
  some_matrix_2_3 << 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f;
  uniform.UpdateValue(eigen::vector<Mat2x3T>{some_matrix_2_3});
  uniform.UpdateValue(eigen::vector<Mat2x3T>{some_matrix_2_3, some_matrix_2_3});
  uniform.UpdateValue(eigen::vector<Mat2x3T>(10, some_matrix_2_3));
}
