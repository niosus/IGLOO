#include "absl/strings/str_format.h"
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
  viewer.Spin();
  return EXIT_SUCCESS;
}
