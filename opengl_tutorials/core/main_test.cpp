#include "opengl_tutorials/ui/glfw/viewer.h"

#include "gtest/gtest.h"

#include <cstdlib>

using gl_tutorials::glfw::Viewer;

int main(int argc, char** argv) {
  setenv("DISPLAY", ":0", false);
  Viewer viewer{"TestViewer"};
  bool initialized = viewer.InitializeHidden();
  if (!initialized) { return 1; }
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
