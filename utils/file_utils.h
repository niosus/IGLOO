#ifndef OPENGL_TUTORIALS_UTILS_FILE_UTILS_H_
#define OPENGL_TUTORIALS_UTILS_FILE_UTILS_H_

#include "absl/types/optional.h"

#include <fstream>
#include <string>
#include <sys/stat.h>

namespace utils {

inline bool FileExists(const std::string& name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

inline absl::optional<std::string> ReadFileContents(const std::string& path) {
  std::ifstream ifs{path};
  if (!ifs) { return {}; }
  return absl::optional<std::string>{
      {std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()}};
}

}  // namespace utils

#endif  // OPENGL_TUTORIALS_UTILS_FILE_UTILS_H_
