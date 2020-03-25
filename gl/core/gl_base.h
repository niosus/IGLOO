#ifndef OPENGL_TUTORIALS_CORE_GL_BASE_H_
#define OPENGL_TUTORIALS_CORE_GL_BASE_H_

#include "third_party/glad/glad.h"

#include <cstdint>

namespace gl {

class OpenGlObject {
 public:
  using IdType = std::uint32_t;

  OpenGlObject(IdType id) : id_{id} {}

  IdType id() const { return id_; }

 protected:
  IdType id_{};
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_CORE_GL_BASE_H_
