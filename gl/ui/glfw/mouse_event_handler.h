#ifndef OPENGL_TUTORIALS_GL_UI_GLFW_MOUSE_EVENT_HANDLER_H_
#define OPENGL_TUTORIALS_GL_UI_GLFW_MOUSE_EVENT_HANDLER_H_

#include "gl/ui/core/user_input_handler.h"

#include "GLFW/glfw3.h"

#include <iostream>

namespace gl {
namespace glfw {
class MouseEventHandler {
 public:
  MouseEventHandler(UserInputHandler* input_handler)
      : input_handler_{input_handler} {}

  void DispatchEventIfNeeded() {
    if (!input_handler_) { return; }
    if (current_clicked_ && prev_clicked_) {
      input_handler_->DispatchMouseEvent(
          MouseKey::kLeft, PressState::kPressed, increment_x_, increment_y_);
    }
  }

  void CheckMove(GLFWwindow* window) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (current_clicked_ && prev_clicked_) {
      increment_x_ = prev_x_ - xpos;
      increment_y_ = ypos - prev_y_;
      prev_x_ = xpos;
      prev_y_ = ypos;
    }
  }

  void CheckClick(GLFWwindow* window) {
    int action = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (action == GLFW_PRESS) {
      if (prev_clicked_) {
        current_clicked_ = true;
      } else {
        prev_clicked_ = true;
      }
    }
    if (action == GLFW_RELEASE) {
      prev_clicked_ = false;
      current_clicked_ = false;
    }
  }

 private:
  UserInputHandler* input_handler_{nullptr};
  double increment_x_{};
  double increment_y_{};
  double prev_x_{};
  double prev_y_{};
  bool prev_clicked_{};
  bool current_clicked_{};
};
}  // namespace glfw
}  // namespace gl

#endif  // OPENGL_TUTORIALS_GL_UI_GLFW_MOUSE_EVENT_HANDLER_H_
