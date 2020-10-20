#include "absl/strings/str_format.h"
#include "examples/3d_viewer/utils/point_cloud.h"
#include "gl/scene/drawables/all.h"
#include "gl/viewer/viewer.h"

#include <Eigen/Geometry>
#include <GLFW/glfw3.h>

#include <iostream>
#include <ostream>
#include <vector>

using namespace units::literals;
using utils::Image;

int main(int argc, char* argv[]) {
  gl::SceneViewer viewer{"3D Scene Viewer"};
  viewer.Initialize();

  auto cloud_ptr =
      PointCloud::FromFile("examples/3d_viewer/utils/test_data/cloud.txt");
  CHECK(cloud_ptr);

  auto face_image = Image::CreateFrom(
      "examples/3d_viewer/utils/test_data/awesomeface.png", true /* flipped */);
  CHECK(face_image);

  auto points_drawable = std::make_shared<gl::Points>(
      viewer.program_pool(), cloud_ptr->points(), cloud_ptr->intensities());

  // auto image_facing_user =
  //     std::make_shared<gl::RectWithTexture>(viewer.program_pool(),
  //                                           face_image.value(),
  //                                           Eigen::Vector3f{0.0f, 0.0f,
  //                                           0.0f});
  // auto image_facing_user_further = std::make_shared<gl::RectWithTexture>(
  //     viewer.program_pool(),
  //     face_image.value(),
  //     Eigen::Vector3f{-10.0f, 0.0f, 0.0f});

  auto image_on_screen =
      std::make_shared<gl::RectWithTexture>(viewer.program_pool(),
                                            face_image.value(),
                                            Eigen::Vector2f{0.0f, 0.0f},
                                            Eigen::Vector2f{0.5f, 0.5f});

  auto camera_center_drawable =
      std::make_shared<gl::CoordinateSystem>(viewer.program_pool());

  viewer.Attach(viewer.world_key(), points_drawable);
  viewer.Attach(viewer.camera_key(), camera_center_drawable);
  // viewer.Attach(viewer.world_key(), image_facing_user);
  // viewer.Attach(viewer.world_key(), image_facing_user_further);
  viewer.Attach(viewer.viewport_key(), image_on_screen);

  viewer.camera().LookAt({0.0f, 0.0f, 0.0f}, {-10.0f, 0.0f, 3.0f});

  viewer.Spin();
  return EXIT_SUCCESS;
}
