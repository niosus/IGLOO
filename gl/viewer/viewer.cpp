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
                std::placeholders::_3));
  viewer_.user_input_handler().RegisterKeyboardCallback(
      std::bind(&SceneViewer::OnKeyboardEvent, this, std::placeholders::_1));
  program_pool_.AddProgramFromShaders(
      ProgramPool::ProgramType::DRAW_POINTS,
      {"gl/scene/shaders/points.vert", "gl/scene/shaders/simple.frag"});
  program_pool_.AddProgramFromShaders(
      ProgramPool::ProgramType::DRAW_COORDINATE_SYSTEM,
      {"gl/scene/shaders/coordinate_system.vert",
       "gl/scene/shaders/coordinate_system.geom",
       "gl/scene/shaders/simple.frag"});
  program_pool_.AddProgramFromShaders(
      ProgramPool::ProgramType::DRAW_TEXTURED_RECT,
      {"gl/scene/shaders/texture.vert",
       "gl/scene/shaders/texture.geom",
       "gl/scene/shaders/texture.frag"});
  program_pool_.AddProgramFromShaders(
      ProgramPool::ProgramType::DRAW_TEXT,
      {"gl/scene/shaders/text.vert", "gl/scene/shaders/texture.frag"});
  program_pool_.SetUniformToAllPrograms("proj_view", camera_.TfViewportWorld());
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
  while (!viewer_.ShouldClose()) {
    viewer_.ProcessInput();
    EraseScheduledKeys();
    Paint();
    viewer_.Spin();
  }
}

void SceneViewer::OnMouseEvent(gl::core::MouseKey key,
                               gl::core::PressState state,
                               const gl::core::MouseMovement& mouse_movement) {
  if (state != gl::core::PressState::kPressed) { return; }
  float modifier = 0.01f;
  camera_.Rotate(gl::Camera::RotationDirection::kHorizontal,
                 units::angle::radian_t{mouse_movement.x},
                 modifier);
  camera_.Rotate(gl::Camera::RotationDirection::kVertical,
                 units::angle::radian_t{mouse_movement.y},
                 modifier);
  UpdateCameraNodePosition();
  program_pool_.SetUniformToAllPrograms("proj_view", camera_.TfViewportWorld());
}

void SceneViewer::OnKeyboardEvent(
    const std::map<gl::core::KeyboardKey, gl::core::PressState>& keys) {
  const float increment = 0.02f;
  auto is_key_pressed = [&keys](auto key) {
    if (auto iter = keys.find(key);
        iter != keys.end() && iter->second == gl::core::PressState::kPressed) {
      return true;
    }
    return false;
  };

  bool shift_pressed{false};
  if (is_key_pressed(gl::core::KeyboardKey::kLeftShift) ||
      is_key_pressed(gl::core::KeyboardKey::kRightShift)) {
    shift_pressed = true;
  }

  if (is_key_pressed(gl::core::KeyboardKey::kArrowUp)) {
    camera_.Translate({shift_pressed ? 0.0f : -increment,
                       0.0f,
                       shift_pressed ? increment : 0.0f});
  }
  if (is_key_pressed(gl::core::KeyboardKey::kArrowDown)) {
    camera_.Translate({shift_pressed ? 0.0f : increment,
                       0.0f,
                       shift_pressed ? -increment : 0.0f});
  }
  if (is_key_pressed(gl::core::KeyboardKey::kArrowLeft)) {
    camera_.Translate({0.0f, -increment, 0.0f});
  }
  if (is_key_pressed(gl::core::KeyboardKey::kArrowRight)) {
    camera_.Translate({0.0f, increment, 0.0f});
  }
  UpdateCameraNodePosition();
  program_pool_.SetUniformToAllPrograms("proj_view", camera_.TfViewportWorld());
}

void SceneViewer::UpdateCameraNodePosition() {
  CHECK(graph_.HasNode(camera_key_));
  graph_.GetNode(camera_key_).tx_parent_local() = camera_.tf_world_target();
}

}  // namespace gl
