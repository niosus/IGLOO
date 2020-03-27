#ifndef OPENGL_TUTORIALS_CORE_PROGRAM_H_
#define OPENGL_TUTORIALS_CORE_PROGRAM_H_

#include "gl/core/opengl_object.h"
#include "gl/core/shader.h"
#include "gl/core/uniform.h"

#include <cstdint>

namespace gl {

// TODO(igor): As Uniforms are bound to a program it is probably a good idea to
// attach them to a program class. However, as our Uniforms are nothing but a
// thin layer that passes the data through, it seems the best way to store them
// is in some vector. Whenever a user adds a uniform, they must transfer
// ownership to the program. The function that adds a uniform would return the
// index in the vector under which the uniform will be available (maybe just an
// id?). This id can be stored externally and the program can be asked to
// provide a reference to a uniform it is owning for value modification. This
// will allow to avoid complex template magic in the program class as it won't
// need to pass the data through to a uniform.

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

}  // namespace gl

#endif  // OPENGL_TUTORIALS_CORE_PROGRAM_H_
