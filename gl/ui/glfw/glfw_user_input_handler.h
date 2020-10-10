#ifndef OPENGL_TUTORIALS_GL_UI_GLFW_MOUSE_EVENT_HANDLER_H_
#define OPENGL_TUTORIALS_GL_UI_GLFW_MOUSE_EVENT_HANDLER_H_

#include "gl/ui/core/user_input_handler.h"

#include "GLFW/glfw3.h"

#include "glog/logging.h"

#include <array>
#include <cmath>

namespace gl {
namespace glfw {

namespace detail {
constexpr std::array kKeyboardKeysToMOnitor{GLFW_KEY_ESCAPE,
                                            GLFW_KEY_LEFT,
                                            GLFW_KEY_RIGHT,
                                            GLFW_KEY_UP,
                                            GLFW_KEY_DOWN,
                                            GLFW_KEY_A,
                                            GLFW_KEY_D,
                                            GLFW_KEY_W,
                                            GLFW_KEY_S,
                                            GLFW_KEY_LEFT_SHIFT,
                                            GLFW_KEY_RIGHT_SHIFT};
constexpr std::array kMouseKeysToMOnitor{
    GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOUSE_BUTTON_MIDDLE};
}  // namespace detail

class UserInputHandler {
 public:
  UserInputHandler() = default;

  void DispatchEventsIfNeeded(GLFWwindow* window) {
    if (!window) { return; }
    const auto mouse_movement{GetMouseMovementAndUpdateCurrentPosition(window)};
    const auto clicked_mouse_key{GetClickedMouseKey(window)};
    if (clicked_mouse_key) {
      user_input_handler_.DispatchMouseEvent(clicked_mouse_key.value(),
                                             core::PressState::kPressed,
                                             mouse_movement);
    }
    std::map<core::KeyboardKey, core::PressState> keys;
    for (int key : detail::kKeyboardKeysToMOnitor) {
      if (glfwGetKey(window, key) != GLFW_PRESS) { continue; }
      keys[MapToKeyboardKeyPress(key)] = MapToPressState(GLFW_PRESS);
    }
    user_input_handler_.DispatchKeyboardEvent(keys);
  }

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

  [[nodiscard]] core::MouseMovement GetMouseMovementAndUpdateCurrentPosition(
      GLFWwindow* window) {
    double xpos, ypos;
    core::MouseMovement mouse_movement{};
    glfwGetCursorPos(window, &xpos, &ypos);
    if (!std::isnan(prev_x_) && !std::isnan(prev_y_)) {
      // We are not at the start of the movement, so update increment!
      mouse_movement.x = prev_x_ - xpos;
      mouse_movement.y = ypos - prev_y_;
    }
    prev_x_ = xpos;
    prev_y_ = ypos;
    return mouse_movement;
  }

  [[nodiscard]] std::optional<core::MouseKey> GetClickedMouseKey(
      GLFWwindow* window) const noexcept {
    std::optional<core::MouseKey> pressed_key{};
    for (auto glfw_button : detail::kMouseKeysToMOnitor) {
      int action = glfwGetMouseButton(window, glfw_button);
      if (action == GLFW_PRESS) {
        pressed_key = MapToMouseKeyPress(glfw_button);
      }
    }
    return pressed_key;
  }

  core::UserInputHandler user_input_handler_;
  double prev_x_{std::numeric_limits<double>::quiet_NaN()};
  double prev_y_{std::numeric_limits<double>::quiet_NaN()};
};
}  // namespace glfw
}  // namespace gl

#endif  // OPENGL_TUTORIALS_GL_UI_GLFW_MOUSE_EVENT_HANDLER_H_
