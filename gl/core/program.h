#ifndef OPENGL_TUTORIALS_CORE_PROGRAM_H_
#define OPENGL_TUTORIALS_CORE_PROGRAM_H_

#include "gl/core/opengl_object.h"
#include "gl/core/shader.h"
#include "gl/core/uniform.h"

#include <cstdint>

namespace gl {

class Program : public OpenGlObject {
 public:
  Program() : OpenGlObject{glCreateProgram()} {}

  inline void AttachShader(const std::shared_ptr<Shader>& shader) {
    glAttachShader(id_, shader->id());
    attached_shaders_.push_back(shader);
  }

  inline void AttachShaders(
      const std::vector<std::shared_ptr<Shader>>& shaders) {
    attached_shaders_.reserve(attached_shaders_.size() + shaders.size());
    for (const auto& shader : shaders) { AttachShader(shader); }
  }

  inline void Use() const { glUseProgram(id_); }

  template <typename T, typename A>
  inline Uniform* SetUniform(const std::string& uniform_name,
                             const std::vector<T, A>& data) {
    auto& uniform{GetUniform(uniform_name)};
    uniform.UpdateValue(data);
    return &uniform;
  }

  template <typename... Ts>
  inline Uniform* SetUniform(const std::string& uniform_name, Ts... numbers) {
    auto& uniform{GetUniform(uniform_name)};
    uniform.UpdateValue(numbers...);
    return &uniform;
  }

  bool Link() const;

  Uniform* EmplaceUniform(Uniform&& uniform);

  static std::unique_ptr<Program> CreateFromShaders(
      const std::vector<std::shared_ptr<Shader>>& shaders);

  Program(const Program&) = delete;
  Program& operator=(const Program&) = delete;

  ~Program() { glDeleteProgram(id_); }

 private:
  Uniform& GetUniform(const std::string& uniform_name);

  std::vector<Uniform> uniforms_;
  std::map<std::string, size_t> uniform_ids_;

  std::vector<std::shared_ptr<Shader>> attached_shaders_;
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_CORE_PROGRAM_H_
