// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "gl/scene/program_pool.h"

#include <glog/logging.h>

namespace gl {

ProgramPool::ProgramIndex ProgramPool::AddProgram(Program&& program) {
  const auto current_index = programs_.size();
  programs_.emplace_back(std::move(program));
  return current_index;
}

std::optional<ProgramPool::ProgramIndex> ProgramPool::AddProgramFromShaders(
    const std::vector<std::shared_ptr<Shader>>& shaders) {
  auto program{Program::CreateFromShaders(shaders)};
  if (program) { return AddProgram(std::move(program.value())); }
  return {};
}

void ProgramPool::UseProgram(ProgramIndex program_index) noexcept {
  CHECK_LT(program_index, programs_.size())
      << "Trying to use a program by a wrong program index.";
  auto& program{programs_[program_index]};
  CHECK(program.has_value()) << "Trying to use a deleted program.";
  active_program_index_ = program_index;
  program->Use();
}

void ProgramPool::RemoveProgram(ProgramIndex program_index) noexcept {
  CHECK_LT(program_index, programs_.size())
      << "Trying to remove a program by a wrong program index.";
  programs_[program_index] = {};
}

}  // namespace gl
