#ifndef OPENGL_TUTORIALS_UTILS_FILE_UTILS_H_
#define OPENGL_TUTORIALS_UTILS_FILE_UTILS_H_

#include "absl/types/optional.h"

#include <fstream>
#include <string>

namespace gl_tutorials {
namespace utils {

inline absl::optional<std::string> ReadFileContents(const std::string &path) {
  std::ifstream ifs{path};
  if (!ifs) { return {}; }
  return absl::optional<std::string>{
      {std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()}};
}

}  // namespace utils
}  // namespace gl_tutorials

#endif  // OPENGL_TUTORIALS_UTILS_FILE_UTILS_H_
