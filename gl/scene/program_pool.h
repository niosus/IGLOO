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

  /// Generate all programs so that they are ready to use.
  void CreateAllPrograms();

  /// Iterate over all types of available programs.
  std::vector<ProgramType> Types();

  const std::shared_ptr<Program> Get(ProgramType program_type);

  template <typename T, typename A>
  inline void SetUniform(const std::string& uniform_name,
                         const std::vector<T, A>& data) {
    for (auto& kv : programs_) {
      auto& program_ptr = kv.second;
      program_ptr->Use();
      program_ptr->SetUniform(uniform_name, data);
    }
  }

  template <typename... Ts>
  inline void SetUniform(const std::string& uniform_name, Ts... numbers) {
    for (auto& kv : programs_) {
      auto& program_ptr = kv.second;
      program_ptr->Use();
      program_ptr->SetUniform(uniform_name, numbers...);
    }
  }

 private:
  ProgramMap programs_;
  std::vector<ProgramType> types_;

  const std::shared_ptr<Program> CreateSharedProgram(ProgramType program_type);
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_GL_SCENE_PROGRAM_POOL_H_
