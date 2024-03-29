#ifndef OPENGL_TUTORIALS_CORE_SHADER_H_
#define OPENGL_TUTORIALS_CORE_SHADER_H_

#include "absl/strings/str_format.h"
#include "absl/strings/str_split.h"
#include "gl/core/opengl_object.h"
#include "glog/logging.h"

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

namespace gl {

class Shader : public OpenGlObject {
 public:
  enum class Type : GLint {
    kUndefined = -1,
    kVertexShader = GL_VERTEX_SHADER,
    kFragmentShader = GL_FRAGMENT_SHADER,
    kGeometryShader = GL_GEOMETRY_SHADER
  };

  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;
  Shader(Shader&& other) = delete;
  Shader& operator=(Shader&& other) = delete;

  static std::unique_ptr<Shader> CreateFromFile(
      const std::filesystem::path& file_name);

  static std::vector<std::shared_ptr<Shader>> CreateFromFiles(
      const std::vector<std::filesystem::path>& file_names);

  inline Shader::Type type() const { return type_; }

  ~Shader() { glDeleteShader(id_); }

 private:
  Shader(Shader::Type gl_shader_type, std::string shader_source)
      : OpenGlObject{glCreateShader(
            static_cast<std::underlying_type<Shader::Type>::type>(
                gl_shader_type))},
        shader_source_{shader_source},
        type_{gl_shader_type} {}

  static Shader::Type DetectShaderType(const std::string& file_name);

  bool CompileShader();

  std::string shader_source_{};
  Shader::Type type_{Shader::Type::kUndefined};
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_CORE_SHADER_H_
