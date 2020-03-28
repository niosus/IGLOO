#ifndef OPENGL_TUTORIALS_CORE_VERTEX_ARRAY_BUFFER_H_
#define OPENGL_TUTORIALS_CORE_VERTEX_ARRAY_BUFFER_H_

#include "gl/core/buffer.h"
#include "gl/core/opengl_object.h"

#include <array>
#include <iostream>
#include <memory>

namespace gl {

class VertexArrayBuffer : public OpenGlObject {
 public:
  VertexArrayBuffer() : OpenGlObject{0} { glGenVertexArrays(1, &id_); }

  void AssignBuffer(const std::shared_ptr<Buffer>& buffer) {
    Bind();
    auto& bound_buffer = bound_buffers_[GetBoundBufferIndex(buffer->type())];
    if (bound_buffer) { bound_buffer->UnBind(); }
    bound_buffer = buffer;
    bound_buffer->Bind();
    UnBind();
  }

  void Bind() {
    // TODO(igor): check if something was bound before. We don't want to change
    // the state.
    glBindVertexArray(id_);
  }
  void UnBind() { glBindVertexArray(0u); }

  bool Draw(int gl_primitive_mode) {
    int number_of_elements{};
    int gl_type{};
    const auto& indices_buffer =
        bound_buffers_[GetBoundBufferIndex(Buffer::Type::kElementArrayBuffer)];
    if (indices_buffer) {
      number_of_elements = indices_buffer->number_of_elements();
      gl_type = indices_buffer->gl_underlying_data_type();
    } else {
      const auto& vertex_buffer =
          bound_buffers_[GetBoundBufferIndex(Buffer::Type::kArrayBuffer)];
      if (!vertex_buffer) { return false; }
      number_of_elements = vertex_buffer->number_of_elements();
      gl_type = vertex_buffer->gl_underlying_data_type();
    }
    Bind();
    glDrawElements(gl_primitive_mode, number_of_elements, gl_type, 0);
    UnBind();
    return true;
  }

  bool EnableVertexAttributePointer(int layout_index,
                                    int stride = 1,
                                    int offset = 0,
                                    int override_component_count = 1,
                                    bool normalized = false) {
    auto& bound_vertex_buffer =
        bound_buffers_[GetBoundBufferIndex(Buffer::Type::kArrayBuffer)];
    if (!bound_vertex_buffer) { return false; }
    Bind();
    glVertexAttribPointer(
        layout_index,
        override_component_count * bound_vertex_buffer->components_per_vertex(),
        bound_vertex_buffer->gl_underlying_data_type(),
        normalized ? GL_TRUE : GL_FALSE,
        stride * bound_vertex_buffer->data_sizeof(),
        reinterpret_cast<void*>(offset * bound_vertex_buffer->data_sizeof()));
    glEnableVertexAttribArray(layout_index);
    UnBind();
    return true;
  }

  ~VertexArrayBuffer() { glDeleteVertexArrays(1, &id_); }

 private:
  static inline std::size_t GetBoundBufferIndex(Buffer::Type type) {
    switch (type) {
      case Buffer::Type::kArrayBuffer: return 0;
      case Buffer::Type::kElementArrayBuffer: return 1;
    }
    // Todo(igor): transition to optional?
    return 666;
  }

  std::array<std::shared_ptr<Buffer>, 2ul> bound_buffers_{};
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_CORE_VERTEX_ARRAY_BUFFER_H_
