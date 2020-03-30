#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"
#include "gl/core/buffer.h"
#include "gl/core/program.h"
#include "gl/core/shader.h"
#include "gl/core/texture.h"
#include "gl/core/uniform.h"
#include "gl/core/vertex_array_buffer.h"
#include "gl/ui/core/user_input_handler.h"
#include "gl/ui/glfw/viewer.h"
#include "gl/utils/eigen_traits.h"
#include "glog/logging.h"
#include "utils/eigen_utils.h"
#include "utils/image.h"

#include <Eigen/Geometry>
#include <GLFW/glfw3.h>

#include <iostream>
#include <ostream>
#include <vector>

using utils::Image;

const eigen::vector<Eigen::Vector3f> vertices{
    {0.5f, 0.5f, 0.0f},
    {0.5f, -0.5f, 0.0f},
    {-0.5f, -0.5f, 0.0f},
    {-0.5f, 0.5f, 0.0f},
};

const eigen::vector<Eigen::Vector3f> colors{
    {1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f},
};

const eigen::vector<Eigen::Vector2f> texture_coordinates{
    {2.0f, 2.0f},
    {2.0f, 0.0f},
    {0.0f, 0.0f},
    {0.0f, 2.0f},
};

const std::vector<uint32_t> indices = {0, 1, 3, 1, 2, 3};  // Two triangles.

int main(int argc, char* argv[]) {
  absl::ParseCommandLine(argc, argv);

  gl::glfw::Viewer viewer{"OpenGlViewer"};
  const bool success = viewer.Initialize();
  CHECK(success) << "Viewer could not initialize.";

  const auto image_container =
      Image::CreateFrom("opengl_tutorials/2_textures/textures/container.jpg");
  const auto image_face = Image::CreateFrom(
      "opengl_tutorials/2_textures/textures/awesomeface.png", true);
  CHECK(image_container.has_value() && image_face.has_value())
      << "Error: images not found.";

  auto texture_1{gl::Texture::Builder{gl::Texture::Type::kTexture2D,
                                      gl::Texture::Identifier::kTexture0}
                     .WithSaneDefaults()
                     .WithImage(*image_container)
                     .Build()};
  auto texture_2{gl::Texture::Builder{gl::Texture::Type::kTexture2D,
                                      gl::Texture::Identifier::kTexture1}
                     .WithSaneDefaults()
                     .WithImage(*image_face)
                     .Build()};

  const std::shared_ptr<gl::Shader> vertex_shader{gl::Shader::CreateFromFile(
      "opengl_tutorials/2_textures/shaders/triangle.vert")};
  const std::shared_ptr<gl::Shader> fragment_shader{gl::Shader::CreateFromFile(
      "opengl_tutorials/2_textures/shaders/triangle.frag")};
  CHECK(vertex_shader) << "Could not create vertex shader.";
  CHECK(fragment_shader) << "Could not create fragment shader.";

  const auto program =
      gl::Program::CreateFromShaders({vertex_shader, fragment_shader});
  CHECK(program) << "Failed to link program.";

  program->Use();
  program->SetUniform("texture1", 0);
  program->SetUniform("texture2", 1);

  float mixture = 0.0f;
  program->SetUniform("mix_ratio", mixture);

  Eigen::Affine3f transform{
      Eigen::AngleAxisf{mixture, Eigen::Vector3f::UnitZ()}};
  program->SetUniform("transform", transform.matrix());

  gl::VertexArrayBuffer vertex_array_buffer{};
  vertex_array_buffer.EnableVertexAttributePointer(
      0,
      std::make_shared<gl::Buffer>(gl::Buffer::Type::kArrayBuffer,
                                   gl::Buffer::Usage::kStaticDraw,
                                   vertices));
  vertex_array_buffer.EnableVertexAttributePointer(
      1,
      std::make_shared<gl::Buffer>(gl::Buffer::Type::kArrayBuffer,
                                   gl::Buffer::Usage::kStaticDraw,
                                   colors));
  vertex_array_buffer.EnableVertexAttributePointer(
      2,
      std::make_shared<gl::Buffer>(gl::Buffer::Type::kArrayBuffer,
                                   gl::Buffer::Usage::kStaticDraw,
                                   texture_coordinates));
  vertex_array_buffer.AssignBuffer(
      std::make_shared<gl::Buffer>(gl::Buffer::Type::kElementArrayBuffer,
                                   gl::Buffer::Usage::kStaticDraw,
                                   indices));

  auto on_key_press = [&mixture, &program](gl::KeyboardKey key_press,
                                           gl::PressState press_state) {
    if (key_press == gl::KeyboardKey::kArrowUp) {
      mixture = std::min(1.0f, mixture + 0.02f);
    } else if (key_press == gl::KeyboardKey::kArrowDown) {
      mixture = std::max(0.0f, mixture - 0.02f);
    }
    Eigen::Affine3f transform{
        Eigen::AngleAxisf{mixture, Eigen::Vector3f{0, 0, 1}}};
    program->SetUniform("transform", transform.matrix());
    program->SetUniform("mix_ratio", mixture);
  };

  viewer.user_input_handler().RegisterKeyboardCallback(on_key_press);

  while (!viewer.ShouldClose()) {
    viewer.ProcessInput();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    program->Use();
    texture_1->Bind();
    texture_2->Bind();

    vertex_array_buffer.Draw(GL_TRIANGLES);
    viewer.Spin();
  }
  return EXIT_SUCCESS;
}
