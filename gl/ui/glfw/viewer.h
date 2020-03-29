#ifndef OPENGL_TUTORIALS_UI_GLFW_VIEWER_H_
#define OPENGL_TUTORIALS_UI_GLFW_VIEWER_H_

#include "gl/core/opengl_object.h"

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

enum class KeyPress {
  kUnknown,
  kArrowUp,
  kArrowDown,
  kArrowLeft,
  kArrowRight,
  kEscape
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
      auto key_from_enum = MapToKeyPress(key);
      const auto& callback_iter = key_press_handlers.find(key_from_enum);
      if (callback_iter == key_press_handlers.end()) { continue; }
      callback_iter->second(key_from_enum);
    }
  }

  inline void Spin() {
    if (!window_) { return; }
    glfwPollEvents();
    glfwSwapBuffers(window_);
  }

  inline bool ShouldClose() const { return glfwWindowShouldClose(window_); }

  inline float GetTime() const { return glfwGetTime(); }

  inline void RegisterKeyPressCallback(
      KeyPress key_press, std::function<void(KeyPress)> key_press_handler) {
    key_press_handlers[key_press] = key_press_handler;
  }

  inline const WindowSize& window_size() const { return window_size_; }

 private:
  static inline KeyPress MapToKeyPress(int glfw_keypress) {
    switch (glfw_keypress) {
      case GLFW_KEY_LEFT: return KeyPress::kArrowLeft;
      case GLFW_KEY_RIGHT: return KeyPress::kArrowRight;
      case GLFW_KEY_UP: return KeyPress::kArrowUp;
      case GLFW_KEY_DOWN: return KeyPress::kArrowDown;
      case GLFW_KEY_ESCAPE: return KeyPress::kEscape;
    }
    return KeyPress::kUnknown;
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

  std::map<KeyPress, std::function<void(KeyPress)>> key_press_handlers;
};

}  // namespace glfw
}  // namespace gl

#endif  // OPENGL_TUTORIALS_UI_GLFW_VIEWER_H_
