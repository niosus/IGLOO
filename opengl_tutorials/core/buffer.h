#ifndef OPENGL_TUTORIALS_CORE_VERTEX_BUFFER_H_
#define OPENGL_TUTORIALS_CORE_VERTEX_BUFFER_H_

#include "opengl_tutorials/core/buffer_traits.h"
#include "opengl_tutorials/core/gl_base.h"
#include "opengl_tutorials/utils/type_utils.h"

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
      : OpenGlObject{0},
        type_{static_cast<GLenum>(type)},
        usage_{static_cast<GLenum>(usage)} {
    glGenBuffers(1, &id_);
  }

  template <typename T, typename A>
  void AssignData(const std::vector<T, A> &vertices) {
    AssignData(vertices.data(), vertices.size());
  }

  template <typename T>
  void AssignData(const T *const data, std::size_t number_of_elements) {
    static_assert(
        type_utils::has_value<
            typename traits::components_per_vertex_count<T>>::value,
        "Missing specialization for trait 'components_per_vertex_count'");
    static_assert(
        type_utils::has_value<typename traits::gl_underlying_type<T>>::value,
        "Missing specialization for trait 'gl_underlying_type'");
    components_per_vertex_ = traits::components_per_vertex_count<T>::value;
    gl_underlying_data_type_ = traits::gl_underlying_type<T>::value;
    data_sizeof_ = sizeof(T);

    GLint bound_buffer;
    glGetIntegerv(MapTypeToBindingType(type_), &bound_buffer);
    glBindBuffer(type_, id_);
    glBufferData(type_, data_sizeof_ * number_of_elements, data, usage_);
    glBindBuffer(type_, bound_buffer);
  }

  void Bind() const { glBindBuffer(type_, id_); }

  void UnBind(OpenGlObject::IdType id_to_bind = 0u) const {
    glBindBuffer(type_, id_to_bind);
  }

  Buffer::Type type() const { return static_cast<Buffer::Type>(type_); }
  GLint gl_type() const { return type_; }
  GLint gl_underlying_data_type() const { return gl_underlying_data_type_; }
  GLint components_per_vertex() const { return components_per_vertex_; }
  std::size_t data_sizeof() const { return data_sizeof_; }

 private:
  static inline GLenum MapTypeToBindingType(GLenum type) {
    switch (type) {
      case GL_ARRAY_BUFFER: return GL_ARRAY_BUFFER_BINDING;
      case GL_ELEMENT_ARRAY_BUFFER: return GL_ELEMENT_ARRAY_BUFFER_BINDING;
    }
    return 0;
  }

  GLenum type_{};
  GLenum usage_{};

  GLint gl_underlying_data_type_{};
  GLint components_per_vertex_{};
  std::size_t data_sizeof_{};
};

}  // namespace gl_tutorials

#endif  // OPENGL_TUTORIALS_CORE_VERTEX_BUFFER_H_
