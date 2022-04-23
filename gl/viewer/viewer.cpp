// Copyright Igor Bogoslavskyi, year 2018.
// In case of any problems with the code please contact me.
// Email: igor.bogoslavskyi@uni-bonn.de.

#include "gl/viewer/viewer.h"
#include "gl/scene/font_pool.h"
#include "gl/scene/program_pool.h"
#include "nholthaus/units.h"

#include <functional>

namespace gl {

void SceneViewer::Initialize(const glfw::WindowSize& window_size,
                             const glfw::GlVersion& gl_version) {
  CHECK(viewer_.Initialize(window_size, gl_version));
  FontPool::Instance().LoadFont("gl/scene/fonts/ubuntu.fnt");
  opengl_initialized_ = true;

  world_key_ = graph_.RegisterBranchKey();
  viewport_key_ = graph_.RegisterBranchKey();
  camera_key_ = graph_.RegisterBranchKey();
}

void SceneViewer::Paint() {
  CHECK(opengl_initialized_);
  glClearColor(0.1, 0.1, 0.1, 0.5);
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  graph_.Draw(world_key_);
  graph_.Draw(viewport_key_);
  graph_.Draw(camera_key_);
  update_pending_ = false;
}

void SceneViewer::EraseScheduledKeys() {
  for (auto key : keys_to_remove_) { graph_.Erase(key); }
  keys_to_remove_.clear();
}

void SceneViewer::Spin() {
  viewer_.user_input_handler().RegisterMouseCallback(
      std::bind(&SceneViewer::OnMouseEvent,
                this,
                std::placeholders::_1,
                std::placeholders::_2,
                std::placeholders::_3));
  viewer_.user_input_handler().RegisterKeyboardCallback(
      std::bind(&SceneViewer::OnKeyboardEvent, this, std::placeholders::_1));
  while (!viewer_.ShouldClose()) {
    viewer_.ProcessInput();
    EraseScheduledKeys();
    Paint();
    viewer_.Spin();
  }
}

void SceneViewer::OnMouseEvent(gl::core::MouseKey key,
                               gl::core::PressState state,
                               const gl::core::PointXY& mouse_movement) {
  if (state != gl::core::PressState::kPressed) { return; }
  if (key == gl::core::MouseKey::kWheel) {
    const float multiplier = 0.5f;
    camera_.SetRadius(camera_.radius() +
                      static_cast<float>(mouse_movement.y) * multiplier);
  } else {
    const float multiplier = 0.01f;
    if (shift_pressed_) {
      camera_.Translate(
          {0.0f, 0.0f, multiplier * static_cast<float>(mouse_movement.y)});
    } else {
      camera_.Rotate(gl::Camera::RotationDirection::kHorizontal,
                     units::angle::radian_t{mouse_movement.x},
                     multiplier);
      camera_.Rotate(gl::Camera::RotationDirection::kVertical,
                     units::angle::radian_t{mouse_movement.y},
                     multiplier);
    }
  }
  UpdateCameraNodePosition();
  program_pool_.SetUniformToAllPrograms("proj_view", camera_.TfViewportWorld());
}

void SceneViewer::OnKeyboardEvent(const std::set<gl::core::KeyboardKey>& keys) {
  const float increment = 0.02f;

  if (keys.count(gl::core::KeyboardKey::kLeftShift) ||
      keys.count(gl::core::KeyboardKey::kRightShift)) {
    shift_pressed_ = true;
  } else {
    shift_pressed_ = false;
  }

  if (keys.count(gl::core::KeyboardKey::kArrowUp)) {
    camera_.Translate({shift_pressed_ ? 0.0f : -increment,
                       0.0f,
                       shift_pressed_ ? increment : 0.0f});
  }
  if (keys.count(gl::core::KeyboardKey::kArrowDown)) {
    camera_.Translate({shift_pressed_ ? 0.0f : increment,
                       0.0f,
                       shift_pressed_ ? -increment : 0.0f});
  }
  if (keys.count(gl::core::KeyboardKey::kArrowLeft)) {
    camera_.Translate({0.0f, -increment, 0.0f});
  }
  if (keys.count(gl::core::KeyboardKey::kArrowRight)) {
    camera_.Translate({0.0f, increment, 0.0f});
  }
  UpdateCameraNodePosition();
  program_pool_.SetUniformToAllPrograms("proj_view", camera_.TfViewportWorld());
}

void SceneViewer::UpdateCameraNodePosition() {
  CHECK(graph_.HasNode(camera_key_));
  graph_.GetNode(camera_key_).tf_parent_from_local() =
      camera_.tf_world_from_target();
}

}  // namespace gl
