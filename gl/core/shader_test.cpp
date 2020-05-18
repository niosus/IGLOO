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

TEST(ShaderTest, Move) {
  auto shader{Shader::CreateFromFile("gl/core/test_shaders/shader.vert")};
  ASSERT_NE(shader, nullptr);
  ASSERT_NE(shader->id(), 0u);
  const auto shader_id{shader->id()};
  const auto raw_shader_ptr{shader.release()};
  const auto other_shader{std::move(*raw_shader_ptr)};
  int is_being_deleted{};
  glGetShaderiv(shader_id, GL_DELETE_STATUS, &is_being_deleted);
  ASSERT_FALSE(is_being_deleted);
  EXPECT_EQ(shader_id, other_shader.id());
}

TEST(ShaderDeathTest, InitWrongSource) {
  EXPECT_DEBUG_DEATH(
      Shader::CreateFromFile("gl/core/test_shaders/wrong_shader.vert"),
      ".*error: syntax error.*");
}
