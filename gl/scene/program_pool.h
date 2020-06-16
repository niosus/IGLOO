// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#ifndef OPENGL_TUTORIALS_GL_SCENE_PROGRAM_POOL_H_
#define OPENGL_TUTORIALS_GL_SCENE_PROGRAM_POOL_H_

#include "gl/core/program.h"

#include <glog/logging.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace gl {

/// Container for all available OpenGL programs.
class ProgramPool {
 public:
  using ProgramVector = std::vector<std::unique_ptr<Program>>;
  using ProgramIndex = typename ProgramVector::size_type;
  using ProgramMap = std::map<std::string, ProgramIndex>;

  ProgramIndex Emplace(const std::string& name,
                       std::unique_ptr<Program>&& program);

  /// Iterate over all types of available programs.
  std::vector<std::string> Names();

  inline void Use(const std::string& name) const {
    CHECK(name_to_index_mapping_.count(name))
        << "A program with name: '" << name
        << "' does not exist in the ProgramPool.";
    this->Use(name_to_index_mapping_.at(name));
  }

  inline void Use(ProgramIndex index) const {
    CHECK_LT(index, programs_.size())
        << "Not enough programs stored to fetch one with index: " << index;
    auto& program_ptr = programs_[index];
    CHECK(program_ptr) << "Fetched program pointer is unexpectedly nullptr.";
    program_ptr->Use();
  }

  template <typename T, typename A>
  inline void SetUniformToAllPrograms(const std::string& uniform_name,
                                      const std::vector<T, A>& data) {
    for (auto& program_ptr : programs_) {
      program_ptr->Use();
      program_ptr->SetUniform(uniform_name, data);
    }
  }

  template <typename... Ts>
  inline void SetUniformToAllPrograms(const std::string& uniform_name,
                                      Ts... numbers) {
    for (auto& program_ptr : programs_) {
      program_ptr->Use();
      program_ptr->SetUniform(uniform_name, numbers...);
    }
  }

  ProgramPool(const ProgramPool&) = delete;
  ProgramPool& operator=(const ProgramPool&) = delete;

  ProgramPool(ProgramPool&&) = default;
  ProgramPool& operator=(ProgramPool&&) = default;

  ~ProgramPool() = default;

 private:
  ProgramMap name_to_index_mapping_;
  ProgramVector programs_;
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_GL_SCENE_PROGRAM_POOL_H_
