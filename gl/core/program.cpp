#include "gl/core/program.h"

#include "absl/strings/str_format.h"

namespace gl {

bool Program::Link() const {
  glLinkProgram(id_);
  return true;
}

Uniform* Program::EmplaceUniform(Uniform&& uniform) {
  if (uniform_ids_.count(uniform.name()) > 0) {
    const size_t found_index = uniform_ids_.at(uniform.name());
    uniforms_[found_index] = std::move(uniform);
    return &uniforms_[found_index];
  }
  const size_t index = uniforms_.size();
  uniform_ids_.emplace(uniform.name(), index);
  return &uniforms_.emplace_back(std::forward<Uniform>(uniform));
}

std::optional<Program> Program::CreateFromShaders(
    const std::vector<std::shared_ptr<Shader>>& shaders) {
  Program program;
  program.AttachShaders(shaders);
  if (!program.Link()) { return {}; }
  return program;
}

size_t Program::GetUniformIndexOrEmplace(const std::string& uniform_name) {
  if (uniform_ids_.count(uniform_name) < 1) {
    uniform_ids_.emplace(uniform_name, uniforms_.size());
    uniforms_.emplace_back(uniform_name, id_);
    return uniforms_.size() - 1U;
  }
  return uniform_ids_.at(uniform_name);
}

}  // namespace gl
