#ifndef CODE_OPENGL_TUTORIALS_GL_CORE_OPENGL_OBJECT_H_
#define CODE_OPENGL_TUTORIALS_GL_CORE_OPENGL_OBJECT_H_

#include "glad/gl.h"

#include <cstdint>

namespace gl {

class OpenGlObject {
 public:
  using IdType = std::uint32_t;

  OpenGlObject(IdType id = 0) : id_{id} {}

  IdType id() const { return id_; }

 protected:
  IdType id_{};
};

}  // namespace gl

#endif  // CODE_OPENGL_TUTORIALS_GL_CORE_OPENGL_OBJECT_H_
