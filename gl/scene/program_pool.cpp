// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "gl/scene/program_pool.h"

#include <glog/logging.h>

namespace gl {

std::vector<std::string> ProgramPool::Names() {
  std::vector<std::string> names;
  names.reserve(name_to_index_mapping_.size());
  for (const auto& kv : name_to_index_mapping_) {
    names.emplace_back(kv.first);
  }
  return names;
}

ProgramPool::ProgramIndex ProgramPool::Emplace(
    const std::string& name, std::unique_ptr<Program>&& program) {
  CHECK_NE(name_to_index_mapping_.count(name), 0u)
      << "Program with name '" << name
      << "' is already present. Please use a different name.";
  const auto index = programs_.size();
  name_to_index_mapping_.emplace(name, index);
  programs_.emplace_back(std::move(program));
  return index;
}

}  // namespace gl
