#ifndef OPENGL_TUTORIALS_UI_GLFW_VIEWER_H_
#define OPENGL_TUTORIALS_UI_GLFW_VIEWER_H_

#include "gl/core/gl_base.h"

#include <GLFW/glfw3.h>

#include <memory>
#include <string>

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
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window_, true);
  }

  inline void Spin() {
    if (!window_) { return; }
    glfwPollEvents();
    glfwSwapBuffers(window_);
  }

  inline bool ShouldClose() const { return glfwWindowShouldClose(window_); }

  inline float GetTime() const { return glfwGetTime(); }

 private:
  static inline void OnResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  }

  bool initialized_{};
  std::string window_name_{};
  GLFWwindow* window_{};
};

}  // namespace glfw
}  // namespace gl

#endif  // OPENGL_TUTORIALS_UI_GLFW_VIEWER_H_
