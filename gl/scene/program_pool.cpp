// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "gl/scene/program_pool.h"

#include <glog/logging.h>

namespace gl {

const std::shared_ptr<Program> ProgramPool::Get(ProgramType program_type) {
  CHECK(programs_.count(program_type))
      << "Must run CreateAllPrograms before using a program.";
  return programs_.at(program_type);
}

std::vector<ProgramPool::ProgramType> ProgramPool::Types() {
  if (!types_.empty()) { return types_; }
  // Populate the types vector.
  types_ =
      std::vector<ProgramType>(static_cast<int>(ProgramType::LAST_PROGRAM));
  for (int i = 0; i < static_cast<int>(ProgramType::LAST_PROGRAM); ++i) {
    types_[i] = static_cast<ProgramType>(i);
  }
  return types_;
}

void ProgramPool::CreateAllPrograms() {
  CHECK(programs_.empty()) << "We should create all programs only once.";
  // Generate all programs needed for execution.
  for (const auto program_type : ProgramPool::Types()) {
    LOG(INFO) << "Creating program: " << static_cast<int>(program_type);
    programs_.emplace(program_type, CreateSharedProgram(program_type));
  }
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
    default: LOG(WARNING) << "Unhandled program type."; break;
  }
  program_ptr->Link();
  return program_ptr;
}

}  // namespace gl
