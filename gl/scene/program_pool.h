// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#ifndef OPENGL_TUTORIALS_GL_SCENE_PROGRAM_POOL_H_
#define OPENGL_TUTORIALS_GL_SCENE_PROGRAM_POOL_H_

#include "gl/core/program.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace gl {

/// Container for all available OpenGL programs.
///
/// In it intended that all work with programs will happen through this pool.
/// This way the programs can always know which program is active right now.
class ProgramPool {
 public:
  using ProgramIndex = std::size_t;

  ProgramPool() = default;
  ProgramPool(const ProgramPool&) = delete;
  ProgramPool& operator=(const ProgramPool&) = delete;
  ProgramPool(ProgramPool&&) = default;
  ProgramPool& operator=(ProgramPool&&) = default;
  ~ProgramPool() noexcept = default;

  /// Add a program to the pool.
  [[nodiscard]] ProgramIndex AddProgram(Program&& program);

  /// Convenience function to add a program to the pool from shader paths.
  [[nodiscard]] std::optional<ProgramPool::ProgramIndex> AddProgramFromShaders(
      const std::vector<std::shared_ptr<Shader>>& shader_paths);

  /// Remove a program associated to this program type from the pool.
  ///
  /// For now this will just set the appropriate index to empty optional,
  /// deleting the program. If there is a drawable that uses the program, the
  /// behavior is undefined.
  void RemoveProgram(ProgramIndex program_index) noexcept;

  /// Use the program.
  void UseProgram(ProgramIndex program_index) noexcept;

  template <typename... Ts>
  [[nodiscard]] inline std::size_t SetUniformToActiveProgram(
      const std::string& uniform_name, const Ts&... numbers) {
    CHECK(active_program_index_.has_value())
        << "There is no active program. Cannot set uniform.";
    auto& program = programs_[active_program_index_.value()];
    return program->SetUniform(uniform_name, numbers...);
  }

  /// Use this version when the uniform index is already known.
  template <typename... Ts>
  inline void UpdateUniformInActiveProgram(std::size_t uniform_index,
                                           const Ts&... numbers) {
    CHECK(active_program_index_.has_value())
        << "There is no active program. Cannot set uniform.";
    auto& program = programs_[active_program_index_.value()];
    return program->GetUniform(uniform_index).UpdateValue(numbers...);
  }

  template <typename... Ts>
  inline void SetUniformToAllPrograms(const std::string& uniform_name,
                                      const Ts&... numbers) {
    const auto prev_active_program_index = active_program_index_;
    std::size_t program_index{};
    for (auto& program : programs_) {
      UseProgram(program_index++);
      (void)program->SetUniform(uniform_name, numbers...);
    }
    if (prev_active_program_index) {
      UseProgram(prev_active_program_index.value());
    }
  }

  [[nodiscard]] inline std::optional<ProgramIndex> active_program_index()
      const noexcept {
    return active_program_index_;
  }

 private:
  std::vector<std::optional<Program>> programs_;
  std::optional<ProgramIndex> active_program_index_;
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_GL_SCENE_PROGRAM_POOL_H_
