#include "opengl_tutorials/core/buffer.h"
#include "opengl_tutorials/core/program.h"
#include "opengl_tutorials/core/shader.h"
#include "opengl_tutorials/core/vertex_array_buffer.h"
#include "opengl_tutorials/ui/glfw/viewer.h"
#include "opengl_tutorials/utils/eigen_utils.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <ostream>
#include <vector>

const gl_tutorials::eigen::vector<Eigen::Vector3f> vertices{
    {0.5f, 0.5f, 0.0f},    // top right
    {0.5f, -0.5f, 0.0f},   // bottom right
    {-0.5f, -0.5f, 0.0f},  // bottom left
    {-0.5f, 0.5f, 0.0f}    // top left
};
const std::vector<uint32_t> indices = {0, 1, 3, 1, 2, 3};  // Two triangles.

int main(int argc, char const *argv[]) {
  gl_tutorials::glfw::Viewer viewer{"OpenGlViewer"};
  bool success = viewer.Initialize();
  if (!success) { return EXIT_FAILURE; }

  const std::shared_ptr<gl_tutorials::Shader> vertex_shader{
      gl_tutorials::Shader::CreateFromFile(
          "opengl_tutorials/examples/hello_triangle/shaders/triangle.vert")};
  const std::shared_ptr<gl_tutorials::Shader> fragment_shader{
      gl_tutorials::Shader::CreateFromFile(
          "opengl_tutorials/examples/hello_triangle/shaders/triangle.frag")};
  if (!vertex_shader || !fragment_shader) { exit(EXIT_FAILURE); }

  const auto program = gl_tutorials::Program::CreateFromShaders(
      {vertex_shader, fragment_shader});
  if (!program) {
    std::cerr << "Failed to link program" << std::endl;
    return EXIT_FAILURE;
  }

  gl_tutorials::VertexArrayBuffer vertex_array_buffer{};
  vertex_array_buffer.AssignBuffer(std::make_shared<gl_tutorials::Buffer>(
      gl_tutorials::Buffer::Type::kArrayBuffer,
      gl_tutorials::Buffer::Usage::kStaticDraw,
      vertices));
  vertex_array_buffer.AssignBuffer(std::make_shared<gl_tutorials::Buffer>(
      gl_tutorials::Buffer::Type::kElementArrayBuffer,
      gl_tutorials::Buffer::Usage::kStaticDraw,
      indices));
  vertex_array_buffer.EnableVertexAttributePointer(0);

  while (!viewer.ShouldClose()) {
    viewer.ProcessInput();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    program->Use();

    vertex_array_buffer.Draw(GL_TRIANGLES);

    viewer.Spin();
  }
  return EXIT_SUCCESS;
}
