// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "gl/ui/glfw/glfw_user_input_handler.h"

namespace gl {
namespace glfw {

namespace {
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

core::PointXY operator-(const core::PointXY& p1, const core::PointXY& p2) {
  return {p1.x - p2.x, p1.y - p2.y};
}
}  // namespace

std::map<GLFWwindow*, UserInputHandler*> UserInputHandler::instances_;

UserInputHandler::UserInputHandler(GLFWwindow* window) {
  CHECK(window);
  glfwSetScrollCallback(window, UserInputHandler::ScrollCallback);
}

void UserInputHandler::DispatchEventsIfNeeded(GLFWwindow* window) {
  if (!window) { return; }
  if (UserInputHandler::instances_.count(window) < 1) {
    UserInputHandler::instances_[window] = this;
  }
  const auto mouse_movement{GetMouseMovementAndUpdateCurrentPosition(window)};
  const auto clicked_mouse_key{GetClickedMouseKey(window)};
  if (clicked_mouse_key) {
    user_input_handler_.DispatchMouseEvent(
        clicked_mouse_key.value(), core::PressState::kPressed, mouse_movement);
  }

  user_input_handler_.DispatchMouseEvent(
      core::MouseKey::kWheel,
      core::PressState::kPressed,
      current_scroll_pos_ - last_sent_scroll_pos_);
  last_sent_scroll_pos_ = current_scroll_pos_;

  std::set<core::KeyboardKey> keys;
  for (int key : kKeyboardKeysToMOnitor) {
    if (glfwGetKey(window, key) != GLFW_PRESS) { continue; }
    keys.insert(MapToKeyboardKeyPress(key));
  }
  user_input_handler_.DispatchKeyboardEvent(keys);
}

[[nodiscard]] core::PointXY
UserInputHandler::GetMouseMovementAndUpdateCurrentPosition(GLFWwindow* window) {
  double xpos, ypos;
  core::PointXY mouse_movement{};
  glfwGetCursorPos(window, &xpos, &ypos);
  if (prev_mouse_pos_.has_value()) {
    // We are not at the start of the movement, so update increment!
    mouse_movement.x = prev_mouse_pos_->x - xpos;
    mouse_movement.y = ypos - prev_mouse_pos_->y;
  }
  prev_mouse_pos_ = {xpos, ypos};
  return mouse_movement;
}

[[nodiscard]] std::optional<core::MouseKey>
UserInputHandler::GetClickedMouseKey(GLFWwindow* window) const noexcept {
  std::optional<core::MouseKey> pressed_key{};
  for (auto glfw_button : kMouseKeysToMOnitor) {
    int action = glfwGetMouseButton(window, glfw_button);
    if (action == GLFW_PRESS) { pressed_key = MapToMouseKeyPress(glfw_button); }
  }
  return pressed_key;
}

void UserInputHandler::ScrollCallback(GLFWwindow* window, double x, double y) {
  if (UserInputHandler::instances_.count(window)) {
    auto* handler_ptr{UserInputHandler::instances_.at(window)};
    CHECK(handler_ptr);
    handler_ptr->ScrollCallback(x, y);
  }
}

void UserInputHandler::ScrollCallback(double x, double y) {
  current_scroll_pos_.x += x;
  current_scroll_pos_.y += y;
}  // namespace glfw

};  // namespace glfw
}  // namespace gl
