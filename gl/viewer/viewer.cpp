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
  viewer_.user_input_handler().RegisterMouseCallback(
      std::bind(&SceneViewer::OnMouseEvent,
                this,
                std::placeholders::_1,
                std::placeholders::_2,
                std::placeholders::_3,
                std::placeholders::_4));
  viewer_.user_input_handler().RegisterKeyboardCallback(
      std::bind(&SceneViewer::OnKeyboardEvent,
                this,
                std::placeholders::_1,
                std::placeholders::_2));
  ProgramPool::CreateAllPrograms();
  FontPool::Instance().LoadFont("gl/scene/fonts/ubuntu.fnt");
  ProgramPool::SetUniform("proj_view", camera_.TfViewportWorld());
  opengl_initialized_ = true;
}

void SceneViewer::Paint() {
  CHECK(opengl_initialized_);
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
  while (!viewer_.ShouldClose()) {
    viewer_.ProcessInput();
    EraseScheduledKeys();
    Paint();
    viewer_.Spin();
  }
}

void SceneViewer::OnMouseEvent(gl::MouseKey key,
                               gl::PressState state,
                               float x_increment,
                               float y_increment) {
  if (state != gl::PressState::kPressed) { return; }
  float modifier = 0.01f;
  camera_.Rotate(gl::Camera::RotationDirection::kHorizontal,
                 units::angle::radian_t{x_increment},
                 modifier);
  camera_.Rotate(gl::Camera::RotationDirection::kVertical,
                 units::angle::radian_t{y_increment},
                 modifier);
  UpdateCameraNodePosition();
  ProgramPool::SetUniform("proj_view", camera_.TfViewportWorld());
}

void SceneViewer::OnKeyboardEvent(gl::KeyboardKey key, gl::PressState state) {
  if (state != gl::PressState::kPressed) { return; }
  const float increment = 0.02f;
  switch (key) {
    case gl::KeyboardKey::kArrowUp:
      camera_.Translate({-increment, 0.0f, 0.0f});
      break;
    case gl::KeyboardKey::kArrowDown:
      camera_.Translate({increment, 0.0f, 0.0f});
      break;
    case gl::KeyboardKey::kArrowLeft:
      camera_.Translate({0.0f, -increment, 0.0f});
      break;
    case gl::KeyboardKey::kArrowRight:
      camera_.Translate({0.0f, increment, 0.0f});
      break;
    default: return;
  }
  UpdateCameraNodePosition();
  ProgramPool::SetUniform("proj_view", camera_.TfViewportWorld());
}

void SceneViewer::UpdateCameraNodePosition() {
  CHECK(graph_.HasNode(camera_key_));
  graph_.GetNode(camera_key_).tx_parent_local() = camera_.tf_world_target();
}

}  // namespace gl
