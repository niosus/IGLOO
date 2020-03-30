// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#ifndef OPENGL_TUTORIALS_GL_UI_CORE_UX_INPUT_INTERFACE_H_
#define OPENGL_TUTORIALS_GL_UI_CORE_UX_INPUT_INTERFACE_H_

#include <functional>

namespace gl {

enum class KeyboardKey {
  kNone,
  kArrowUp,
  kArrowDown,
  kArrowLeft,
  kArrowRight,
  kEscape
};

enum class MouseKey { kNone, kLeft, kRight, kMiddle, kWheelUp, kWheelDown };

enum class PressState {
  kNone,
  kPressed,
  kReleased,
};

class UserInputHandler {
 public:
  void DispatchKeyboardEvent(KeyboardKey key, PressState state) {
    for (auto& callback : keyboard_callbacks_) { callback(key, state); }
  }

  void DispatchMouseEvent(MouseKey key, PressState state, float increment) {
    for (auto& callback : mouse_callbacks_) { callback(key, state, increment); }
  }

  void RegisterKeyboardCallback(
      std::function<void(KeyboardKey, PressState)> callback) {
    keyboard_callbacks_.emplace_back(std::move(callback));
  }

  void RegisterMouseCallback(
      std::function<void(MouseKey, PressState, float)> callback) {
    mouse_callbacks_.emplace_back(std::move(callback));
  }

 protected:
  std::vector<std::function<void(KeyboardKey, PressState)>> keyboard_callbacks_;
  std::vector<std::function<void(MouseKey, PressState, float)>>
      mouse_callbacks_;
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_GL_UI_CORE_UX_INPUT_INTERFACE_H_
