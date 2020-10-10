#ifndef OPENGL_TUTORIALS_GL_UI_GLFW_MOUSE_EVENT_HANDLER_H_
#define OPENGL_TUTORIALS_GL_UI_GLFW_MOUSE_EVENT_HANDLER_H_

#include "gl/ui/core/user_input_handler.h"

#include "GLFW/glfw3.h"

#include "glog/logging.h"

#include <cmath>
#include <iostream>

namespace gl {
namespace glfw {
class MouseEventHandler {
 public:
  MouseEventHandler(UserInputHandler* input_handler)
      : input_handler_{input_handler} {}

  void DispatchEventIfNeeded() {
    if (!input_handler_) { return; }
    if (currently_pressed_mouse_key_) {
      input_handler_->DispatchMouseEvent(currently_pressed_mouse_key_.value(),
                                         PressState::kPressed,
                                         {increment_x_, increment_y_});
    }
  }

  void CheckMove(GLFWwindow* window) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (!std::isnan(prev_x_) && !std::isnan(prev_y_)) {
      // We are at not the start of the movement, so update increment!
      increment_x_ = prev_x_ - xpos;
      increment_y_ = ypos - prev_y_;
    }
    prev_x_ = xpos;
    prev_y_ = ypos;
  }

  void CheckClick(GLFWwindow* window) {
    for (auto glfw_button : {GLFW_MOUSE_BUTTON_LEFT,
                             GLFW_MOUSE_BUTTON_RIGHT,
                             GLFW_MOUSE_BUTTON_MIDDLE}) {
      int action = glfwGetMouseButton(window, glfw_button);
      if (currently_pressed_mouse_key_.has_value() && action == GLFW_RELEASE &&
          MapToKeyPress(glfw_button) == currently_pressed_mouse_key_.value()) {
        currently_pressed_mouse_key_.reset();
        continue;
      }
      if (action == GLFW_PRESS) {
        if (currently_pressed_mouse_key_) { continue; }
        currently_pressed_mouse_key_ = MapToKeyPress(glfw_button);
      }
    }
  }

 private:
  static inline MouseKey MapToKeyPress(int glfw_keypress) {
    switch (glfw_keypress) {
      case GLFW_MOUSE_BUTTON_LEFT: return MouseKey::kLeft;
      case GLFW_MOUSE_BUTTON_RIGHT: return MouseKey::kRight;
      case GLFW_MOUSE_BUTTON_MIDDLE: return MouseKey::kMiddle;
    }
    return MouseKey::kNone;
  }

  UserInputHandler* input_handler_{nullptr};
  double increment_x_{};
  double increment_y_{};
  double prev_x_{std::numeric_limits<double>::quiet_NaN()};
  double prev_y_{std::numeric_limits<double>::quiet_NaN()};
  std::optional<MouseKey> currently_pressed_mouse_key_{};
};
}  // namespace glfw
}  // namespace gl

#endif  // OPENGL_TUTORIALS_GL_UI_GLFW_MOUSE_EVENT_HANDLER_H_
