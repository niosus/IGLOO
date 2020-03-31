#ifndef OPENGL_TUTORIALS_CORE_VERTEX_ARRAY_BUFFER_H_
#define OPENGL_TUTORIALS_CORE_VERTEX_ARRAY_BUFFER_H_

#include "gl/core/buffer.h"
#include "gl/core/opengl_object.h"

#include "glog/logging.h"

#include <array>
#include <iostream>
#include <memory>

namespace gl {

class VertexArrayBuffer : public OpenGlObject {
 public:
  VertexArrayBuffer() : OpenGlObject{0} { glGenVertexArrays(1, &id_); }

  const Buffer* AssignBuffer(const std::shared_ptr<Buffer>& buffer) {
    if (buffer->type() == Buffer::Type::kElementArrayBuffer) {
      CHECK(bound_buffers_[buffer->type()].empty())
          << "Multiple GL_ELEMENT_ARRAY_BUFFERs are not allowed.";
      indices_present_ = true;
      gl_indices_type_ = buffer->gl_underlying_data_type();
      number_of_elements_to_draw_ = buffer->number_of_elements();
    } else {
      CHECK(GetStoredBuffer(buffer->type(), buffer->id()) == nullptr)
          << "This buffer is already stored.";
      if (!indices_present_) {
        // We only want to update this number here if there are no indices
        // present.
        number_of_elements_to_draw_ = buffer->number_of_elements();
      }
    }
    bound_buffers_[buffer->type()].emplace(buffer->id(), buffer);
    Bind();
    buffer->Bind();
    UnBind();
    return buffer.get();
  }

  bool EnableVertexAttributePointer(int layout_index,
                                    const std::shared_ptr<Buffer>& buffer,
                                    int stride = 1,
                                    int offset = 0,
                                    bool normalized = false) {
    const auto* buffer_ptr = GetStoredBuffer(buffer->type(), buffer->id());
    if (!buffer_ptr) { buffer_ptr = AssignBuffer(buffer); }
    Bind();
    glVertexAttribPointer(
        layout_index,
        buffer_ptr->components_per_vertex(),
        buffer_ptr->gl_underlying_data_type(),
        normalized ? GL_TRUE : GL_FALSE,
        stride * buffer_ptr->data_sizeof(),
        reinterpret_cast<void*>(offset * buffer_ptr->data_sizeof()));
    glEnableVertexAttribArray(layout_index);
    UnBind();
    return true;
  }

  bool EnableVertexAttributePointer(int layout_index,
                                    int stride = 1,
                                    int offset = 0,
                                    int override_component_count = 1,
                                    bool normalized = false) {
    CHECK_EQ(bound_buffers_[Buffer::Type::kArrayBuffer].size(), 1ul)
        << "This is a low-level interface. Use it only if you have a single "
           "array buffer bound.";
    const auto& buffer =
        bound_buffers_.at(Buffer::Type::kArrayBuffer).begin()->second;
    Bind();
    glVertexAttribPointer(
        layout_index,
        override_component_count * buffer->components_per_vertex(),
        buffer->gl_underlying_data_type(),
        normalized ? GL_TRUE : GL_FALSE,
        stride * buffer->data_sizeof(),
        reinterpret_cast<void*>(offset * buffer->data_sizeof()));
    glEnableVertexAttribArray(layout_index);
    UnBind();
    return true;
  }

  void Bind() { glBindVertexArray(id_); }
  void UnBind() { glBindVertexArray(0u); }

  bool Draw(GLint gl_primitive_mode, int stride = 1) {
    CHECK(!bound_buffers_.empty()) << "There are no buffers to draw.";
    Bind();
    if (indices_present_) {
      glDrawElements(
          gl_primitive_mode, number_of_elements_to_draw_, gl_indices_type_, 0);
    } else {
      glDrawArrays(gl_primitive_mode, 0, number_of_elements_to_draw_ / stride);
    }
    UnBind();
    return true;
  }

  ~VertexArrayBuffer() { glDeleteVertexArrays(1, &id_); }

 private:
  Buffer* GetStoredBuffer(Buffer::Type buffer_type,
                          OpenGlObject::IdType id) const {
    const auto buffers_iter = bound_buffers_.find(buffer_type);
    if (buffers_iter == bound_buffers_.end()) { return nullptr; }
    const auto buffer_iter = buffers_iter->second.find(id);
    if (buffer_iter == buffers_iter->second.end()) { return nullptr; }
    return buffer_iter->second.get();
  }

  bool indices_present_{false};
  GLint gl_indices_type_{};
  GLint number_of_elements_to_draw_{};

  std::map<Buffer::Type,
           std::map<OpenGlObject::IdType, std::shared_ptr<Buffer>>>
      bound_buffers_{};
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_CORE_VERTEX_ARRAY_BUFFER_H_
