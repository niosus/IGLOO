#include "gl/ui/glfw/viewer.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

using gl::glfw::Viewer;

int main(int argc, char** argv) {
  google::InitGoogleLogging(*argv);
  Viewer viewer{"TestViewer"};
  bool initialized = viewer.InitializeHidden();
  if (!initialized) { return 1; }
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
