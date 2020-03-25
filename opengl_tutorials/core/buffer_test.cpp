#include "opengl_tutorials/core/buffer.h"
#include "opengl_tutorials/utils/eigen_utils.h"
#include "gtest/gtest.h"

using namespace gl_tutorials;

namespace {
GLuint GetCurrentlyBoundBuffer(GLenum binding_type) {
  GLint bound_buffer;
  glGetIntegerv(binding_type, &bound_buffer);
  return static_cast<GLuint>(bound_buffer);
  }
}

TEST(BufferTest, Init) {
  Buffer buffer{Buffer::Type::kArrayBuffer, Buffer::Usage::kStaticDraw};
  EXPECT_NE(0u, buffer.id());
  EXPECT_EQ(Buffer::Type::kArrayBuffer, buffer.type());
  EXPECT_EQ(GL_ARRAY_BUFFER, buffer.gl_type());
  EXPECT_EQ(Buffer::Usage::kStaticDraw, buffer.usage());
  EXPECT_EQ(0, buffer.gl_underlying_data_type());
  EXPECT_EQ(0, buffer.components_per_vertex());
  EXPECT_EQ(0ul, buffer.data_sizeof());
  EXPECT_EQ(0ul, buffer.number_of_elements());
}

TEST(BufferTest, AssignData) {
  Buffer buffer{Buffer::Type::kArrayBuffer, Buffer::Usage::kStaticDraw};
  std::vector<float> data{1, 2, 3, 4};
  buffer.AssignData(data);
  EXPECT_EQ(Buffer::Type::kArrayBuffer, buffer.type());
  EXPECT_EQ(GL_ARRAY_BUFFER, buffer.gl_type());
  EXPECT_EQ(Buffer::Usage::kStaticDraw, buffer.usage());
  EXPECT_EQ(GL_FLOAT, buffer.gl_underlying_data_type());
  EXPECT_EQ(1, buffer.components_per_vertex());
  EXPECT_EQ(sizeof(float), buffer.data_sizeof());
  EXPECT_EQ(4, buffer.number_of_elements());
}

TEST(BufferTest, AssignDataContructor) {
  std::vector<float> data{1, 2, 3, 4};
  Buffer buffer{Buffer::Type::kArrayBuffer, Buffer::Usage::kStaticDraw, data};
  EXPECT_EQ(Buffer::Type::kArrayBuffer, buffer.type());
  EXPECT_EQ(GL_ARRAY_BUFFER, buffer.gl_type());
  EXPECT_EQ(Buffer::Usage::kStaticDraw, buffer.usage());
  EXPECT_EQ(GL_FLOAT, buffer.gl_underlying_data_type());
  EXPECT_EQ(1, buffer.components_per_vertex());
  EXPECT_EQ(sizeof(float), buffer.data_sizeof());
  EXPECT_EQ(4, buffer.number_of_elements());
}

TEST(BufferTest, AssignDataInVectors) {
  Buffer buffer{Buffer::Type::kArrayBuffer, Buffer::Usage::kStaticDraw};
  std::vector<Eigen::Vector3i> data{{1, 2, 3}, {4, 5, 6}};
  buffer.AssignData(data);
  ASSERT_EQ(12, sizeof(Eigen::Vector3i))
      << "The data in Eigen vector is weirdly packed.";
  EXPECT_EQ(Buffer::Type::kArrayBuffer, buffer.type());
  EXPECT_EQ(GL_ARRAY_BUFFER, buffer.gl_type());
  EXPECT_EQ(Buffer::Usage::kStaticDraw, buffer.usage());
  EXPECT_EQ(GL_INT, buffer.gl_underlying_data_type());
  EXPECT_EQ(3, buffer.components_per_vertex());
  EXPECT_EQ(sizeof(Eigen::Vector3i), buffer.data_sizeof());
  EXPECT_EQ(2, buffer.number_of_elements());
}

TEST(BufferTest, BindUnbind) {
  Buffer buffer_1{Buffer::Type::kArrayBuffer, Buffer::Usage::kStaticDraw};
  Buffer buffer_2{Buffer::Type::kArrayBuffer, Buffer::Usage::kStaticDraw};
  EXPECT_NE(0u, buffer_1.id());
  EXPECT_NE(0u, buffer_2.id());
  EXPECT_NE(buffer_1.id(), buffer_2.id());
  ASSERT_EQ(0u, GetCurrentlyBoundBuffer(GL_ARRAY_BUFFER_BINDING));
  auto prev_bound_buffer{buffer_1.Bind()};
  ASSERT_EQ(buffer_1.id(), GetCurrentlyBoundBuffer(GL_ARRAY_BUFFER_BINDING));
  ASSERT_EQ(0, prev_bound_buffer);
  prev_bound_buffer = buffer_2.Bind();
  ASSERT_EQ(buffer_2.id(), GetCurrentlyBoundBuffer(GL_ARRAY_BUFFER_BINDING));
  ASSERT_EQ(buffer_1.id(), prev_bound_buffer);
  buffer_2.UnBindAndRebind(buffer_1.id());
  ASSERT_EQ(buffer_1.id(), GetCurrentlyBoundBuffer(GL_ARRAY_BUFFER_BINDING));
  buffer_2.UnBind();
  ASSERT_EQ(0, GetCurrentlyBoundBuffer(GL_ARRAY_BUFFER_BINDING));
}

