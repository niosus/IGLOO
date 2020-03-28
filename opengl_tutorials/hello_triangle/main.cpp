#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"
#include "gl/core/buffer.h"
#include "gl/core/program.h"
#include "gl/core/shader.h"
#include "gl/core/uniform.h"
#include "gl/core/vertex_array_buffer.h"
#include "gl/ui/glfw/viewer.h"
#include "gl/utils/eigen_traits.h"
#include "utils/eigen_utils.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <ostream>
#include <vector>

const eigen::vector<Eigen::Vector3f> vertices{
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

int main(int argc, char* argv[]) {
  absl::ParseCommandLine(argc, argv);

  gl::glfw::Viewer viewer{"OpenGlViewer"};
  const bool success = viewer.Initialize();
  if (!success) { return EXIT_FAILURE; }

  const std::shared_ptr<gl::Shader> vertex_shader{gl::Shader::CreateFromFile(
      "opengl_tutorials/hello_triangle/shaders/triangle.vert")};
  const std::shared_ptr<gl::Shader> fragment_shader{gl::Shader::CreateFromFile(
      "opengl_tutorials/hello_triangle/shaders/triangle.frag")};
  if (!vertex_shader || !fragment_shader) { exit(EXIT_FAILURE); }

  const auto program =
      gl::Program::CreateFromShaders({vertex_shader, fragment_shader});
  if (!program) {
    std::cerr << "Failed to link program" << std::endl;
    return EXIT_FAILURE;
  }

  auto* color_uniform = program->EmplaceUniform({"ourColor", program->id()});

  gl::VertexArrayBuffer vertex_array_buffer{};
  vertex_array_buffer.AssignBuffer(
      std::make_shared<gl::Buffer>(gl::Buffer::Type::kArrayBuffer,
                                   gl::Buffer::Usage::kStaticDraw,
                                   vertices));
  vertex_array_buffer.AssignBuffer(
      std::make_shared<gl::Buffer>(gl::Buffer::Type::kElementArrayBuffer,
                                   gl::Buffer::Usage::kStaticDraw,
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
    float green_value = (sin(viewer.GetTime() * 3.0f) / 2.0f) + 0.5f;
    color_uniform->UpdateValue(0.0f, green_value, 0.0f);

    vertex_array_buffer.Draw(GL_TRIANGLES);
    viewer.Spin();
  }
  return EXIT_SUCCESS;
}
