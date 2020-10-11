#include "gl/utils/camera.h"
#include "gtest/gtest.h"

using namespace gl;

TEST(CameraTest, Posititon) {
  const float radius{5.0f};
  Camera camera{radius};
  Eigen::Matrix4f tf__camera__world;
  tf__camera__world << 0, 1, 0, 0,  //
      0, 0, -1, 0,                  //
      -1, 0, 0, 5,                  //
      0, 0, 0, 1;
  EXPECT_TRUE(camera.tf__camera__world().matrix().isApprox(tf__camera__world))
      << camera.tf__camera__world().matrix();
  const Eigen::Vector3f position{1, 2, 3};
  camera.SetTargetPosition(position);
  EXPECT_TRUE(camera.GetTargetPosition().isApprox(position));
}

TEST(CameraTest, CameraFromWorld) {
  Camera camera{};

  auto Check = [](const Camera& camera,
                  const Eigen::Vector4f& world_point,
                  const Eigen::Vector4f& expected_camera_point) {
    const Eigen::Vector4f camera_point =
        camera.tf__camera__world() * world_point;
    EXPECT_TRUE(camera_point.isApprox(expected_camera_point))
        << "Actual: " << camera_point.transpose()
        << "\nExpected: " << expected_camera_point.transpose();
  };

  Eigen::Vector3f camera_position;
  Eigen::Vector3f target_position;
  camera_position = {1, 0, 0};
  target_position = {0, 0, 0};
  camera.LookAt(target_position, camera_position);
  EXPECT_TRUE(camera.GetTargetPosition().isApprox(target_position))
      << camera.tf__camera__world().matrix();
  Check(camera, {0, 0, 0, 1}, {0, 0, 1, 1});
  camera_position = {2, 2, 2};
  camera.LookAt(target_position, camera_position);
  EXPECT_TRUE(camera.GetTargetPosition().isApprox(target_position))
      << camera.tf__camera__world().matrix();
  EXPECT_TRUE((camera.tf__world__camera() * Eigen::Vector3f{0.0f, 0.0f, 0.0f})
                  .isApprox(camera_position))
      << camera.tf__world__camera().matrix();
  Check(camera, {1, 1, 1, 1}, {0, 0, sqrt(3), 1});
}
