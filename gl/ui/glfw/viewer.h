#ifndef OPENGL_TUTORIALS_UI_GLFW_VIEWER_H_
#define OPENGL_TUTORIALS_UI_GLFW_VIEWER_H_

#include "gl/core/opengl_object.h"
#include "gl/ui/core/user_input_handler.h"

#include <GLFW/glfw3.h>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace gl {
namespace glfw {

struct WindowSize {
  std::uint32_t width;
  std::uint32_t height;
};

struct GlVersion {
  std::uint32_t major;
  std::uint32_t minor;
};

class Viewer {
 public:
  Viewer(const std::string& window_name) : window_name_{window_name} {}

  bool InitializeHidden(const WindowSize& window_size = {800, 600},
                        const GlVersion& gl_verions = {3, 3}) {
    return Initialize(window_size, gl_verions, true);
  }

  bool Initialize(const WindowSize& window_size = {800, 600},
                  const GlVersion& gl_verions = {3, 3},
                  bool hidden = false);

  Viewer(const Viewer&) = delete;
  Viewer(Viewer&&) = delete;
  Viewer& operator=(Viewer&&) = delete;
  Viewer& operator=(const Viewer&) = delete;
  ~Viewer() {
    if (window_) { glfwDestroyWindow(window_); }
    glfwTerminate();
  }

  inline void Resize(const WindowSize& window_size) {
    if (!window_) { return; }
    Viewer::OnResize(window_, window_size.width, window_size.height);
  }

  inline void ProcessInput() {
    if (!window_) { return; }
    for (int key : keys_to_monitor_) {
      if (glfwGetKey(window_, key) != GLFW_PRESS) { continue; }
      user_input_handler_.DispatchKeyboardEvent(MapToKeyPress(key),
                                                MapToPressState(GLFW_PRESS));
    }
  }

  inline void Spin() {
    if (!window_) { return; }
    glfwPollEvents();
    glfwSwapBuffers(window_);
  }

  inline bool ShouldClose() const { return glfwWindowShouldClose(window_); }

  inline float GetTime() const { return glfwGetTime(); }

  inline const WindowSize& window_size() const { return window_size_; }

  inline UserInputHandler& user_input_handler() { return user_input_handler_; }

 private:
  static inline KeyboardKey MapToKeyPress(int glfw_keypress) {
    switch (glfw_keypress) {
      case GLFW_KEY_LEFT: return KeyboardKey::kArrowLeft;
      case GLFW_KEY_RIGHT: return KeyboardKey::kArrowRight;
      case GLFW_KEY_UP: return KeyboardKey::kArrowUp;
      case GLFW_KEY_DOWN: return KeyboardKey::kArrowDown;
      case GLFW_KEY_ESCAPE: return KeyboardKey::kEscape;
    }
    return KeyboardKey::kNone;
  }

  static inline PressState MapToPressState(int glfw_keypress_state) {
    switch (glfw_keypress_state) {
      case GLFW_PRESS: return PressState::kPressed;
      case GLFW_RELEASE: return PressState::kReleased;
    }
    return PressState::kNone;
  }

  static inline void OnResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  }

  bool initialized_{};
  std::string window_name_{};
  GLFWwindow* window_{};
  WindowSize window_size_{};

  std::vector<int> keys_to_monitor_{GLFW_KEY_ESCAPE,
                                    GLFW_KEY_LEFT,
                                    GLFW_KEY_RIGHT,
                                    GLFW_KEY_UP,
                                    GLFW_KEY_DOWN};

  UserInputHandler user_input_handler_;
};

}  // namespace glfw
}  // namespace gl

#endif  // OPENGL_TUTORIALS_UI_GLFW_VIEWER_H_
