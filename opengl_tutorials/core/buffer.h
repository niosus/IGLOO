#ifndef OPENGL_TUTORIALS_CORE_VERTEX_BUFFER_H_
#define OPENGL_TUTORIALS_CORE_VERTEX_BUFFER_H_

#include "opengl_tutorials/core/gl_base.h"

#include "Eigen/Core"

#include <map>
#include <vector>

namespace gl_tutorials {

class Buffer : public OpenGlObject {
public:
  enum class Type : GLenum {
    kArrayBuffer = GL_ARRAY_BUFFER,
    kElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER
  };

  enum class Usage : GLenum {
    kStaticDraw = GL_STATIC_DRAW,
    kDynamicDraw = GL_DYNAMIC_DRAW
  };

  Buffer(Buffer::Type type, Buffer::Usage usage)
      : OpenGlObject{0}, type_{static_cast<GLenum>(type)},
        usage_{static_cast<GLenum>(usage)} {
    glGenBuffers(1, &id_);
  }

  template <typename T, typename A>
  void AssignData(const std::vector<T, A> &vertices) const {
    AssignData(vertices.data(), vertices.size());
  }

  template <typename T>
  void AssignData(const T *const data, std::size_t number_of_elements) const {
    GLint bound_buffer;
    glGetIntegerv(MapTypeToBindingType(type_), &bound_buffer);
    glBindBuffer(type_, id_);
    glBufferData(type_, sizeof(T) * number_of_elements, data, usage_);
    glBindBuffer(type_, bound_buffer);
  }

  void Bind() const { glBindBuffer(type_, id_); }

  void UnBind(OpenGlObject::IdType id_to_bind = 0u) const {
    glBindBuffer(type_, id_to_bind);
  }

private:
  static inline GLenum MapTypeToBindingType(GLenum type) {
    switch (type) {
    case GL_ARRAY_BUFFER:
      return GL_ARRAY_BUFFER_BINDING;
    case GL_ELEMENT_ARRAY_BUFFER:
      return GL_ELEMENT_ARRAY_BUFFER_BINDING;
    }
    return 0;
  }

  GLenum type_;
  GLenum usage_;
};

} // namespace gl_tutorials

#endif // OPENGL_TUTORIALS_CORE_VERTEX_BUFFER_H_
