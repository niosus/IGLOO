#include "gl/core/shader.h"
#include "gtest/gtest.h"

using namespace gl;

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

TEST(ShaderTest, InitWrongPath) {
  const auto shader{Shader::CreateFromFile("wrong/path.vert")};
  ASSERT_EQ(shader, nullptr);
}

TEST(ShaderDeathTest, InitWrongSource) {
  EXPECT_DEBUG_DEATH(
      Shader::CreateFromFile("gl/core/test_shaders/wrong_shader.vert"),
      ".*error: syntax error.*");
}
