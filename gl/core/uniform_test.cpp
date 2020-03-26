#include "gl/core/uniform.h"
#include "gl/ui/glfw/viewer.h"
#include "gl/utils/eigen_traits.h"
#include "utils/eigen_utils.h"
#include "gtest/gtest.h"

using namespace gl;

TEST(UniformTest, Init) {
  Uniform uniform{"some_name", 0};
  EXPECT_EQ("some_name", uniform.name());
  EXPECT_NE(0u, uniform.id());
}

TEST(UniformTest, UpdateValueFromPack) {
  Uniform uniform{"some_name", 0};
  uniform.UpdateValue(1.0f);
  uniform.UpdateValue(1.0f, 2.0f);
  uniform.UpdateValue(1.0f, 2.0f, 3.0f);
  uniform.UpdateValue(1.0f, 2.0f, 3.0f, 4.0f);
}

// TEST(UniformTest, UpdateValueFromPackOfIntegers) {
//   Uniform uniform{"some_name", 0};
//   uniform.UpdateValue(1);
//   uniform.UpdateValue(1, 2);
//   uniform.UpdateValue(1, 2, 3);
//   uniform.UpdateValue(1, 2, 3, 4);
// }

TEST(UniformTest, UpdateValueFromArray) {
  Uniform uniform{"some_name", 0};
  // 2 elements
  uniform.UpdateValue(eigen::vector<Eigen::Vector2f>{{1.0f, 2.0f}});
  uniform.UpdateValue(
      eigen::vector<Eigen::Vector2f>{{1.0f, 2.0f}, {2.0f, 3.0f}});
  // 3 elements
  uniform.UpdateValue(eigen::vector<Eigen::Vector3f>{{1.0f, 2.0f, 3.0f}});
  uniform.UpdateValue(
      eigen::vector<Eigen::Vector3f>{{1.0f, 2.0f, 3.0f}, {1.0f, 2.0f, 3.0f}});
  uniform.UpdateValue(eigen::vector<Eigen::Vector3f>(10, {1.0f, 2.0f, 3.0f}));
  // 4 elements
  uniform.UpdateValue(eigen::vector<Eigen::Vector4f>{{1.0f, 2.0f, 3.0f, 4.0f}});
  uniform.UpdateValue(
      eigen::vector<Eigen::Vector4f>(10, {1.0f, 2.0f, 3.0f, 4.0f}));
}
