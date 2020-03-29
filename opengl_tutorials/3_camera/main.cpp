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

using utils::Image;

const eigen::vector<float> vertices{
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

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

  const auto image_container =
      Image::CreateFrom("opengl_tutorials/3_camera/textures/container.jpg");
  const auto image_face = Image::CreateFrom(
      "opengl_tutorials/3_camera/textures/awesomeface.png", true);
  if (!image_container || !image_face) {
    absl::FPrintF(stderr, "Error: images not found.\n");
    return EXIT_FAILURE;
  }
  absl::PrintF("Width: %d, Height: %d, Number of channels: %d\n",
               image_container->width(),
               image_container->height(),
               image_container->number_of_channels());

  absl::PrintF("Width: %d, Height: %d, Number of channels: %d\n",
               image_face->width(),
               image_face->height(),
               image_face->number_of_channels());

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
      "opengl_tutorials/3_camera/shaders/triangle.vert")};
  const std::shared_ptr<gl::Shader> fragment_shader{gl::Shader::CreateFromFile(
      "opengl_tutorials/3_camera/shaders/triangle.frag")};
  if (!vertex_shader || !fragment_shader) { exit(EXIT_FAILURE); }

  const auto program =
      gl::Program::CreateFromShaders({vertex_shader, fragment_shader});
  if (!program) {
    std::cerr << "Failed to link program" << std::endl;
    return EXIT_FAILURE;
  }
  program->Use();
  program->SetUniform("texture1", 0);
  program->SetUniform("texture2", 1);

  float mixture = 0.5f;
  program->SetUniform("mix_ratio", mixture);

  gl::Camera camera{};

  Eigen::Matrix4f perspective = camera.Perspective(M_PI_4,
                                                   viewer.window_size().width,
                                                   viewer.window_size().height,
                                                   0.1f,
                                                   100.0f);
  program->SetUniform("projection", perspective);

  gl::VertexArrayBuffer vertex_array_buffer{};
  vertex_array_buffer.AssignBuffer(
      std::make_shared<gl::Buffer>(gl::Buffer::Type::kArrayBuffer,
                                   gl::Buffer::Usage::kStaticDraw,
                                   vertices));
  const int stride = 5;
  const int pos_offset = 0;
  const int vertex_layout_index = 0;
  const int components_per_vertex_entry = 3;
  vertex_array_buffer.EnableVertexAttributePointer(
      vertex_layout_index, stride, pos_offset, components_per_vertex_entry);
  const int tex_layout_index = 1;
  const int tex_coords_offset = 3;
  const int components_per_tex_entry = 2;
  vertex_array_buffer.EnableVertexAttributePointer(
      tex_layout_index, stride, tex_coords_offset, components_per_tex_entry);

  while (!viewer.ShouldClose()) {
    viewer.ProcessInput();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    program->Use();
    texture_1->Bind();
    texture_2->Bind();
    float camX = sin(glfwGetTime()) * 3;
    float camY = cos(glfwGetTime()) * 3;
    camera.LookAt({0, 0, 0}, {camX, camY, 0});
    program->SetUniform("view", camera.tf_camera_world().matrix());
    Eigen::Affine3f model{Eigen::AngleAxisf{1.0f, Eigen::Vector3f::UnitX()}};
    program->SetUniform("model", model.matrix());

    vertex_array_buffer.Draw(GL_TRIANGLES, stride);
    viewer.Spin();
  }
  return EXIT_SUCCESS;
}
