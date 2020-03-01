#ifndef OPENGL_TUTORIALS_CORE_SHADER_H_
#define OPENGL_TUTORIALS_CORE_SHADER_H_

#include "opengl_tutorials/core/gl_base.h"

#include "absl/strings/str_format.h"
#include "absl/strings/str_split.h"
#include "opengl_tutorials/utils/file_utils.h"

#include <iostream>
#include <memory>
#include <string>

namespace gl_tutorials {

class Shader : public OpenGlObject {
public:
  enum class Type : GLint {
    kUndefined = -1,
    kVertexShader = GL_VERTEX_SHADER,
    kFragmentShader = GL_FRAGMENT_SHADER
  };

  static std::unique_ptr<Shader> CreateFromFile(const std::string &file_name);
  inline Shader::Type type() const { return type_; }

  ~Shader() { glDeleteShader(id_); }

private:
  Shader(Shader::Type gl_shader_type, std::string shader_source)
      : OpenGlObject{glCreateShader(
            static_cast<std::underlying_type<Shader::Type>::type>(
                gl_shader_type))},
        shader_source_{std::move(shader_source)} {}

  static Shader::Type DetectShaderType(const std::string &file_name);

  bool CompileShader();

  std::string shader_source_{};
  Shader::Type type_{Shader::Type::kUndefined};
};

} // namespace gl_tutorials

#endif // OPENGL_TUTORIALS_CORE_SHADER_H_
