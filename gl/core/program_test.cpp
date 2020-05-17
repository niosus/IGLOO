#include "gl/core/program.h"
#include "gtest/gtest.h"

using namespace gl;

TEST(ProgramTest, Init) {
  const std::shared_ptr<Shader> vertex_shader{
      Shader::CreateFromFile("gl/core/test_shaders/shader.vert")};
  ASSERT_NE(vertex_shader, nullptr);
  const std::shared_ptr<Shader> fragment_shader{
      Shader::CreateFromFile("gl/core/test_shaders/shader.frag")};
  ASSERT_NE(fragment_shader, nullptr);
  const auto program{
      Program::CreateFromShaders({vertex_shader, fragment_shader})};
  ASSERT_NE(program, nullptr);
}
