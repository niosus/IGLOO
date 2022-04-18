#include "gl/core/program.h"
#include "gl/core/uniform.h"
#include "gl/ui/glfw/viewer.h"
#include "gl/utils/eigen_traits.h"
#include "utils/eigen_utils.h"
#include "gtest/gtest.h"

using namespace gl;

class UniformTest : public ::testing::Test {
 protected:
  void SetUp() override {
    const std::shared_ptr<Shader> vertex_shader{
        Shader::CreateFromFile("gl/core/test_shaders/shader.vert")};
    ASSERT_NE(vertex_shader, nullptr);
    const std::shared_ptr<Shader> fragment_shader{
        Shader::CreateFromFile("gl/core/test_shaders/shader.frag")};
    ASSERT_NE(fragment_shader, nullptr);
    program_ = Program::CreateFromShaders({vertex_shader, fragment_shader});
    ASSERT_NE(program_, nullptr);
    int success{};
    glGetProgramiv(program_->id(), GL_LINK_STATUS, &success);
    ASSERT_TRUE(success);
    program_->Use();
  }

  std::shared_ptr<Program> program_{};
};

TEST_F(UniformTest, Init) {
  Uniform uniform{"dummy_value_dim_1", program_->id()};
  EXPECT_EQ("dummy_value_dim_1", uniform.name());
  EXPECT_EQ(uniform.location(), 0);
}

TEST_F(UniformTest, UpdateValueFromPack) {
  Uniform uniform_1{"dummy_value_dim_1", program_->id()};
  EXPECT_EQ(uniform_1.location(), 0);
  uniform_1.UpdateValue(1.0f);
  Uniform uniform_2{"dummy_value_dim_2", program_->id()};
  EXPECT_EQ(uniform_2.location(), 1);
  uniform_2.UpdateValue(1.0f, 2.0f);
  Uniform uniform_3{"dummy_value_dim_3", program_->id()};
  EXPECT_EQ(uniform_3.location(), 2);
  uniform_3.UpdateValue(1.0f, 2.0f, 3.0f);
  Uniform uniform_4{"dummy_value_dim_4", program_->id()};
  EXPECT_EQ(uniform_4.location(), 3);
  uniform_4.UpdateValue(1.0f, 2.0f, 3.0f, 4.0f);
}

TEST_F(UniformTest, UpdateValueFromEigenMat) {
  Uniform uniform_2{"dummy_value_dim_2", program_->id()};
  EXPECT_EQ(uniform_2.location(), 1);
  uniform_2.UpdateValue(Eigen::Vector2f{1.0f, 2.0f});
  Uniform uniform_3{"dummy_value_dim_3", program_->id()};
  EXPECT_EQ(uniform_3.location(), 2);
  uniform_3.UpdateValue(Eigen::Vector3f{1.0f, 2.0f, 3.0f});
  Uniform uniform_4{"dummy_value_dim_4", program_->id()};
  EXPECT_EQ(uniform_4.location(), 3);
  uniform_4.UpdateValue(Eigen::Vector4f{1.0f, 2.0f, 3.0f, 4.0f});
}

TEST_F(UniformTest, UpdateValueFromPackOfIntegers) {
  Uniform uniform_1{"i_dummy_value_dim_1", program_->id()};
  EXPECT_EQ(uniform_1.location(), 4);
  uniform_1.UpdateValue(1);
  Uniform uniform_2{"i_dummy_value_dim_2", program_->id()};
  EXPECT_EQ(uniform_2.location(), 5);
  uniform_2.UpdateValue(1, 2);
  Uniform uniform_3{"i_dummy_value_dim_3", program_->id()};
  EXPECT_EQ(uniform_3.location(), 6);
  uniform_3.UpdateValue(1, 2, 3);
  Uniform uniform_4{"i_dummy_value_dim_4", program_->id()};
  EXPECT_EQ(uniform_4.location(), 7);
  uniform_4.UpdateValue(1, 2, 3, 4);
}

TEST_F(UniformTest, UpdateValueFromArray) {
  // 2 elements
  Uniform uniform_dim_2{"dummy_value_dim_2", program_->id()};
  uniform_dim_2.UpdateValue(eigen::vector<Eigen::Vector2f>{{1.0f, 2.0f}});
  Uniform uniform_vec_vec_2{"vec_of_vec_2", program_->id()};
  uniform_vec_vec_2.UpdateValue(
      eigen::vector<Eigen::Vector2f>{{1.0f, 2.0f}, {2.0f, 3.0f}});
  // 3 elements
  Uniform uniform_dim_3{"dummy_value_dim_3", program_->id()};
  uniform_dim_3.UpdateValue(eigen::vector<Eigen::Vector3f>{{1.0f, 2.0f, 3.0f}});
  Uniform uniform_vec_vec_3{"vec_of_vec_3", program_->id()};
  uniform_vec_vec_3.UpdateValue(
      eigen::vector<Eigen::Vector3f>{{1.0f, 2.0f, 3.0f}, {1.0f, 2.0f, 3.0f}});
  // 4 elements
  Uniform uniform_dim_4{"dummy_value_dim_4", program_->id()};
  uniform_dim_4.UpdateValue(
      eigen::vector<Eigen::Vector4f>{{1.0f, 2.0f, 3.0f, 4.0f}});
}

TEST_F(UniformTest, UpdateValueFromMatrix) {
  // 2x2
  Uniform uniform_mat_2{"matrix_2", program_->id()};
  Eigen::Matrix2f small_mat;
  small_mat << 1.0f, 2.0f, 3.0f, 4.0f;
  uniform_mat_2.UpdateValue(small_mat);
  uniform_mat_2.UpdateValue(eigen::vector<Eigen::Matrix2f>{small_mat});
  // 3x3
  Uniform uniform_mat_3{"matrix_3", program_->id()};
  Eigen::Matrix3f some_matrix;
  some_matrix << 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f;
  uniform_mat_3.UpdateValue(some_matrix);
  uniform_mat_3.UpdateValue(eigen::vector<Eigen::Matrix3f>{some_matrix});
  // 2x3
  Uniform uniform_mat_2x3{"matrix_2x3", program_->id()};
  using Mat2x3T = Eigen::Matrix<float, 2, 3>;
  Mat2x3T some_matrix_2_3;
  some_matrix_2_3 << 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f;
  uniform_mat_2x3.UpdateValue(some_matrix_2_3);
  uniform_mat_2x3.UpdateValue(eigen::vector<Mat2x3T>{some_matrix_2_3});
}

TEST_F(UniformTest, EverythingIsPossibleWithNonExistingUniform) {
  Uniform uniform{"non_existing_name", program_->id()};
  EXPECT_EQ(uniform.location(), -1);
  uniform.UpdateValue(1);
  uniform.UpdateValue(1.0f);
  uniform.UpdateValue(1.0f, 2.0f);
  uniform.UpdateValue(Eigen::Vector3f{1.0f, 2.0f, 3.0f});
}

TEST(UniformDeathTest, InitWithoutProgram) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  EXPECT_DEBUG_DEATH(Uniform("some_name", 0), ".*GL_INVALID_VALUE.*");
}
