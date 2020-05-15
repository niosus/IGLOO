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
                  const glfw::GlVersion& gl_version = {3, 3});

  /// Remove the drawable with a given key from the graph upon the next Gl
  /// update. We must do it from the Gl thread, otherwise we have memory
  /// corruption.
  inline void ScheduleKeyRemoval(gl::SceneGraph::Key key) {
    keys_to_remove_.emplace_back(key);
  }

  /// Attach a new drawable to a node in the scene graph.
  inline gl::SceneGraph::Key Attach(gl::SceneGraph::Key parent_key,
                                    gl::Drawable::SharedPtr drawable,
                                    const Eigen::Isometry3f& tx_parent_child =
                                        Eigen::Isometry3f::Identity()) {
    auto new_key = graph_.Attach(parent_key, drawable, tx_parent_child);
    return new_key;
  }

  void OnMouseEvent(gl::MouseKey key,
                    gl::PressState state,
                    float x_increment,
                    float y_increment);

  void OnKeyboardEvent(gl::KeyboardKey key, gl::PressState state);

  const inline gl::SceneGraph::Key& world_key() const { return world_key_; }
  const inline gl::SceneGraph::Key& viewport_key() const {
    return viewport_key_;
  }
  const inline gl::SceneGraph::Key& camera_key() const { return camera_key_; }

  void Spin();

 protected:
  /// The underlying viewer.
  glfw::Viewer viewer_;

  /// A convenience bool to not cause multiple redraws for queued updates.
  bool update_pending_;
  /// As we cannot init opengl in constructor, we have to check if it is
  /// initialized before using any functionality from it.
  bool opengl_initialized_;

  void Paint();

  /// Update the current position of the camera node from the current position
  /// of the camera to correctly draw all the nodes attached to it.
  void UpdateCameraNodePosition();

  /// Called from gui thread, this actually erases drawables from the graph.
  void EraseScheduledKeys();

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
