#ifndef OPENGL_TUTORIALS_GL_UI_GLFW_MOUSE_EVENT_HANDLER_H_
#define OPENGL_TUTORIALS_GL_UI_GLFW_MOUSE_EVENT_HANDLER_H_

#include "gl/ui/core/user_input_handler.h"

#include "GLFW/glfw3.h"

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
    if (current_clicked_) {
      input_handler_->DispatchMouseEvent(
          MouseKey::kLeft, PressState::kPressed, increment_x_, increment_y_);
    }
  }

  void CheckMove(GLFWwindow* window) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (current_clicked_) {
      if (!std::isnan(prev_x_) && !std::isnan(prev_y_)) {
        // We are at not the start of the movement, so update increment!
        increment_x_ = prev_x_ - xpos;
        increment_y_ = ypos - prev_y_;
      }
      prev_x_ = xpos;
      prev_y_ = ypos;
    }
  }

  void CheckClick(GLFWwindow* window) {
    int action = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (action == GLFW_PRESS) {
      if (current_clicked_) { return; }
      prev_x_ = std::numeric_limits<double>::quiet_NaN();
      prev_y_ = std::numeric_limits<double>::quiet_NaN();
      increment_x_ = 0.0;
      increment_y_ = 0.0;
      current_clicked_ = true;
    }
    if (action == GLFW_RELEASE) {
      if (!current_clicked_) { return; }
      current_clicked_ = false;
    }
  }

 private:
  bool AreCoordsFarEnoughToPrev(double x, double y) {
    const double min_dist = 10.0f;
    return std::abs(last_drop_x_ - x) < min_dist &&
           std::abs(last_drop_y_ - y) < min_dist;
  }

  UserInputHandler* input_handler_{nullptr};
  double increment_x_{};
  double increment_y_{};
  double prev_x_{};
  double prev_y_{};
  double last_drop_x_{};
  double last_drop_y_{};
  bool current_clicked_{};
};
}  // namespace glfw
}  // namespace gl

#endif  // OPENGL_TUTORIALS_GL_UI_GLFW_MOUSE_EVENT_HANDLER_H_
