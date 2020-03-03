#ifndef OPENGL_TUTORIALS_CORE_VERTEX_ARRAY_BUFFER_H_
#define OPENGL_TUTORIALS_CORE_VERTEX_ARRAY_BUFFER_H_

#include "opengl_tutorials/core/buffer.h"
#include "opengl_tutorials/core/gl_base.h"

#include <array>
#include <memory>

namespace gl_tutorials {

class VertexArrayBuffer : public OpenGlObject {
 public:
  VertexArrayBuffer() : OpenGlObject{0} { glGenVertexArrays(1, &id_); }

  void BindBuffer(const std::shared_ptr<Buffer> &buffer) {
    Bind();
    auto &bound_buffer = bound_buffers_[GetBoundBufferIndex(buffer->type())];
    if (bound_buffer) { bound_buffer->UnBind(); }
    bound_buffer = buffer;
    bound_buffer->Bind();
    UnBind();
  }

  void Bind() { glBindVertexArray(id_); }
  void UnBind() { glBindVertexArray(0u); }

  bool EnableAttributePointer(int layout_index,
                              bool normalized = false,
                              void *offset = nullptr) {
    Bind();
    auto &bound_vertex_buffer =
        bound_buffers_[GetBoundBufferIndex(Buffer::Type::kArrayBuffer)];
    if (!bound_vertex_buffer) { return false; }
    glVertexAttribPointer(layout_index,
                          bound_vertex_buffer->components_per_vertex(),
                          bound_vertex_buffer->gl_underlying_data_type(),
                          normalized ? GL_TRUE : GL_FALSE,
                          bound_vertex_buffer->data_sizeof(),
                          offset);
    glEnableVertexAttribArray(layout_index);
    UnBind();
    return true;
  }

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

}  // namespace gl_tutorials

#endif  // OPENGL_TUTORIALS_CORE_VERTEX_ARRAY_BUFFER_H_
