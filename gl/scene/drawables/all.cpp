// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "gl/scene/drawables/all.h"
#include "gl/scene/font_pool.h"
#include "gl/scene/program_pool.h"

#include <algorithm>
#include <vector>

namespace gl {

Points::Points(ProgramPool* program_pool,
               ProgramPool::ProgramIndex program_index,
               const eigen::vector<Eigen::Vector3f>& points,
               const Eigen::Vector3f& color,
               float point_size,
               GLenum gl_mode)
    : Points{program_pool,
             program_index,
             points,
             std::vector<float>(points.size(), 1.0f),
             color,
             point_size,
             gl_mode} {}

Points::Points(ProgramPool* program_pool,
               ProgramPool::ProgramIndex program_index,
               const eigen::vector<Eigen::Vector3f>& points,
               const std::vector<float>& intensities,
               const Eigen::Vector3f& color,
               float point_size,
               GLenum gl_mode)
    : Drawable{program_pool, program_index, gl_mode, point_size, color},
      points_{points},
      intensities_{intensities} {}

void Points::FillBuffers() {
  CHECK(program_pool_) << "Cannot fill buffers without a program pool.";
  CHECK(program_index_) << "Cannot fill buffers without an active program.";
  CHECK_EQ(points_.size(), intensities_.size());

  vao_ = std::make_unique<VertexArrayBuffer>();
  vao_->EnableVertexAttributePointer(
      0,
      std::make_shared<gl::Buffer>(gl::Buffer::Type::kArrayBuffer,
                                   gl::Buffer::Usage::kStaticDraw,
                                   points_));
  vao_->EnableVertexAttributePointer(
      1,
      std::make_shared<gl::Buffer>(gl::Buffer::Type::kArrayBuffer,
                                   gl::Buffer::Usage::kStaticDraw,
                                   intensities_));
  program_pool_->UseProgram(program_index_.value());
  color_uniform_index_ =
      program_pool_->SetUniformToActiveProgram("color", color_);
  model_uniform_index_ = program_pool_->SetUniformToActiveProgram(
      "model", Eigen::Matrix4f::Identity());
  projection_view_uniform_index_ = program_pool_->SetUniformToActiveProgram(
      "proj_view", Eigen::Matrix4f::Identity());
  ready_to_draw_ = true;
}

Lines::Lines(ProgramPool* program_pool,
             ProgramPool::ProgramIndex program_index,
             const eigen::vector<Eigen::Vector3f>& points,
             const Eigen::Vector3f& color,
             float line_width)
    : Points{program_pool, program_index, points, color, line_width, GL_LINES} {
}

CoordinateSystem::CoordinateSystem(ProgramPool* program_pool,
                                   ProgramPool::ProgramIndex program_index)
    : Drawable(program_pool, program_index, GL_POINTS) {}

void CoordinateSystem::FillBuffers() {
  CHECK(program_pool_) << "Cannot fill buffers without a program pool.";
  CHECK(program_index_) << "Cannot fill buffers without an active program.";
  std::vector<Eigen::Vector4f> points{{0, 0, 0, 1}};

  vao_ = std::make_unique<VertexArrayBuffer>();
  vao_->EnableVertexAttributePointer(
      0,
      std::make_shared<gl::Buffer>(gl::Buffer::Type::kArrayBuffer,
                                   gl::Buffer::Usage::kStaticDraw,
                                   points));
  program_pool_->UseProgram(program_index_.value());
  model_uniform_index_ = program_pool_->SetUniformToActiveProgram(
      "model", Eigen::Matrix4f::Identity());
  projection_view_uniform_index_ = program_pool_->SetUniformToActiveProgram(
      "proj_view", Eigen::Matrix4f::Identity());
  ready_to_draw_ = true;
}

RectWithTexture::RectWithTexture(ProgramPool* program_pool,
                                 ProgramPool::ProgramIndex program_index,
                                 std::shared_ptr<gl::Texture> texture,
                                 const Eigen::Vector2f& size)
    : Drawable{program_pool, program_index, GL_POINTS}, size_{size} {
  texture_ = texture;
}

void RectWithTexture::FillBuffers() {
  CHECK(program_pool_) << "Cannot fill buffers without a program pool.";
  CHECK(program_index_) << "Cannot fill buffers without an active program.";

  const std::vector<Eigen::Vector3f> raw = {{0.0F, 0.0F, 0.0F}};
  vao_ = std::make_unique<VertexArrayBuffer>();
  vao_->EnableVertexAttributePointer(
      0,
      std::make_shared<gl::Buffer>(
          gl::Buffer::Type::kArrayBuffer, gl::Buffer::Usage::kStaticDraw, raw));

  program_pool_->UseProgram(program_index_.value());
  (void)program_pool_->SetUniformToActiveProgram("source", 0);
  (void)program_pool_->SetUniformToActiveProgram("rect_size", size_);
  model_uniform_index_ = program_pool_->SetUniformToActiveProgram(
      "model", Eigen::Matrix4f::Identity());
  projection_view_uniform_index_ = program_pool_->SetUniformToActiveProgram(
      "proj_view", Eigen::Matrix4f::Identity());
  ready_to_draw_ = true;
}

ScreenRectWithTexture::ScreenRectWithTexture(
    ProgramPool* program_pool,
    ProgramPool::ProgramIndex program_index,
    std::shared_ptr<gl::Texture> texture,
    const Eigen::Vector2f& size)
    : Drawable{program_pool, program_index, GL_POINTS}, size_{size} {
  texture_ = texture;
}

void ScreenRectWithTexture::FillBuffers() {
  CHECK(program_pool_) << "Cannot fill buffers without a program pool.";
  CHECK(program_index_) << "Cannot fill buffers without an active program.";

  const std::vector<Eigen::Vector3f> raw = {{0.0F, 0.0F, 0.0F}};
  vao_ = std::make_unique<VertexArrayBuffer>();
  vao_->EnableVertexAttributePointer(
      0,
      std::make_shared<gl::Buffer>(
          gl::Buffer::Type::kArrayBuffer, gl::Buffer::Usage::kStaticDraw, raw));

  program_pool_->UseProgram(program_index_.value());
  (void)program_pool_->SetUniformToActiveProgram("source", 0);
  (void)program_pool_->SetUniformToActiveProgram("rect_size", size_);
  model_uniform_index_ = program_pool_->SetUniformToActiveProgram(
      "model", Eigen::Matrix4f::Identity());
  projection_view_uniform_index_ = program_pool_->SetUniformToActiveProgram(
      "proj_view", Eigen::Matrix4f::Identity());
  ready_to_draw_ = true;
}

Text::Text(ProgramPool* program_pool,
           ProgramPool::ProgramIndex program_index,
           const std::string& text,
           const std::string& font_name,
           float scale)
    : Drawable{program_pool, program_index, GL_TRIANGLES},
      text_{text},
      font_name_{font_name},
      scale_{scale} {}

void Text::FillBuffers() {
  CHECK(program_pool_) << "Cannot fill buffers without a program pool.";
  CHECK(program_index_) << "Cannot fill buffers without an active program.";

  CHECK(FontPool::Instance().HasFont(font_name_));
  auto font = FontPool::Instance().Get(font_name_);

  std::vector<Eigen::Vector2f> rect_coords;
  rect_coords.reserve(text_.size());
  std::vector<Eigen::Vector2f> texture_coords;
  texture_coords.reserve(text_.size());
  float x_start = 0.0f;
  float y_start = 0.0f;
  float char_height = 0.0f;
  for (char c : text_) {
    if (c == '\n') {
      x_start = 0.0f;
      y_start += scale_ * char_height;
      continue;
    }
    if (c == '\t') { c = ' '; }
    const auto& texture_pos = font->GetCharCoords(c);
    const float x = texture_pos.x;
    const float y = texture_pos.y;
    const float w = texture_pos.width;
    const float h = texture_pos.height;

    rect_coords.emplace_back(x_start, y_start);
    rect_coords.emplace_back(x_start, y_start + scale_ * h);
    rect_coords.emplace_back(x_start + scale_ * w, y_start + scale_ * h);
    rect_coords.emplace_back(x_start, y_start);
    rect_coords.emplace_back(x_start + scale_ * w, y_start + scale_ * h);
    rect_coords.emplace_back(x_start + scale_ * w, y_start);
    texture_coords.emplace_back(x, y);
    texture_coords.emplace_back(x, y + h);
    texture_coords.emplace_back(x + w, y + h);
    texture_coords.emplace_back(x, y);
    texture_coords.emplace_back(x + w, y + h);
    texture_coords.emplace_back(x + w, y);
    x_start += scale_ * w;
    if (h < char_height) { char_height = h; }
  }

  vao_ = std::make_unique<VertexArrayBuffer>();
  vao_->EnableVertexAttributePointer(
      0,
      std::make_shared<gl::Buffer>(gl::Buffer::Type::kArrayBuffer,
                                   gl::Buffer::Usage::kStaticDraw,
                                   rect_coords));
  vao_->EnableVertexAttributePointer(
      1,
      std::make_shared<gl::Buffer>(gl::Buffer::Type::kArrayBuffer,
                                   gl::Buffer::Usage::kStaticDraw,
                                   texture_coords));

  texture_ = gl::Texture::Builder{gl::Texture::Type::kTexture2D,
                                  gl::Texture::Identifier::kTexture1}
                 .WithSaneDefaults()
                 .WithImage(font->texture())
                 .Build();

  program_pool_->UseProgram(program_index_.value());
  (void)program_pool_->SetUniformToActiveProgram("source", 1);
  model_uniform_index_ = program_pool_->SetUniformToActiveProgram(
      "model", Eigen::Matrix4f::Identity());
  projection_view_uniform_index_ = program_pool_->SetUniformToActiveProgram(
      "proj_view", Eigen::Matrix4f::Identity());

  ready_to_draw_ = true;
}

}  // namespace gl
