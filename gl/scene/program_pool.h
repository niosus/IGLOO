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
class ProgramPool {
 public:
  enum class ProgramType {
    DRAW_POINTS,
    DRAW_COORDINATE_SYSTEM,
    DRAW_TEXTURED_RECT,
    DRAW_TEXT,
    LAST_PROGRAM
  };

  using ProgramMap = std::map<ProgramType, std::shared_ptr<Program>>;

  ProgramPool() = default;
  ProgramPool(const ProgramPool&) = delete;
  ProgramPool(ProgramPool&&) = delete;
  ProgramPool& operator=(const ProgramPool&) = delete;
  ProgramPool& operator=(ProgramPool&&) = delete;
  ~ProgramPool() noexcept = default;

  void Clear() { programs_.clear(); }

  /// Iterate over all types of available programs.
  std::vector<ProgramType> QueryAvailableProgramTypes() const noexcept;

  /// Add a program to the pool.
  const std::shared_ptr<Program> AddProgram(ProgramType program_type,
                                            std::shared_ptr<Program> program);

  /// Convenience function to add a program to the pool from shader paths.
  const std::shared_ptr<Program> AddProgramFromShaders(
      ProgramType program_type, const std::vector<std::string>& shader_paths);

  /// Remove a program associated to this program type from the pool.
  bool RemoveProgram(ProgramType program_type);

  /// Get a program.
  const std::shared_ptr<Program> GetProgram(
      ProgramType program_type) const noexcept;

  template <typename T, typename A>
  inline void SetUniformToAllPrograms(const std::string& uniform_name,
                                      const std::vector<T, A>& data) {
    for (auto& kv : programs_) {
      auto& program_ptr = kv.second;
      program_ptr->Use();
      program_ptr->SetUniform(uniform_name, data);
    }
  }

  template <typename... Ts>
  inline void SetUniformToAllPrograms(const std::string& uniform_name,
                                      Ts... numbers) {
    for (auto& kv : programs_) {
      auto& program_ptr = kv.second;
      program_ptr->Use();
      program_ptr->SetUniform(uniform_name, numbers...);
    }
  }

 private:
  ProgramMap programs_;
  const std::shared_ptr<Program> CreateSharedProgram(ProgramType program_type);
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_GL_SCENE_PROGRAM_POOL_H_
