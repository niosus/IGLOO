#include "absl/strings/str_format.h"
#include "gl/scene/drawables/all.h"
#include "gl/viewer/viewer.h"

#include <Eigen/Geometry>
#include <GLFW/glfw3.h>

#include <iostream>
#include <ostream>
#include <vector>

using namespace units::literals;
using utils::Image;

eigen::vector<Eigen::Vector3f> GenerateCloud() {
  eigen::vector<Eigen::Vector3f> points;
  float step{0.1f};
  float distance{10.0f};
  for (float angle = 0.0f; angle < 2.0f * M_PIf32; angle += step) {
    points.push_back({distance * sinf(angle), distance * cosf(angle), 0.0f});
  }
  return points;
}

int main(int argc, char* argv[]) {
  gl::SceneViewer viewer{"3D Scene Viewer"};
  viewer.Initialize();

  auto points_drawable =
      std::make_shared<gl::Points>(viewer.program_pool(), GenerateCloud());

  viewer.Attach(viewer.world_key(), points_drawable);

  viewer.Spin();
  return EXIT_SUCCESS;
}
