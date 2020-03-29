#include "gl/ui/glfw/viewer.h"

#include <iostream>

namespace gl {
namespace glfw {

namespace {
void ErrorCallback(int error, const char* description) {
  std::cerr << "error[" << error << "]:" << description << std::endl;
}
}  // namespace

bool Viewer::Initialize(const WindowSize& window_size,
                        const GlVersion& gl_verions,
                        bool hidden) {
  glfwSetErrorCallback(ErrorCallback);
  if (!glfwInit()) { return false; }

  glfwInitHint(GLFW_COCOA_MENUBAR, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_verions.major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_verions.minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  if (hidden) { glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); }

  window_ = glfwCreateWindow(window_size.width,
                             window_size.height,
                             window_name_.c_str(),
                             nullptr,
                             nullptr);
  if (!window_) { return false; }
  glfwMakeContextCurrent(window_);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return false;
  }

  glfwSetFramebufferSizeCallback(window_, Viewer::OnResize);
  Resize(window_size);

  key_press_handlers[KeyPress::kEscape] = [this](KeyPress) {
    glfwSetWindowShouldClose(window_, true);
  };

  window_size_ = window_size;
  initialized_ = true;
  return true;
}

}  // namespace glfw
}  // namespace gl
