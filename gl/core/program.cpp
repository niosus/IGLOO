#include "gl/core/program.h"

#include "absl/strings/str_format.h"

namespace gl {

bool Program::Link() const {
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

Uniform* Program::EmplaceUniform(Uniform&& uniform) {
  if (uniform_ids_.count(uniform.name()) > 0) {
    const size_t found_index = uniform_ids_.at(uniform.name());
    uniforms_[found_index] = std::forward<Uniform>(uniform);
    return &uniforms_[found_index];
  }
  const size_t index = uniforms_.size();
  uniform_ids_.emplace(uniform.name(), index);
  return &uniforms_.emplace_back(std::forward<Uniform>(uniform));
}

std::unique_ptr<Program> Program::CreateFromShaders(
    const std::vector<std::shared_ptr<Shader>>& shaders) {
  auto program{std::make_unique<Program>()};
  program->AttachShaders(shaders);
  if (!program->Link()) { return nullptr; }
  return program;
}

Uniform& Program::GetUniform(const std::string& uniform_name) {
  if (uniform_ids_.count(uniform_name) < 1) {
    uniform_ids_.emplace(uniform_name, uniforms_.size());
    return uniforms_.emplace_back(uniform_name, id_);
  }
  return uniforms_[uniform_ids_.at(uniform_name)];
}

}  // namespace gl
