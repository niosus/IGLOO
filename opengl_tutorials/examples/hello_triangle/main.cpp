#include "opengl_tutorials/core/buffer.h"
#include "opengl_tutorials/core/program.h"
#include "opengl_tutorials/core/shader.h"
#include "opengl_tutorials/utils/eigen_utils.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <ostream>
#include <vector>

const gl_tutorials::eigen::vector<Eigen::Vector3f> vertices{
    {0.5f, 0.5f, 0.0f},   // top right
    {0.5f, -0.5f, 0.0f},  // bottom right
    {-0.5f, -0.5f, 0.0f}, // bottom left
    {-0.5f, 0.5f, 0.0f}   // top left
};
const gl_tutorials::eigen::vector<Eigen::Vector3i> indices = {
    {0, 1, 3}, // first triangle
    {1, 2, 3}  // second triangle
};

void error_callback(int error, const char *description) {
  std::cerr << "error[" << error << "]:" << description << std::endl;
}

void FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main(int argc, char const *argv[]) {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  const std::shared_ptr<gl_tutorials::Shader> vertex_shader{
      gl_tutorials::Shader::CreateFromFile(
          "opengl_tutorials/examples/hello_triangle/shaders/triangle.vert")};
  const std::shared_ptr<gl_tutorials::Shader> fragment_shader{
      gl_tutorials::Shader::CreateFromFile(
          "opengl_tutorials/examples/hello_triangle/shaders/triangle.frag")};
  if (!vertex_shader || !fragment_shader) {
    exit(EXIT_FAILURE);
  }

  const auto program = gl_tutorials::Program::CreateFromShaders(
      {vertex_shader, fragment_shader});
  if (!program) {
    std::cerr << "Failed to link program" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  std::uint32_t VAO;
  glGenVertexArrays(1, &VAO);

  // 1. bind Vertex Array Object
  glBindVertexArray(VAO);
  // 2. copy our vertices array in a vertex buffer for OpenGL to use
  gl_tutorials::Buffer vertex_buffer{gl_tutorials::Buffer::Type::kArrayBuffer,
                                     gl_tutorials::Buffer::Usage::kStaticDraw};
  vertex_buffer.AssignData(vertices);
  vertex_buffer.Bind();
  // 3. copy our index array in a element buffer for OpenGL to use
  gl_tutorials::Buffer index_buffer{
      gl_tutorials::Buffer::Type::kElementArrayBuffer,
      gl_tutorials::Buffer::Usage::kStaticDraw};
  index_buffer.AssignData(indices);
  index_buffer.Bind();
  // 4. then set the vertex attributes pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    program->Use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
