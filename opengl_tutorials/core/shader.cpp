#include "opengl_tutorials/core/shader.h"

#include "absl/strings/str_format.h"
#include "absl/strings/str_split.h"
#include "opengl_tutorials/utils/file_utils.h"

#include <iostream>
#include <memory>
#include <string>

namespace gl_tutorials {

std::unique_ptr<Shader> Shader::CreateFromFile(const std::string &file_name) {
  auto gl_shader_type = DetectShaderType(file_name);
  if (!gl_shader_type) {
    absl::FPrintF(stderr, "Could not detect shader type");
    return nullptr;
  }
  const auto shader_source = utils::ReadFileContents(file_name);
  if (!shader_source) {
    absl::FPrintF(stderr, "Cannot load shader from path: '%s'", file_name);
    return nullptr;
  }
  std::unique_ptr<Shader> shader{new Shader{shader_source.value()}};
  if (shader->CompileShader(gl_shader_type.value())) {
    return shader;
  }
  return nullptr;
}

absl::optional<int> Shader::DetectShaderType(const std::string &file_name) {
  const std::vector<std::string> split{absl::StrSplit(file_name, '.')};
  if (split.empty()) {
    absl::FPrintF(stderr, "Shader file: '%s' has no extention", file_name);
    return {};
  }
  int gl_shader_type{-1};
  const std::string &extention = split.back();
  if (extention == "vert") {
    gl_shader_type = GL_VERTEX_SHADER;
  } else if (extention == "frag") {
    gl_shader_type = GL_FRAGMENT_SHADER;
  } else {
    absl::FPrintF(stderr, "Unknown shader file extention: '%s' for file '%s'",
                  extention, file_name);
    return {};
  }
  return gl_shader_type;
}

bool Shader::CompileShader(int gl_shader_type) {
  id_ = glCreateShader(gl_shader_type);
  const char *vert_shader_data = shader_source_.data();
  glShaderSource(id_, 1, &vert_shader_data, nullptr);
  glCompileShader(id_);
  int success;
  char info_log[512];
  glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id_, 512, nullptr, info_log);
    absl::FPrintF(stderr, "Shader compilation failed: %s", info_log);
    return false;
  }
  return true;
}

} // namespace gl_tutorials
