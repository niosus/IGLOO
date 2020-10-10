#ifndef OPENGL_TUTORIALS_GL_UI_GLFW_MOUSE_EVENT_HANDLER_H_
#define OPENGL_TUTORIALS_GL_UI_GLFW_MOUSE_EVENT_HANDLER_H_

#include "GLFW/glfw3.h"
#include "gl/ui/core/user_input_handler.h"
#include "glog/logging.h"

#include <array>
#include <cmath>

namespace gl {
namespace glfw {

class UserInputHandler {
 public:
  UserInputHandler(GLFWwindow* window);

  void DispatchEventsIfNeeded(GLFWwindow* window);

  inline core::UserInputHandler& user_input_handler() {
    return user_input_handler_;
  }

 private:
  [[nodiscard]] static inline core::MouseKey MapToMouseKeyPress(
      int glfw_keypress) {
    switch (glfw_keypress) {
      case GLFW_MOUSE_BUTTON_LEFT: return core::MouseKey::kLeft;
      case GLFW_MOUSE_BUTTON_RIGHT: return core::MouseKey::kRight;
      case GLFW_MOUSE_BUTTON_MIDDLE: return core::MouseKey::kMiddle;
    }
    return core::MouseKey::kNone;
  }

  [[nodiscard]] static inline core::KeyboardKey MapToKeyboardKeyPress(
      int glfw_keypress) {
    switch (glfw_keypress) {
      case GLFW_KEY_A:
      case GLFW_KEY_LEFT: return core::KeyboardKey::kArrowLeft;
      case GLFW_KEY_D:
      case GLFW_KEY_RIGHT: return core::KeyboardKey::kArrowRight;
      case GLFW_KEY_W:
      case GLFW_KEY_UP: return core::KeyboardKey::kArrowUp;
      case GLFW_KEY_S:
      case GLFW_KEY_DOWN: return core::KeyboardKey::kArrowDown;
      case GLFW_KEY_ESCAPE: return core::KeyboardKey::kEscape;
      case GLFW_KEY_LEFT_SHIFT: return core::KeyboardKey::kLeftShift;
      case GLFW_KEY_RIGHT_SHIFT: return core::KeyboardKey::kRightShift;
    }
    return core::KeyboardKey::kNone;
  }

  [[nodiscard]] static inline core::PressState MapToPressState(
      int glfw_keypress_state) {
    switch (glfw_keypress_state) {
      case GLFW_PRESS: return core::PressState::kPressed;
      case GLFW_RELEASE: return core::PressState::kReleased;
    }
    return core::PressState::kNone;
  }

  [[nodiscard]] core::PointXY GetMouseMovementAndUpdateCurrentPosition(
      GLFWwindow* window);

  [[nodiscard]] std::optional<core::MouseKey> GetClickedMouseKey(
      GLFWwindow* window) const noexcept;

  void ScrollCallback(double x, double y);

  static void ScrollCallback(GLFWwindow* window, double x, double y);

  static std::map<GLFWwindow*, UserInputHandler*> instances_;

  core::UserInputHandler user_input_handler_;
  core::PointXY last_sent_scroll_pos_{};
  core::PointXY current_scroll_pos_{};
  std::optional<core::PointXY> prev_mouse_pos_{};
};
}  // namespace glfw
}  // namespace gl

#endif  // OPENGL_TUTORIALS_GL_UI_GLFW_MOUSE_EVENT_HANDLER_H_
