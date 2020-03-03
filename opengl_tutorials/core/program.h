#ifndef OPENGL_TUTORIALS_CORE_PROGRAM_H_
#define OPENGL_TUTORIALS_CORE_PROGRAM_H_

#include "opengl_tutorials/core/gl_base.h"
#include "opengl_tutorials/core/shader.h"

#include <cstdint>

namespace gl_tutorials {

class Program : public OpenGlObject {
 public:
  Program() : OpenGlObject{glCreateProgram()} {}

  inline void AttachShader(const std::shared_ptr<Shader> &shader) {
    glAttachShader(id_, shader->id());
    attached_shaders_.push_back(shader);
  }
  inline void AttachShaders(
      const std::vector<std::shared_ptr<Shader>> &shaders) {
    attached_shaders_.reserve(attached_shaders_.size() + shaders.size());
    for (const auto &shader : shaders) { AttachShader(shader); }
  }
  bool Link() const {
    glLinkProgram(id_);
    char info_log[512];
    int success = 0;
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(id_, 512, nullptr, info_log);
      absl::FPrintF(stderr, "Program linking failed: %s\n", info_log);
      return false;
    }
    return true;
  }
  inline void Use() const { glUseProgram(id_); }

  static std::unique_ptr<Program> CreateFromShaders(
      const std::vector<std::shared_ptr<Shader>> &shaders) {
    std::unique_ptr<Program> program{new Program};
    program->AttachShaders(shaders);
    if (!program->Link()) { return nullptr; }
    return program;
  }

  ~Program() { glDeleteProgram(id_); }

 private:
  std::vector<std::shared_ptr<Shader>> attached_shaders_;
};

}  // namespace gl_tutorials

#endif  // OPENGL_TUTORIALS_CORE_PROGRAM_H_
