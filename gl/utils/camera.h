// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#ifndef OPENGL_TUTORIALS_GL_CORE_CAMERA_H_
#define OPENGL_TUTORIALS_GL_CORE_CAMERA_H_

#include "Eigen/Geometry"
#include "glog/logging.h"
#include "nholthaus/units.h"

#include <cmath>
#include <iostream>

#include <iostream>
#include <vector>

namespace detail {
constexpr float kMinZ{0.5f};
constexpr float kMaxZ{200.0f};
constexpr float kMaxRadius{kMaxZ - 20.0f};
}  // namespace detail

namespace gl {

/// Camera class that represents a typical computer vision camera, i.e., z -
/// forward, y - down, x - to the right. It handles transformation between such
/// a frame and a typical opengl frame in the transformation to the viewport.
class Camera {
 public:
  enum class RotationDirection { kVertical, kHorizontal };

  Camera(float radius = 5.0f,
         units::angle::radian_t horizontal_angle = units::angle::radian_t{0},
         units::angle::radian_t vertical_angle = units::angle::radian_t{0})
      : radius_{radius},
        rotation_horizontal_{horizontal_angle},
        rotation_vertical_{vertical_angle} {
    UpdateWorldCameraPosition();
  }

  Eigen::Isometry3f LookAt(
      const Eigen::Vector3f& target,
      const Eigen::Vector3f& camera_position,
      const Eigen::Vector3f& world_up = Eigen::Vector3f::UnitZ()) {
    tf__world__target_.translation() = target;
    UpdateCameraAngles(target, camera_position);
    return LookAtImpl(target, camera_position, world_up);
  }

  void SetRadius(float radius) {
    radius_ = std::min(std::max(radius, detail::kMinZ), detail::kMaxRadius);
    UpdateWorldCameraPosition();
  }

  void Rotate(RotationDirection rotation,
              units::angle::radian_t radians,
              float speed_multiplier = 1.0f) {
    switch (rotation) {
      case RotationDirection::kVertical:
        rotation_vertical_ += radians * speed_multiplier;
        rotation_vertical_ = units::math::max(
            units::math::min(rotation_vertical_, units::angle::degree_t{89.9}),
            units::angle::degree_t{-89.9});
        break;
      case RotationDirection::kHorizontal:
        rotation_horizontal_ += radians * speed_multiplier;
        break;
    }
    UpdateWorldCameraPosition();
  }

  static Eigen::Matrix4f Perspective(units::angle::radian_t fov_y,
                                     std::int32_t window_width,
                                     std::int32_t window_height,
                                     float z_near = detail::kMinZ,
                                     float z_far = detail::kMaxZ) {
    // https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml
    Eigen::Matrix4f M = Eigen::Matrix4f::Zero();
    // Copied from gluPerspective
    const float aspect_ratio =
        static_cast<float>(window_width) / static_cast<float>(window_height);
    const float f = 1.0f / std::tan(0.5f * fov_y.value());
    M(0, 0) = f / aspect_ratio;
    M(1, 1) = f;
    M(2, 2) = (z_near + z_far) / (z_near - z_far);
    M(2, 3) = (2.0f * z_far * z_near) / (z_near - z_far);
    M(3, 2) = -1.0f;
    return M;
  }

  void SetTargetPosition(const Eigen::Vector3f& position) {
    tf__world__target_.translation() = position;
    UpdateWorldCameraPosition();
  }

  void Translate(const Eigen::Vector3f& translation, float speed = 1.0f) {
    // Compute the transform from "rotated_target" to "target" frame.
    Eigen::Isometry3f tf_target_rotated(Eigen::AngleAxisf(
        rotation_horizontal_.value(), Eigen::Vector3f::UnitZ()));
    Eigen::Vector3f translation_in_target =
        tf_target_rotated * (translation * speed * radius_);
    tf__world__target_.translation() += translation_in_target;
    UpdateWorldCameraPosition();
  }

  Eigen::Vector3f GetTargetPosition() const {
    return tf__world__target_.translation();
  }

  const Eigen::Isometry3f tf__world__target() const {
    return tf__world__target_;
  }

  const Eigen::Isometry3f& tf__camera__world() const {
    return tf__camera__world_;
  }
  const Eigen::Isometry3f& tf__world__camera() const {
    return tf__world__camera_;
  }

  Eigen::Matrix4f TfViewportWorld() const {
    return tf__viewport__gl_camera *
           (tf__gl_camera__camera_ * tf__camera__world()).matrix();
  }

  Eigen::Vector3f GetCameraPositionInTargetFrame() const {
    float radius_2d = radius_ * cosf(rotation_vertical_.value());
    return {radius_2d * cosf(rotation_horizontal_.value()),
            radius_2d * sinf(rotation_horizontal_.value()),
            radius_ * sinf(rotation_vertical_.value())};
  }

  float radius() const noexcept { return radius_; }

 private:
  Eigen::Isometry3f LookAtImpl(
      const Eigen::Vector3f& target,
      const Eigen::Vector3f& camera_position,
      const Eigen::Vector3f& world_up = Eigen::Vector3f::UnitZ()) {
    Eigen::Vector3f z_direction = (target - camera_position).normalized();
    if ((z_direction - world_up).isApprox(Eigen::Vector3f::Zero())) {
      // Make sure cross product is always well-defined.
      z_direction(0) -= 0.00001;
    }
    Eigen::Vector3f x_direction = z_direction.cross(world_up).normalized();
    Eigen::Vector3f y_direction = z_direction.cross(x_direction).normalized();
    tf__world__camera_.linear() << x_direction, y_direction, z_direction;
    tf__world__camera_.translation() = camera_position;
    tf__camera__world_ = tf__world__camera_.inverse();
    return tf__camera__world_;
  }

  void UpdateCameraAngles(const Eigen::Vector3f& world_target,
                          const Eigen::Vector3f& world_camera) {
    const Eigen::Vector3f diff = world_camera - world_target;
    const float x_diff = diff.x();
    const float y_diff = diff.y();
    const float z_diff = diff.z();
    const float xy_dist = diff.topRows(2).norm();
    radius_ = diff.norm();
    rotation_horizontal_ = units::angle::radian_t{std::atan2(y_diff, x_diff)};
    rotation_vertical_ = units::angle::radian_t{std::atan2(z_diff, xy_dist)};
  }

  void UpdateWorldCameraPosition() {
    Eigen::Vector3f camera_world =
        tf__world__target_ * GetCameraPositionInTargetFrame();
    // Lookat handles also rotation of the frame from z-up coordinate system to
    // the OpenGL (z-back) coordinate system. No need for explicit
    // transformation.
    LookAtImpl(GetTargetPosition(), camera_world);
  }

  Eigen::Isometry3f tf__world__target_{Eigen::Isometry3f::Identity()};
  Eigen::Isometry3f tf__world__camera_{Eigen::Isometry3f::Identity()};
  Eigen::Isometry3f tf__camera__world_{Eigen::Isometry3f::Identity()};

  /// Rotation around x axis by M_PI brings our normal computer vision camera
  // (z - forward from the user, x - right, y - down) to the opengl coordinate
  /// system (z - backwards, towards the user, x - right, y - up).
  Eigen::Isometry3f tf__gl_camera__camera_{
      Eigen::AngleAxisf{M_PI, Eigen::Vector3f::UnitX()}};

  Eigen::Vector3f camera_pos_in_target_frame_{Eigen::Vector3f::Zero()};

  float radius_{};
  units::angle::radian_t rotation_horizontal_{};
  units::angle::radian_t rotation_vertical_{};

  Eigen::Matrix4f tf__viewport__gl_camera =
      Camera::Perspective(units::angle::degree_t{45}, 800, 600);
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_GL_CORE_CAMERA_H_
