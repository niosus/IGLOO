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
  [[nodiscard]] inline std::size_t SetUniform(const std::string& uniform_name,
                                              const std::vector<T, A>& data) {
    const auto uniform_index{GetUniformIndexOrEmplace(uniform_name)};
    uniforms_[uniform_index].UpdateValue(data);
    return uniform_index;
  }

  template <typename... Ts>
  [[nodiscard]] inline std::size_t SetUniform(const std::string& uniform_name,
                                              Ts... numbers) {
    const auto uniform_index{GetUniformIndexOrEmplace(uniform_name)};
    uniforms_[uniform_index].UpdateValue(numbers...);
    return uniform_index;
  }

  [[nodiscard]] Uniform& GetUniform(std::size_t index) noexcept {
    CHECK_LT(index, uniforms_.size());
    return uniforms_[index];
  }

  [[nodiscard]] const Uniform& GetUniform(std::size_t index) const noexcept {
    CHECK_LT(index, uniforms_.size());
    return uniforms_[index];
  }

  [[nodiscard]] bool Link() const;

  [[nodiscard]] Uniform* EmplaceUniform(Uniform&& uniform);

  [[nodiscard]] static std::optional<Program> CreateFromShaders(
      const std::vector<std::shared_ptr<Shader>>& shaders);

  Program(const Program&) = delete;
  Program& operator=(const Program&) = delete;
  Program(Program&& other) { *this = std::move(other); }
  Program& operator=(Program&& other) {
    if (this == &other) { return *this; }
    id_ = other.id_;
    uniforms_ = std::move(other.uniforms_);
    uniform_ids_ = std::move(other.uniform_ids_);
    attached_shaders_ = std::move(other.attached_shaders_);
    other.id_ = 0;
    return *this;
  }
  ~Program() {
    if (id_) { glDeleteProgram(id_); }
  }

 private:
  std::size_t GetUniformIndexOrEmplace(const std::string& uniform_name);

  std::vector<Uniform> uniforms_{};
  std::map<std::string, std::size_t> uniform_ids_{};
  std::vector<std::shared_ptr<Shader>> attached_shaders_{};
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_CORE_PROGRAM_H_
