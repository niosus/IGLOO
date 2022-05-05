#include "absl/strings/str_format.h"
#include "examples/3d_viewer/utils/point_cloud.h"
#include "gl/scene/drawables/all.h"
#include "gl/viewer/viewer.h"

#include <Eigen/Geometry>
#include <GLFW/glfw3.h>

#include <iostream>
#include <ostream>
#include <vector>

namespace {

using namespace units::literals;
using gl::Shader;
using utils::Image;

}  // namespace

int main(int argc, char* argv[]) {
  gl::SceneViewer viewer{"3D Scene Viewer"};
  viewer.Initialize();

  auto& program_pool = viewer.program_pool();

  const auto draw_points_program_index =
      program_pool.AddProgramFromShaders(Shader::CreateFromFiles(
          {"gl/scene/shaders/points.vert", "gl/scene/shaders/simple.frag"}));
  CHECK(draw_points_program_index.has_value());
  const auto draw_coordinate_system_program_index =
      program_pool.AddProgramFromShaders(
          Shader::CreateFromFiles({"gl/scene/shaders/coordinate_system.vert",
                                   "gl/scene/shaders/coordinate_system.geom",
                                   "gl/scene/shaders/simple.frag"}));
  CHECK(draw_coordinate_system_program_index.has_value());
  const auto draw_textured_rect_program_index =
      program_pool.AddProgramFromShaders(
          Shader::CreateFromFiles({"gl/scene/shaders/texture.vert",
                                   "gl/scene/shaders/texture.geom",
                                   "gl/scene/shaders/texture.frag"}));
  CHECK(draw_textured_rect_program_index.has_value());
  const auto draw_textured_rect_on_screen_program_index =
      program_pool.AddProgramFromShaders(
          Shader::CreateFromFiles({"gl/scene/shaders/texture.vert",
                                   "gl/scene/shaders/texture_on_screen.geom",
                                   "gl/scene/shaders/texture.frag"}));
  CHECK(draw_textured_rect_on_screen_program_index.has_value());
  const auto draw_text_program_index =
      program_pool.AddProgramFromShaders(Shader::CreateFromFiles(
          {"gl/scene/shaders/text.vert", "gl/scene/shaders/texture.frag"}));
  CHECK(draw_text_program_index.has_value());

  auto cloud_ptr =
      PointCloud::FromFile("examples/3d_viewer/utils/test_data/cloud.txt");
  CHECK(cloud_ptr);

  const auto image_face = Image::CreateFrom(
      "examples/3d_viewer/utils/test_data/textures/awesomeface.png", true);
  CHECK(image_face);
  const std::shared_ptr<gl::Texture> texture_face =
      gl::Texture::Builder{gl::Texture::Type::kTexture2D,
                           gl::Texture::Identifier::kTexture0}
          .WithSaneDefaults()
          .WithImage(*image_face)
          .Build();

  const auto points_drawable =
      std::make_shared<gl::Points>(&viewer.program_pool(),
                                   draw_points_program_index.value(),
                                   cloud_ptr->points(),
                                   cloud_ptr->intensities());

  const auto camera_center_drawable = std::make_shared<gl::CoordinateSystem>(
      &viewer.program_pool(), draw_coordinate_system_program_index.value());

  const auto texture_3d_drawable = std::make_shared<gl::RectWithTexture>(
      &viewer.program_pool(),
      draw_textured_rect_program_index.value(),
      texture_face,
      Eigen::Vector2f{1.0F, 1.0F});

  const auto texture_2d_drawable = std::make_shared<gl::RectWithTexture>(
      &viewer.program_pool(),
      draw_textured_rect_on_screen_program_index.value(),
      texture_face,
      Eigen::Vector2f{0.5F, 0.5F});

  viewer.Attach(viewer.world_key(), points_drawable);
  viewer.Attach(viewer.camera_key(), camera_center_drawable);
  viewer.Attach(
      viewer.world_key(),
      texture_3d_drawable,
      Eigen::Translation3f{5.0F, 0.0F, 0.0F} *
          Eigen::AngleAxisf(0.0F, Eigen::Vector3f::UnitX()) *
          Eigen::AngleAxisf(-0.5F * M_PIf32, Eigen::Vector3f::UnitY()) *
          Eigen::AngleAxisf(-0.5F * M_PIf32, Eigen::Vector3f::UnitZ()));
  viewer.Attach(
      viewer.world_key(),
      texture_3d_drawable,
      Eigen::Translation3f{-1.0F, -1.0F, 2.0F} *
          Eigen::AngleAxisf(0.0F, Eigen::Vector3f::UnitX()) *
          Eigen::AngleAxisf(0.0, Eigen::Vector3f::UnitY()) *
          Eigen::AngleAxisf(-0.5F * M_PIf32, Eigen::Vector3f::UnitZ()));
  viewer.AttachToScreen(texture_2d_drawable, {0.5f, 0.5f});

  viewer.camera().LookAt({0.0f, 0.0f, 0.0f}, {-10.0f, 0.0f, 3.0f});

  viewer.Spin();
  return EXIT_SUCCESS;
}
