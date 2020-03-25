#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"
#include "opengl_tutorials/core/buffer.h"
#include "opengl_tutorials/core/program.h"
#include "opengl_tutorials/core/shader.h"
#include "opengl_tutorials/core/uniform.h"
#include "opengl_tutorials/core/vertex_array_buffer.h"
#include "opengl_tutorials/ui/glfw/viewer.h"
#include "opengl_tutorials/utils/eigen_utils.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <ostream>
#include <vector>

const gl_tutorials::eigen::vector<Eigen::Vector3f> vertices{
    {0.5f, 0.5f, 0.0f},    // top right
    {1.0f, 0.0f, 0.0f},    // top right color
    {0.5f, -0.5f, 0.0f},   // bottom right
    {0.0f, 0.0f, 0.0f},    // bottom right color
    {-0.5f, -0.5f, 0.0f},  // bottom left
    {0.0f, 0.0f, 1.0f},    // bottom left color
    {-0.5f, 0.5f, 0.0f},   // top left
    {0.0f, 0.0f, 0.0f}     // top left color
};
const std::vector<uint32_t> indices = {0, 1, 3, 1, 2, 3};  // Two triangles.

int main(int argc, char *argv[]) {
  absl::ParseCommandLine(argc, argv);

  gl_tutorials::glfw::Viewer viewer{"OpenGlViewer"};
  const bool success = viewer.Initialize();
  if (!success) { return EXIT_FAILURE; }

  const std::string fragment_shader_source{
      "opengl_tutorials/examples/hello_triangle/shaders/triangle.frag"};

  const std::shared_ptr<gl_tutorials::Shader> vertex_shader{
      gl_tutorials::Shader::CreateFromFile(
          "opengl_tutorials/examples/hello_triangle/shaders/triangle.vert")};
  const std::shared_ptr<gl_tutorials::Shader> fragment_shader{
      gl_tutorials::Shader::CreateFromFile(fragment_shader_source)};
  if (!vertex_shader || !fragment_shader) { exit(EXIT_FAILURE); }

  const auto program = gl_tutorials::Program::CreateFromShaders(
      {vertex_shader, fragment_shader});
  if (!program) {
    std::cerr << "Failed to link program" << std::endl;
    return EXIT_FAILURE;
  }

  gl_tutorials::Uniform uniform{"ourColor", program->id()};

  gl_tutorials::VertexArrayBuffer vertex_array_buffer{};
  vertex_array_buffer.AssignBuffer(std::make_shared<gl_tutorials::Buffer>(
      gl_tutorials::Buffer::Type::kArrayBuffer,
      gl_tutorials::Buffer::Usage::kStaticDraw,
      vertices));
  vertex_array_buffer.AssignBuffer(std::make_shared<gl_tutorials::Buffer>(
      gl_tutorials::Buffer::Type::kElementArrayBuffer,
      gl_tutorials::Buffer::Usage::kStaticDraw,
      indices));
  const int stride = 2;
  const int pos_offset = 0;
  vertex_array_buffer.EnableVertexAttributePointer(0, stride, pos_offset);
  const int color_offset = 1;
  vertex_array_buffer.EnableVertexAttributePointer(1, stride, color_offset);

  while (!viewer.ShouldClose()) {
    viewer.ProcessInput();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    program->Use();
    float green_value = (sin(viewer.GetTime()) / 2.0f) + 0.5f;
    uniform.UpdateValue(0.0f, green_value, 0.0f);

    vertex_array_buffer.Draw(GL_TRIANGLES);
    viewer.Spin();
  }
  return EXIT_SUCCESS;
}
