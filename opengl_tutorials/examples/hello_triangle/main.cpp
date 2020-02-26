#include "opengl_tutorials/utils/shader_loader.h"
#include "third_party/glad/glad.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

const float vertices[] = {
    0.5f,  0.5f,  0.0f, // top right
    0.5f,  -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f,  0.0f  // top left
};
const unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

void FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

std::vector<std::uint32_t> CreateShaders() {
  std::uint32_t vertex_shader_id;
  vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  const auto vertex_shader_source = ShaderLoader::ReadShader(
      "opengl_tutorials/examples/hello_triangle/shaders/triangle.vert");
  if (!vertex_shader_source) {
    std::cerr << "Cannot load vertex shader!" << std::endl;
    exit(1);
  }
  const char *vert_shader_data = vertex_shader_source->data();
  glShaderSource(vertex_shader_id, 1, &vert_shader_data, nullptr);
  glCompileShader(vertex_shader_id);
  int success;
  char info_log[512];
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader_id, 512, nullptr, info_log);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << info_log << std::endl;
  }

  std::uint32_t fragment_shader_id;
  fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
  const auto fragment_shader_source = ShaderLoader::ReadShader(
      "opengl_tutorials/examples/hello_triangle/shaders/triangle.frag");
  if (!fragment_shader_source) {
    std::cerr << "Cannot load fragment shader!" << std::endl;
    exit(1);
  }
  const char *frag_shader_data = fragment_shader_source->c_str();
  glShaderSource(fragment_shader_id, 1, &frag_shader_data, nullptr);
  glCompileShader(fragment_shader_id);
  glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader_id, 512, nullptr, info_log);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << info_log << std::endl;
  }
  return {vertex_shader_id, fragment_shader_id};
}

std::uint32_t CreateProgram(const std::vector<std::uint32_t> &shader_ids) {
  char info_log[512];
  int success = 0;
  std::uint32_t shader_program_id;
  shader_program_id = glCreateProgram();
  for (auto shader_id : shader_ids) {
    glAttachShader(shader_program_id, shader_id);
  }
  glLinkProgram(shader_program_id);
  glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program_id, 512, nullptr, info_log);
    std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
  }
  return shader_program_id;
}

int main(int argc, char const *argv[]) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window =
      glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  auto shader_program_id = CreateProgram(CreateShaders());

  std::uint32_t VBO;
  glGenBuffers(1, &VBO);

  std::uint32_t EBO;
  glGenBuffers(1, &EBO);

  std::uint32_t VAO;
  glGenVertexArrays(1, &VAO);

  // 1. bind Vertex Array Object
  glBindVertexArray(VAO);
  // 2. copy our vertices array in a vertex buffer for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 3. copy our index array in a element buffer for OpenGL to use
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  // 4. then set the vertex attributes pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    ProcessInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program_id);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
