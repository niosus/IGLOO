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

class Shader {
public:
  static std::unique_ptr<Shader> CreateFromFile(const std::string &file_name);
  inline std::uint32_t id() const { return id_; }

private:
  Shader(std::string shader_source)
      : shader_source_{std::move(shader_source)} {}

  static absl::optional<int> DetectShaderType(const std::string &file_name);

  bool CompileShader(int gl_shader_type);

  std::string shader_source_{};
  std::uint32_t id_{};
};

} // namespace gl_tutorials

#endif // OPENGL_TUTORIALS_CORE_SHADER_H_
