#include "gl/core/shader.h"

#include "absl/strings/str_split.h"
#include "glog/logging.h"
#include "utils/file_utils.h"

#include <iostream>
#include <memory>
#include <string>

namespace {
constexpr auto kExpectEveryStringNullTerminated = nullptr;
constexpr auto kLengthNotNeeded = nullptr;
constexpr auto kDefaultErrorBufferSize{512};
}  // namespace

namespace gl {

std::unique_ptr<Shader> Shader::CreateFromFile(
    const std::filesystem::path& file_name) {
  CHECK(std::filesystem::exists(file_name))
      << "File '" << file_name << "' does not exist.";
  const auto gl_shader_type = DetectShaderType(file_name.filename());
  if (gl_shader_type == Shader::Type::kUndefined) { return nullptr; }
  const auto shader_source = utils::ReadFileContents(file_name);
  if (!shader_source) { return nullptr; }
  // Cannot use make_unique due to a private constructor.
  std::unique_ptr<Shader> shader{
      new Shader{gl_shader_type, shader_source.value()}};
  if (shader->CompileShader()) { return shader; }
  return nullptr;
}

std::vector<std::shared_ptr<Shader>> Shader::CreateFromFiles(
    const std::vector<std::filesystem::path>& file_names) {
  std::vector<std::shared_ptr<gl::Shader>> shaders{};
  std::transform(file_names.cbegin(),
                 file_names.cend(),
                 std::back_inserter(shaders),
                 [](const auto& shader_path) -> std::shared_ptr<gl::Shader> {
                   return gl::Shader::CreateFromFile(shader_path);
                 });
  return shaders;
}

Shader::Type Shader::DetectShaderType(const std::string& file_name) {
  const std::vector<std::string> split{absl::StrSplit(file_name, '.')};
  if (split.empty()) {
    LOG(ERROR) << "Shader file: '" << file_name << "' has no extension";
    return Shader::Type::kUndefined;
  }
  const std::string& extention = split.back();
  if (extention == "vert") { return Shader::Type::kVertexShader; }
  if (extention == "frag") { return Shader::Type::kFragmentShader; }
  if (extention == "geom") { return Shader::Type::kGeometryShader; }
  LOG(ERROR) << "Unknown shader file extention: '" << extention
             << "' for file '" << file_name;
  return Shader::Type::kUndefined;
}

bool Shader::CompileShader() {
  const char* data = shader_source_.data();
  glShaderSource(id_, 1, &data, kExpectEveryStringNullTerminated);
  glCompileShader(id_);
  std::int32_t success{};
  glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
  if (!success) {
    char error_msg[kDefaultErrorBufferSize];
    glGetShaderInfoLog(
        id_, kDefaultErrorBufferSize, kLengthNotNeeded, error_msg);
    LOG(FATAL) << "Shader compilation failed: \n" << error_msg;
    return false;
  }
  return true;
}

}  // namespace gl
