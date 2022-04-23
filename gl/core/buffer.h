#ifndef OPENGL_TUTORIALS_CORE_VERTEX_BUFFER_H_
#define OPENGL_TUTORIALS_CORE_VERTEX_BUFFER_H_

#include "gl/core/opengl_object.h"
#include "gl/core/traits.h"
#include "utils/type_traits.h"

#include <map>
#include <vector>

namespace gl {

// TODO(igor): probably replace the globject with traits.
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
      : type_{static_cast<GLenum>(type)}, usage_{static_cast<GLenum>(usage)} {
    glGenBuffers(1, &id_);
  }

  template <typename T>
  Buffer(Buffer::Type type,
         Buffer::Usage usage,
         const T* const data,
         std::size_t number_of_elements)
      : Buffer{type, usage} {
    AssignData(data, number_of_elements);
  }

  template <typename T, typename A>
  Buffer(Buffer::Type type,
         Buffer::Usage usage,
         const std::vector<T, A>& vertices)
      : Buffer{type, usage, vertices.data(), vertices.size()} {}

  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;

  Buffer(Buffer&& other) { *this = std::move(other); }

  Buffer& operator=(Buffer&& other) {
    if (this == &other) { return *this; }
    id_ = other.id_;
    type_ = other.type_;
    usage_ = other.usage_;

    gl_underlying_data_type_ = other.gl_underlying_data_type_;
    components_per_vertex_ = other.components_per_vertex_;
    data_sizeof_ = other.data_sizeof_;
    number_of_elements_ = other.number_of_elements_;
    other.id_ = 0u;
    return *this;
  }

  ~Buffer() {
    if (!id_) { return; }
    // If the id is valid, then we have things to unbind.
    UnBind();
    glDeleteBuffers(1, &id_);
  }

  template <typename T, typename A>
  void AssignData(const std::vector<T, A>& vertices) {
    AssignData(vertices.data(), vertices.size());
  }

  template <typename T>
  void AssignData(const T* const data, std::size_t number_of_elements) {
    static_assert(::traits::has_value_member<
                      typename traits::number_of_entries<T>>::value,
                  "Missing specialization for trait 'number_of_entries'");
    static_assert(::traits::has_value_member<
                      typename traits::gl_underlying_type<T>>::value,
                  "Missing specialization for trait 'gl_underlying_type'");
    components_per_vertex_ = traits::number_of_entries<T>::value;
    gl_underlying_data_type_ = traits::gl_underlying_type<T>::value;
    data_sizeof_ = sizeof(T);
    number_of_elements_ = number_of_elements;

    const auto previously_bound_buffer{Bind()};
    glBufferData(type_, data_sizeof_ * number_of_elements, data, usage_);
    if (previously_bound_buffer != id_) {
      UnBindAndRebind(previously_bound_buffer);
    }
  }

  inline GLuint Bind() const {
    GLuint bound_buffer{GetCurrentlyBoundBuffer(type_)};
    if (bound_buffer != id_) { glBindBuffer(type_, id_); }
    return bound_buffer;
  }

  inline void UnBind() const { UnBindAndRebind(0u); }

  inline void UnBindAndRebind(OpenGlObject::IdType id_to_bind = 0u) const {
    glBindBuffer(type_, id_to_bind);
  }

  inline Buffer::Type type() const { return static_cast<Buffer::Type>(type_); }
  inline Buffer::Usage usage() const {
    return static_cast<Buffer::Usage>(usage_);
  }
  inline GLint gl_type() const { return type_; }
  inline GLint gl_underlying_data_type() const {
    return gl_underlying_data_type_;
  }
  inline GLint components_per_vertex() const { return components_per_vertex_; }
  inline std::size_t data_sizeof() const { return data_sizeof_; }
  inline std::size_t number_of_elements() const { return number_of_elements_; }

 private:
  static inline GLuint GetCurrentlyBoundBuffer(GLenum type) {
    GLint bound_buffer;
    glGetIntegerv(MapTypeToBindingType(type), &bound_buffer);
    return static_cast<GLuint>(bound_buffer);
  }

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
  std::size_t number_of_elements_{};
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_CORE_VERTEX_BUFFER_H_
