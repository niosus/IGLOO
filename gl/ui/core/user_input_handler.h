// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#ifndef OPENGL_TUTORIALS_GL_UI_CORE_UX_INPUT_INTERFACE_H_
#define OPENGL_TUTORIALS_GL_UI_CORE_UX_INPUT_INTERFACE_H_

#include <functional>
#include <map>

namespace gl {
namespace core {

enum class KeyboardKey {
  kNone,
  kArrowUp,
  kArrowDown,
  kArrowLeft,
  kArrowRight,
  kEscape,
  kLeftShift,
  kRightShift
};

enum class MouseKey { kNone, kLeft, kRight, kMiddle, kWheelUp, kWheelDown };

enum class PressState {
  kNone,
  kPressed,
  kReleased,
};

struct MouseMovement {
  double x;
  double y;
};

class UserInputHandler {
 public:
  void DispatchKeyboardEvent(
      const std::map<KeyboardKey, PressState>& key_states) const {
    for (auto& callback : keyboard_callbacks_) { callback(key_states); }
  }

  void DispatchMouseEvent(MouseKey key,
                          PressState state,
                          const MouseMovement& mouse_movement) const {
    for (auto& callback : mouse_callbacks_) {
      callback(key, state, mouse_movement);
    }
  }

  void RegisterKeyboardCallback(
      std::function<void(const std::map<KeyboardKey, PressState>&)> callback) {
    keyboard_callbacks_.emplace_back(callback);
  }

  void RegisterMouseCallback(
      std::function<void(MouseKey, PressState, const MouseMovement&)>
          callback) {
    mouse_callbacks_.emplace_back(callback);
  }

 protected:
  std::vector<std::function<void(const std::map<KeyboardKey, PressState>&)>>
      keyboard_callbacks_;
  std::vector<std::function<void(MouseKey, PressState, const MouseMovement&)>>
      mouse_callbacks_;
};

}  // namespace core
}  // namespace gl

#endif  // OPENGL_TUTORIALS_GL_UI_CORE_UX_INPUT_INTERFACE_H_
