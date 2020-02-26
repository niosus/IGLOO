#ifndef OPENGL_TUTORIALS_OPENGL_TUTORIALS_UTILS_SHADER_LOADER_H_
#define OPENGL_TUTORIALS_OPENGL_TUTORIALS_UTILS_SHADER_LOADER_H_

#include "absl/types/optional.h"

#include <fstream>
#include <string>

class ShaderLoader {
public:
  static absl::optional<std::string> ReadShader(const std::string &path) {
    std::ifstream ifs{path};
    if (!ifs) {
      return {};
    }
    return absl::optional<std::string>{{std::istreambuf_iterator<char>(ifs),
                                        std::istreambuf_iterator<char>()}};
  }
};

#endif // OPENGL_TUTORIALS_OPENGL_TUTORIALS_UTILS_SHADER_LOADER_H_
