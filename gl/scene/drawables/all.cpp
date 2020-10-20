// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "gl/scene/drawables/all.h"

#include "gl/scene/program_pool.h"

#include <algorithm>
#include <vector>

namespace gl {

Points::Points(const ProgramPool& program_pool,
               const eigen::vector<Eigen::Vector3f>& points,
               const Eigen::Vector3f& color,
               float point_size,
               GLenum gl_mode)
    : Points{program_pool,
             points,
             std::vector<float>(points.size(), 1.0f),
             color,
             point_size,
             gl_mode} {}

Points::Points(const ProgramPool& program_pool,
               const eigen::vector<Eigen::Vector3f>& points,
               const std::vector<float>& intensities,
               const Eigen::Vector3f& color,
               float point_size,
               GLenum gl_mode)
    : Drawable{program_pool,
               ProgramPool::ProgramType::DRAW_POINTS,
               Style::DRAW_3D,
               gl_mode,
               point_size,
               color},
      points_{points},
      intensities_{intensities} {}

void Points::FillBuffers() {
  CHECK(program_) << "Cannot fill buffers without an active program.";
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
  program_->Use();
  color_uniform_index_ = program_->SetUniform("color", color_);
  model_uniform_index_ =
      program_->SetUniform("model", Eigen::Matrix4f::Identity());
  projection_view_uniform_index_ =
      program_->SetUniform("proj_view", Eigen::Matrix4f::Identity());
  ready_to_draw_ = true;
}

Lines::Lines(const ProgramPool& program_pool,
             const eigen::vector<Eigen::Vector3f>& points,
             const Eigen::Vector3f& color,
             float line_width)
    : Points{program_pool, points, color, line_width, GL_LINES} {}

CoordinateSystem::CoordinateSystem(const ProgramPool& program_pool)
    : Drawable(program_pool,
               ProgramPool::ProgramType::DRAW_COORDINATE_SYSTEM,
               Style::DRAW_3D,
               GL_POINTS) {}

void CoordinateSystem::FillBuffers() {
  CHECK(program_) << "Cannot fill buffers without an active program.";
  std::vector<Eigen::Vector4f> points{{0, 0, 0, 1}};

  vao_ = std::make_unique<VertexArrayBuffer>();
  vao_->EnableVertexAttributePointer(
      0,
      std::make_shared<gl::Buffer>(gl::Buffer::Type::kArrayBuffer,
                                   gl::Buffer::Usage::kStaticDraw,
                                   points));
  model_uniform_index_ =
      program_->SetUniform("model", Eigen::Matrix4f::Identity());
  projection_view_uniform_index_ =
      program_->SetUniform("proj_view", Eigen::Matrix4f::Identity());
  ready_to_draw_ = true;
}

RectWithTexture::RectWithTexture(const ProgramPool& program_pool,
                                 const utils::Image& data,
                                 const Eigen::Vector3f& bottom_left,
                                 const Eigen::Vector2f& size,
                                 Style draw_style)
    : Drawable{program_pool,
               ProgramPool::ProgramType::DRAW_TEXTURED_RECT,
               draw_style,
               GL_POINTS},
      data_{data},
      bottom_left_{bottom_left},
      size_{size} {}

RectWithTexture::RectWithTexture(const ProgramPool& program_pool,
                                 const utils::Image& data,
                                 const Eigen::Vector3f& bottom_left,
                                 const Eigen::Vector2f& size)
    : RectWithTexture{program_pool, data, bottom_left, size, Style::DRAW_3D} {}

RectWithTexture::RectWithTexture(const ProgramPool& program_pool,
                                 const utils::Image& data,
                                 const Eigen::Vector2f& bottom_left,
                                 const Eigen::Vector2f& size)
    : RectWithTexture{program_pool,
                      data,
                      {bottom_left.x(), bottom_left.y(), -1.0f},
                      size,
                      Style::DRAW_2D} {
  CHECK_LE(bottom_left.x() + size.x(), 1.0f) << "Drawing outside of viewport.";
  CHECK_LE(bottom_left.y() + size.y(), 1.0f) << "Drawing outside of viewport.";
  CHECK_GE(bottom_left.x(), -1.0f) << "Drawing outside of viewport.";
  CHECK_GE(bottom_left.y(), -1.0f) << "Drawing outside of viewport.";
}

void RectWithTexture::FillBuffers() {
  CHECK(program_) << "Cannot fill buffers without an active program.";
  // TODO(igor): for now it is always the texture 0. Is this fine?
  texture_ = Texture::Builder(Texture::Type::kTexture2D,
                              Texture::Identifier::kTexture0)
                 .WithImage(data_)
                 .WithSaneDefaults()
                 .Build();

  const std::vector<Eigen::Vector3f> raw = {bottom_left_};

  vao_ = std::make_unique<VertexArrayBuffer>();
  vao_->EnableVertexAttributePointer(
      0,
      std::make_shared<gl::Buffer>(
          gl::Buffer::Type::kArrayBuffer, gl::Buffer::Usage::kStaticDraw, raw));
  program_->Use();
  color_uniform_index_ = program_->SetUniform("color", color_);
  model_uniform_index_ =
      program_->SetUniform("model", Eigen::Matrix4f::Identity());
  projection_view_uniform_index_ =
      program_->SetUniform("proj_view", Eigen::Matrix4f::Identity());

  sampler_index_ = 0;  // This should match the number of the texture we picked.
  program_->SetUniform("source", sampler_index_);
  program_->SetUniform("rect_size", size_);
  ready_to_draw_ = true;
}

// Text::Text(const std::string& text,
//            const std::string& font_name,
//            const Eigen::Vector3f& pos,
//            float scale,
//            Style draw_style)
//     : Drawable{draw_style, GL_TRIANGLES},
//       text_{text},
//       font_name_{font_name},
//       pos_{pos},
//       scale_{scale} {}

// Text::Text(const std::string& text,
//            const std::string& font_name,
//            const Eigen::Vector3f& pos,
//            float scale)
//     : Text(text, font_name, pos, scale, Style::DRAW_3D) {}
// Text::Text(const std::string& text,
//            const std::string& font_name,
//            const glow::Eigen::Vector2f& pos,
//            float scale)
//     : Text(text, font_name, {pos.x, pos.y, -1.0f}, scale, Style::DRAW_2D) {}

// void Text::FillBuffers() {
//   CHECK(FontPool::Instance().HasFont(font_name_));
//   auto font = FontPool::Instance().Get(font_name_);
//   const utils::Image& texture = font->texture();

//   program_ = ProgramPool::Get(ProgramPool::ProgramType::DRAW_TEXT);

//   texture_ = make_unique<glow::GlTexture>(
//       texture.cols, texture.rows, glow::TextureFormat::RGBA);
//   texture_->assign(PixelFormat::BGRA, PixelType::UNSIGNED_BYTE,
//   texture.data);

//   std::vector<Eigen::Vector2f> rect_coords;
//   rect_coords.reserve(text_.size());
//   std::vector<Eigen::Vector2f> texture_coords;
//   texture_coords.reserve(text_.size());
//   float x_start = 0.0f;
//   float y_start = 0.0f;
//   float char_height = 0.0f;
//   for (char c : text_) {
//     if (c == '\n') {
//       x_start = 0.0f;
//       y_start += scale_ * char_height;
//       continue;
//     }
//     if (c == '\t') { c = ' '; }
//     const auto& texture_pos = font->GetCharCoords(c);
//     const float x = texture_pos.x;
//     const float y = texture_pos.y;
//     const float w = texture_pos.width;
//     const float h = texture_pos.height;

//     rect_coords.emplace_back(x_start, y_start);
//     rect_coords.emplace_back(x_start, y_start + scale_ * h);
//     rect_coords.emplace_back(x_start + scale_ * w, y_start + scale_ * h);
//     rect_coords.emplace_back(x_start, y_start);
//     rect_coords.emplace_back(x_start + scale_ * w, y_start + scale_ * h);
//     rect_coords.emplace_back(x_start + scale_ * w, y_start);
//     texture_coords.emplace_back(x, y);
//     texture_coords.emplace_back(x, y + h);
//     texture_coords.emplace_back(x + w, y + h);
//     texture_coords.emplace_back(x, y);
//     texture_coords.emplace_back(x + w, y + h);
//     texture_coords.emplace_back(x + w, y);
//     x_start += scale_ * w;
//     if (h < char_height) { char_height = h; }
//   }

//   num_points_ = rect_coords.size();

//   vao_ = make_unique<GlVertexArray>();
//   vao_->bind();
//   SetVertexAttribute(0, rect_coords, vao_.get());
//   SetVertexAttribute(1, texture_coords, vao_.get());
//   vao_->release();

//   sampler_index_ = 1;
//   uniforms_to_override_.emplace_back(
//       make_unique<GlUniform<int>>("source", sampler_index_));
//   uniforms_to_override_.emplace_back(
//       make_unique<GlUniform<Eigen::Vector3f>>("anchor", pos_));
//   ready_to_draw_ = true;
// }

}  // namespace gl
