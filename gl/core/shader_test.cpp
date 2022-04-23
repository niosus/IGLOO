#include "gl/core/shader.h"
#include "gtest/gtest.h"

using gl::Shader;

TEST(ShaderTest, Init) {
  auto shader{Shader::CreateFromFile("gl/core/test_shaders/shader.vert")};
  ASSERT_NE(shader, nullptr);
  ASSERT_NE(shader->id(), 0u);
  EXPECT_EQ(Shader::Type::kVertexShader, shader->type())
      << static_cast<GLint>(shader->type());
  int success;
  glGetShaderiv(shader->id(), GL_COMPILE_STATUS, &success);
  EXPECT_TRUE(success);
}

TEST(ShaderDeathTest, InitWrongPath) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  EXPECT_DEATH(Shader::CreateFromFile("wrong/path.vert"), ".*does not exist.");
}

TEST(ShaderDeathTest, InitWrongSource) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  EXPECT_DEATH(Shader::CreateFromFile("gl/core/test_shaders/wrong_shader.vert"),
               ".*syntax error.*");
}
