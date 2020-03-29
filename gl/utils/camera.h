// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#ifndef OPENGL_TUTORIALS_GL_CORE_CAMERA_H_
#define OPENGL_TUTORIALS_GL_CORE_CAMERA_H_

#include <Eigen/Geometry>

#include <cmath>
#include <iostream>

namespace gl {

class Camera {
 public:
  Eigen::Affine3f LookAt(
      const Eigen::Vector3f& target,
      const Eigen::Vector3f& camera_position,
      const Eigen::Vector3f& world_up = Eigen::Vector3f::UnitZ()) {
    Eigen::Vector3f z_direction = (camera_position - target).normalized();
    if ((z_direction - world_up).isApprox(Eigen::Vector3f::Zero())) {
      // Make sure cross product is always well-defined.
      z_direction(0) -= 0.00001;
    }
    Eigen::Vector3f x_direction = world_up.cross(z_direction).normalized();
    Eigen::Vector3f y_direction = z_direction.cross(x_direction).normalized();
    tf_world_camera_.linear() << x_direction, y_direction, z_direction;
    tf_world_camera_.translation() = camera_position;
    return tf_world_camera_.inverse();
  }

  Eigen::Matrix4f Perspective(float fov_y,
                              float window_width,
                              float window_height,
                              float z_near,
                              float z_far) {
    // https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml
    Eigen::Matrix4f M = Eigen::Matrix4f::Zero();
    // Copied from gluPerspective
    float f = 1.0f / std::tan(0.5f * fov_y);
    M(0, 0) = f / (window_width / window_height);
    M(1, 1) = f;
    M(2, 2) = (z_near + z_far) / (z_near - z_far);
    M(2, 3) = (2.0f * z_far * z_near) / (z_near - z_far);
    M(3, 2) = -1.0f;
    return M;
  }

  void SetPosition(const Eigen::Vector3f& position) {
    tf_world_camera_.translation() = position;
  }

  Eigen::Vector3f GetPosition() const { return tf_world_camera_.translation(); }

  Eigen::Affine3f tf_camera_world() const { return tf_world_camera_.inverse(); }
  const Eigen::Affine3f& tf_world_camera() const { return tf_world_camera_; }

 private:
  Eigen::Affine3f tf_world_camera_{Eigen::Affine3f::Identity()};
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_GL_CORE_CAMERA_H_
