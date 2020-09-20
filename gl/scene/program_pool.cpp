// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "gl/scene/program_pool.h"

#include <glog/logging.h>

namespace gl {

ProgramPool& ProgramPool::Instance() {
  static ProgramPool pool;
  return pool;
}

const std::shared_ptr<Program> ProgramPool::AddProgram(
    ProgramType program_type, std::shared_ptr<Program> program) {
  const auto& [iter, emplaced] = programs_.emplace(program_type, program);
  CHECK(emplaced) << "Could not add a program to the pool. It already exists.";
  return iter->second;
}

const std::shared_ptr<Program> ProgramPool::AddProgramFromShaders(
    ProgramType program_type, const std::vector<std::string>& shader_paths) {
  std::vector<std::shared_ptr<Shader>> shaders{};
  std::transform(shader_paths.cbegin(),
                 shader_paths.cend(),
                 std::back_inserter(shaders),
                 [](const auto& shader_path) -> std::shared_ptr<Shader> {
                   return Shader::CreateFromFile(shader_path);
                 });
  return AddProgram(program_type, Program::CreateFromShaders(shaders));
}

const std::shared_ptr<Program> ProgramPool::GetProgram(
    ProgramType program_type) const {
  CHECK(programs_.count(program_type))
      << "Must add programs before getting them.";
  return programs_.at(program_type);
}

std::vector<ProgramPool::ProgramType>
ProgramPool::QueryAvailableProgramTypes() {
  std::vector<ProgramType> program_types;
  program_types.reserve(programs_.size());
  std::transform(programs_.cbegin(),
                 programs_.cend(),
                 std::back_inserter(program_types),
                 [](const auto& program_iter) { return program_iter.first; });
  return program_types;
}

bool ProgramPool::RemoveProgram(ProgramType program_type) {
  return programs_.erase(program_type) > 0ul;
}

const std::shared_ptr<Program> ProgramPool::CreateSharedProgram(
    ProgramType program_type) {
  auto program_ptr = std::make_unique<Program>();
  switch (program_type) {
    case ProgramType::DRAW_POINTS: {
      program_ptr->AttachShaders(
          {Shader::CreateFromFile("gl/scene/shaders/points.vert"),
           Shader::CreateFromFile("gl/scene/shaders/simple.frag")});
      break;
    }
    case ProgramType::DRAW_COORDINATE_SYSTEM: {
      program_ptr->AttachShaders(
          {Shader::CreateFromFile("gl/scene/shaders/coordinate_system.vert"),
           Shader::CreateFromFile("gl/scene/shaders/coordinate_system.geom"),
           Shader::CreateFromFile("gl/scene/shaders/simple.frag")});
      break;
    }
    case ProgramType::DRAW_TEXTURED_RECT: {
      program_ptr->AttachShaders(
          {Shader::CreateFromFile("gl/scene/shaders/texture.vert"),
           Shader::CreateFromFile("gl/scene/shaders/texture.geom"),
           Shader::CreateFromFile("gl/scene/shaders/texture.frag")});
      break;
    }
    case ProgramType::DRAW_TEXT: {
      program_ptr->AttachShaders(
          {Shader::CreateFromFile("gl/scene/shaders/text.vert"),
           Shader::CreateFromFile("gl/scene/shaders/texture.frag")});
      break;
    }
    default: LOG(FATAL) << "Unhandled program type."; break;
  }
  program_ptr->Link();
  return program_ptr;
}

}  // namespace gl
