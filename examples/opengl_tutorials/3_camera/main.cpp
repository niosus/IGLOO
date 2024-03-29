#include "absl/strings/str_format.h"
#include "gl/core/buffer.h"
#include "gl/core/program.h"
#include "gl/core/shader.h"
#include "gl/core/texture.h"
#include "gl/core/uniform.h"
#include "gl/core/vertex_array_buffer.h"
#include "gl/ui/glfw/viewer.h"
#include "gl/utils/camera.h"
#include "gl/utils/eigen_traits.h"
#include "utils/eigen_utils.h"
#include "utils/image.h"

#include <Eigen/Geometry>
#include <GLFW/glfw3.h>

#include <iostream>
#include <ostream>
#include <vector>

using namespace units::literals;
using utils::Image;

const eigen::vector<Eigen::Vector3f> vertices{
    {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f},
    {0.5f, 0.5f, -0.5f},   {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, 0.5f},  {0.5f, -0.5f, 0.5f},  {0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},    {-0.5f, 0.5f, 0.5f},  {-0.5f, -0.5f, 0.5f},
    {-0.5f, 0.5f, 0.5f},   {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},    {0.5f, 0.5f, -0.5f},  {0.5f, -0.5f, -0.5f},
    {0.5f, -0.5f, -0.5f},  {0.5f, -0.5f, 0.5f},  {0.5f, 0.5f, 0.5f},
    {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, 0.5f},
    {0.5f, -0.5f, 0.5f},   {-0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, -0.5f},
    {-0.5f, 0.5f, -0.5f},  {0.5f, 0.5f, -0.5f},  {0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},    {-0.5f, 0.5f, 0.5f},  {-0.5f, 0.5f, -0.5f}};

const eigen::vector<Eigen::Vector2f> texture_coordinates{
    {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
    {0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f},
    {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
    {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f},
    {0.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f},
    {1.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f},
    {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f},
    {0.0f, 1.0f}};

Eigen::Vector3f GetLocalCameraPos(float radius,
                                  float rot_vertical,
                                  float rot_horizontal) {
  float radius_2d = radius * cosf(rot_vertical);
  return {radius_2d * cosf(rot_horizontal),
          radius_2d * sinf(rot_horizontal),
          radius * sinf(rot_vertical)};
}

int main(int argc, char* argv[]) {
  gl::glfw::Viewer viewer{"OpenGlViewer"};
  const bool success = viewer.Initialize();
  if (!success) { return EXIT_FAILURE; }

  const auto image_container = Image::CreateFrom(
      "examples/opengl_tutorials/3_camera/textures/container.jpg");
  const auto image_face = Image::CreateFrom(
      "examples/opengl_tutorials/3_camera/textures/awesomeface.png", true);
  if (!image_container || !image_face) {
    absl::FPrintF(stderr, "Error: images not found.\n");
    return EXIT_FAILURE;
  }

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
      "examples/opengl_tutorials/3_camera/shaders/triangle.vert")};
  const std::shared_ptr<gl::Shader> fragment_shader{gl::Shader::CreateFromFile(
      "examples/opengl_tutorials/3_camera/shaders/triangle.frag")};
  if (!vertex_shader || !fragment_shader) { exit(EXIT_FAILURE); }

  auto program =
      gl::Program::CreateFromShaders({vertex_shader, fragment_shader});
  if (!program) {
    std::cerr << "Failed to link program" << std::endl;
    return EXIT_FAILURE;
  }
  program->Use();
  (void)program->SetUniform("texture1", 0);
  (void)program->SetUniform("texture2", 1);

  float mixture = 0.5f;
  (void)program->SetUniform("mix_ratio", mixture);

  gl::Camera camera{};

  Eigen::Matrix4f perspective = camera.Perspective(
      45_deg, viewer.window_size().width, viewer.window_size().height);
  (void)program->SetUniform("projection", perspective);

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
                                   texture_coordinates));

  viewer.user_input_handler().RegisterKeyboardCallback(
      [&camera](const std::set<gl::core::KeyboardKey>& keys) {
        const float increment = 0.02f;
        if (keys.count(gl::core::KeyboardKey::kArrowUp)) {
          camera.Translate({-increment, 0.0f, 0.0f});
        }
        if (keys.count(gl::core::KeyboardKey::kArrowDown)) {
          camera.Translate({increment, 0.0f, 0.0f});
        }
        if (keys.count(gl::core::KeyboardKey::kArrowLeft)) {
          camera.Translate({0.0f, -increment, 0.0f});
        }
        if (keys.count(gl::core::KeyboardKey::kArrowRight)) {
          camera.Translate({0.0f, increment, 0.0f});
        }
      });

  viewer.user_input_handler().RegisterMouseCallback(
      [&camera](gl::core::MouseKey key,
                gl::core::PressState state,
                const gl::core::PointXY& increment) {
        if (state != gl::core::PressState::kPressed) { return; }
        float modifier = 0.01f;
        camera.Rotate(gl::Camera::RotationDirection::kHorizontal,
                      units::angle::radian_t{increment.x},
                      modifier);
        camera.Rotate(gl::Camera::RotationDirection::kVertical,
                      units::angle::radian_t{increment.y},
                      modifier);
      });

  camera.LookAt({0, 0, 0}, {5, 0, 0});

  /// Rotation around x axis by M_PI brings our normal computer vision camera
  // (z - forward from the user, x - right, y - down) to the opengl coordinate
  /// system (z - backwards, towards the user, x - right, y - up).
  const Eigen::Isometry3f tf_gl_camera_from_camera{
      Eigen::AngleAxisf{M_PI, Eigen::Vector3f::UnitX()}};

  while (!viewer.ShouldClose()) {
    viewer.ProcessInput();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    program->Use();
    texture_1->Bind();
    texture_2->Bind();

    (void)program->SetUniform(
        "view",
        (tf_gl_camera_from_camera * camera.tf_camera_from_world()).matrix());
    Eigen::Affine3f model{Eigen::AngleAxisf{0.0f, Eigen::Vector3f::UnitX()}};
    (void)program->SetUniform("model", model.matrix());

    vertex_array_buffer.Draw(GL_TRIANGLES);
    viewer.Spin();
  }
  return EXIT_SUCCESS;
}
