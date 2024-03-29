// Copyright Igor Bogoslavskyi, year 2018.
// In case of any problems with the code please contact me.
// Email: igor.bogoslavskyi@uni-bonn.de.

#ifndef OPENGL_TUTORIALS_GL_VIEWER_VIEWER_H_
#define OPENGL_TUTORIALS_GL_VIEWER_VIEWER_H_

#include "gl/scene/scene_graph.h"
#include "gl/ui/glfw/viewer.h"
#include "gl/utils/camera.h"

#include <string>
#include <vector>

namespace gl {

/// A viewe that is a Qt window that we use to show our OpneGL context.
class SceneViewer {
 public:
  SceneViewer(const std::string& window_name) : viewer_{window_name} {}

  void Initialize(const glfw::WindowSize& window_size = {800, 600},
                  const glfw::GlVersion& gl_version = {4, 3});

  /// Remove the drawable with a given key from the graph upon the next Gl
  /// update. We must do it from the Gl thread, otherwise we have memory
  /// corruption.
  inline void ScheduleKeyRemoval(gl::SceneGraph::Key key) {
    keys_to_remove_.emplace_back(key);
  }

  /// Attach a new drawable to a node in the scene graph.
  inline gl::SceneGraph::Key Attach(
      gl::SceneGraph::Key parent_key,
      gl::Drawable::SharedPtr drawable,
      const Eigen::Isometry3f& tf_parent_from_local =
          Eigen::Isometry3f::Identity()) {
    return graph_.Attach(parent_key, drawable, tf_parent_from_local);
  }

  /// Attach a new drawable to a parent within the screen space with a relative
  /// transformation from child to parent.
  inline gl::SceneGraph::Key AttachToScreen(
      gl::Drawable::SharedPtr drawable,
      const Eigen::Vector2f& tf_parent_from_local = Eigen::Vector2f::Zero()) {
    Eigen::Isometry3f position_isometry{Eigen::Isometry3f::Identity()};
    position_isometry.translation().x() = tf_parent_from_local.x();
    position_isometry.translation().y() = tf_parent_from_local.y();
    return Attach(viewport_key(), drawable, position_isometry);
  }

  gl::Camera& camera() { return camera_; }

  void OnMouseEvent(gl::core::MouseKey key,
                    gl::core::PressState state,
                    const gl::core::PointXY& mouse_movement);

  void OnKeyboardEvent(const std::set<gl::core::KeyboardKey>& keys);

  const inline gl::SceneGraph::Key& world_key() const { return world_key_; }
  const inline gl::SceneGraph::Key& viewport_key() const {
    return viewport_key_;
  }
  const inline gl::SceneGraph::Key& camera_key() const { return camera_key_; }

  void Spin();

  const ProgramPool& program_pool() const noexcept { return program_pool_; }
  ProgramPool& program_pool() noexcept { return program_pool_; }

 protected:
  void Paint();

  /// Update the current position of the camera node from the current position
  /// of the camera to correctly draw all the nodes attached to it.
  void UpdateCameraNodePosition();

  /// Called from gui thread, this actually erases drawables from the graph.
  void EraseScheduledKeys();

  /// The underlying viewer.
  glfw::Viewer viewer_;

  /// Program pool to use to create all drawables.
  ProgramPool program_pool_;

  /// A convenience bool to not cause multiple redraws for queued updates.
  bool update_pending_;
  /// As we cannot init opengl in constructor, we have to check if it is
  /// initialized before using any functionality from it.
  bool opengl_initialized_;

  /// Detect if shift is pressed.
  bool shift_pressed_{};

  /// An instance of the camera that we are using here.
  gl::Camera camera_;

  /// Scene graph that holds all drawables.
  gl::SceneGraph graph_;
  gl::SceneGraph::Key world_key_;
  gl::SceneGraph::Key viewport_key_;
  gl::SceneGraph::Key camera_key_;

  std::vector<gl::SceneGraph::Key> keys_to_remove_;
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_GL_VIEWER_VIEWER_H_
