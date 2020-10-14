// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "gl/ui/glfw/viewer.h"
#include "gl/core/init.h"
#include "glog/logging.h"

#include "imgui.h"

#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include <iostream>

namespace gl {
namespace glfw {

namespace {
void ErrorCallback(int error, const char* description) {
  LOG(FATAL) << "error[" << error << "]: " << description;
}
}  // namespace

bool Viewer::Initialize(const WindowSize& window_size,
                        const GlVersion& gl_verions,
                        bool hidden) {
  glfwSetErrorCallback(ErrorCallback);
  glfwInitHint(GLFW_COCOA_MENUBAR, GLFW_FALSE);
  if (!glfwInit()) { return false; }
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

  InitializeGlContext(glfwGetProcAddress);

  glfwSetFramebufferSizeCallback(window_, Viewer::OnResize);
  Resize(window_size);

  user_input_handler_ = UserInputHandler{window_};
  user_input_handler_->user_input_handler().RegisterKeyboardCallback(
      [this](const std::set<core::KeyboardKey>& keys) {
        if (!keys.count(core::KeyboardKey::kEscape)) { return; }
        glfwSetWindowShouldClose(window_, true);
      });

  window_size_ = window_size;

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init("#version 330");
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  initialized_ = true;
  return true;
}

}  // namespace glfw
}  // namespace gl
