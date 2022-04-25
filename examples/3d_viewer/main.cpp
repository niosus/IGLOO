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
  const auto draw_text_program_index =
      program_pool.AddProgramFromShaders(Shader::CreateFromFiles(
          {"gl/scene/shaders/text.vert", "gl/scene/shaders/texture.frag"}));
  CHECK(draw_text_program_index.has_value());

  auto cloud_ptr =
      PointCloud::FromFile("examples/3d_viewer/utils/test_data/cloud.txt");

  auto points_drawable =
      std::make_shared<gl::Points>(&viewer.program_pool(),
                                   draw_points_program_index.value(),
                                   cloud_ptr->points(),
                                   cloud_ptr->intensities());

  auto camera_center_drawable = std::make_shared<gl::CoordinateSystem>(
      &viewer.program_pool(), draw_coordinate_system_program_index.value());

  viewer.Attach(viewer.world_key(), points_drawable);
  viewer.Attach(viewer.camera_key(), camera_center_drawable);

  viewer.camera().LookAt({0.0f, 0.0f, 0.0f}, {-10.0f, 0.0f, 3.0f});

  viewer.Spin();
  return EXIT_SUCCESS;
}
