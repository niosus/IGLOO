#include "gl/utils/camera.h"
#include "gtest/gtest.h"

using namespace gl;

TEST(CameraTest, Posititon) {
  Camera camera{};
  EXPECT_TRUE(camera.tf_camera_world().isApprox(Eigen::Affine3f::Identity()));
  const Eigen::Vector3f position{1, 2, 3};
  camera.SetPosition(position);
  EXPECT_TRUE(camera.GetPosition().isApprox(position));
}

TEST(CameraTest, CameraFromWorld) {
  Camera camera{};

  auto Check = [](const Camera& camera,
                  const Eigen::Vector4f& world_point,
                  const Eigen::Vector4f& expected_camera_point) {
    const Eigen::Vector4f camera_point = camera.tf_camera_world() * world_point;
    EXPECT_TRUE(camera_point.isApprox(expected_camera_point))
        << "Actual: " << camera_point.transpose()
        << "\nExpected: " << expected_camera_point.transpose();
  };

  Eigen::Vector3f position;
  position = {1, 0, 0};
  camera.LookAt({0, 0, 0}, position);
  EXPECT_TRUE(camera.GetPosition().isApprox(position))
      << camera.tf_camera_world().matrix();
  Check(camera, {0, 0, 0, 1}, {0, 0, 1, 1});
  position = {2, 2, 2};
  camera.LookAt({0, 0, 0}, position);
  EXPECT_TRUE(camera.GetPosition().isApprox(position))
      << camera.tf_camera_world().matrix();
  Check(camera, {1, 1, 1, 1}, {0, 0, sqrt(3), 1});
}
