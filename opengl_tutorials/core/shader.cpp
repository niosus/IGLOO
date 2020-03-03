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
  if (gl_shader_type == Shader::Type::kUndefined) {
    absl::FPrintF(stderr, "Could not detect shader type\n");
    return nullptr;
  }
  const auto shader_source = utils::ReadFileContents(file_name);
  if (!shader_source) {
    absl::FPrintF(stderr, "Cannot load shader from path: '%s'\n", file_name);
    return nullptr;
  }
  std::unique_ptr<Shader> shader{
      new Shader{gl_shader_type, shader_source.value()}};
  if (shader->CompileShader()) { return shader; }
  return nullptr;
}

Shader::Type Shader::DetectShaderType(const std::string &file_name) {
  const std::vector<std::string> split{absl::StrSplit(file_name, '.')};
  if (split.empty()) {
    absl::FPrintF(stderr, "Shader file: '%s' has no extention\n", file_name);
    return Shader::Type::kUndefined;
  }
  const std::string &extention = split.back();
  if (extention == "vert") { return Shader::Type::kVertexShader; }
  if (extention == "frag") { return Shader::Type::kFragmentShader; }
  absl::FPrintF(stderr,
                "Unknown shader file extention: '%s' for file '%s'\n",
                extention,
                file_name);
  return Shader::Type::kUndefined;
}

bool Shader::CompileShader() {
  const char *data = shader_source_.data();
  glShaderSource(id_, 1, &data, nullptr);
  glCompileShader(id_);
  int success;
  char info_log[512];
  glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id_, 512, nullptr, info_log);
    absl::FPrintF(stderr, "Shader compilation failed: %s\n", info_log);
    return false;
  }
  return true;
}

}  // namespace gl_tutorials
