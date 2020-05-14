// Copyright Igor Bogoslavskyi, year 2018.
// In case of any problems with the code please contact me.
// Email: igor.bogoslavskyi@uni-bonn.de.

#include <ipb_opengl_tools/gl/drawables/all.h>

#include <ipb_core/utils/cv_utils.h>
#include <ipb_opengl_tools/gl/font_pool.h>
#include <ipb_opengl_tools/gl/glow_helper.h>

#include <algorithm>

namespace ipb {
namespace vis {

using Eigen::Matrix4f;
using Eigen::Vector3f;
using Eigen::Vector4f;
using glow::AttributeType;
using glow::BufferTarget;
using glow::BufferUsage;
using glow::GlBuffer;
using glow::GlUniform;
using glow::GlVertexArray;
using glow::PixelFormat;
using glow::PixelType;
using glow::vec2;
using glow::vec3;
using ipb::make_unique;

Points::Points(const ipb::EigenStdVector<Eigen::Vector3f>& points,
               const glow::vec3& color,
               float point_size,
               GLenum gl_mode)
    : Points{points,
             std::vector<float>(points.size(), 1.0f),
             color,
             point_size,
             gl_mode} {}

Points::Points(const ipb::EigenStdVector<Eigen::Vector3f>& points,
               const std::vector<float>& intensities,
               const glow::vec3& color,
               float point_size,
               GLenum gl_mode)
    : Drawable(Style::DRAW_3D, gl_mode, point_size, color),
      points_{points},
      intensities_{intensities} {}

void Points::FillBuffers() {
  CHECK_EQ(points_.size(), intensities_.size());
  num_points_ = points_.size();

  program_ = ProgramPool::Get(ProgramPool::ProgramType::DRAW_POINTS);

  vao_ = make_unique<GlVertexArray>();
  vao_->bind();
  SetVertexAttribute(0, points_, vao_.get());
  SetVertexAttribute(1, intensities_, vao_.get());
  // Set other attributes before unbinding.
  vao_->release();
  program_->setUniform(glow::GlUniform<glow::vec3>("color", color_));
  ready_to_draw_ = true;
}

Lines::Lines(const ipb::EigenStdVector<Eigen::Vector3f>& points,
             const glow::vec3& color,
             float line_width)
    : Points{points, color, line_width, GL_LINES} {}

CoordinateSystem::CoordinateSystem() : Drawable(Style::DRAW_3D, GL_POINTS) {}

void CoordinateSystem::FillBuffers() {
  program_ = ProgramPool::Get(ProgramPool::ProgramType::DRAW_COORDINATE_SYSTEM);
  std::vector<Vector4f> points{{0, 0, 0, 1}};
  num_points_ = points.size();

  vao_ = make_unique<GlVertexArray>();
  vao_->bind();
  SetVertexAttribute(0, points, vao_.get());
  vao_->release();
  ready_to_draw_ = true;
}

RectWithTexture::RectWithTexture(const cv::Mat& data,
                                 const vec3& bottom_left,
                                 const vec2& size,
                                 Style draw_style)
    : Drawable{draw_style, GL_POINTS},
      data_{data},
      bottom_left_{bottom_left},
      size_{size} {}

RectWithTexture::RectWithTexture(const cv::Mat& data,
                                 const vec3& bottom_left,
                                 const vec2& size)
    : RectWithTexture(data, bottom_left, size, Style::DRAW_3D) {}

RectWithTexture::RectWithTexture(const cv::Mat& data,
                                 const vec2& bottom_left,
                                 const vec2& size)
    : RectWithTexture(
          data, {bottom_left.x, bottom_left.y, -1.0f}, size, Style::DRAW_2D) {
  CHECK_LE(bottom_left.x + size.x, 1.0) << "Drawing outside of viewport.";
  CHECK_LE(bottom_left.y + size.y, 1.0) << "Drawing outside of viewport.";
  CHECK_GE(bottom_left.x, -1.0) << "Drawing outside of viewport.";
  CHECK_GE(bottom_left.y, -1.0) << "Drawing outside of viewport.";
}

void RectWithTexture::FillBuffers() {
  program_ = ProgramPool::Get(ProgramPool::ProgramType::DRAW_TEXTURED_RECT);

  cv::Mat flipped;
  cv::flip(data_, flipped, 0);

  auto pixel_type = glow::PixelType::UNSIGNED_BYTE;

  auto cv_depth = flipped.depth();
  if (cv_depth == CV_8U) {
    pixel_type = glow::PixelType::UNSIGNED_BYTE;
  } else if (cv_depth == CV_32F) {
    pixel_type = glow::PixelType::FLOAT;
  } else {
    LOG(FATAL) << "Unexpected type of image.";
  }

  auto texture_format = glow::TextureFormat::RGB;
  auto pixel_format = glow::PixelFormat::BRG;
  int num_channels = flipped.channels();
  if (num_channels == 1) {
    texture_format = glow::TextureFormat::R;
    pixel_format = glow::PixelFormat::R;
  } else if (num_channels == 3) {
    texture_format = glow::TextureFormat::RGB;
    pixel_format = glow::PixelFormat::BRG;
  } else if (num_channels == 4) {
    texture_format = glow::TextureFormat::RGBA;
    pixel_format = glow::PixelFormat::BGRA;
  } else {
    LOG(FATAL) << "Unexpected number of channels.";
  }

  texture_ =
      make_unique<glow::GlTexture>(data_.cols, data_.rows, texture_format);
  texture_->assign(pixel_format, pixel_type, flipped.data);

  const std::vector<vec3> raw = {bottom_left_};
  num_points_ = raw.size();

  vao_ = make_unique<GlVertexArray>();
  vao_->bind();
  SetVertexAttribute(0, raw, vao_.get());
  vao_->release();

  sampler_index_ = 0;
  uniforms_to_override_.emplace_back(
      make_unique<GlUniform<int>>("source", sampler_index_));
  uniforms_to_override_.emplace_back(
      make_unique<GlUniform<vec2>>("rect_size", size_));
  ready_to_draw_ = true;
}

Text::Text(const std::string& text,
           const std::string& font_name,
           const glow::vec3& pos,
           float scale,
           Style draw_style)
    : Drawable{draw_style, GL_TRIANGLES},
      text_{text},
      font_name_{font_name},
      pos_{pos},
      scale_{scale} {}

Text::Text(const std::string& text,
           const std::string& font_name,
           const glow::vec3& pos,
           float scale)
    : Text(text, font_name, pos, scale, Style::DRAW_3D) {}
Text::Text(const std::string& text,
           const std::string& font_name,
           const glow::vec2& pos,
           float scale)
    : Text(text, font_name, {pos.x, pos.y, -1.0f}, scale, Style::DRAW_2D) {}

void Text::FillBuffers() {
  CHECK(FontPool::Instance().HasFont(font_name_));
  auto font = FontPool::Instance().Get(font_name_);
  const cv::Mat& texture = font->texture();

  program_ = ProgramPool::Get(ProgramPool::ProgramType::DRAW_TEXT);

  texture_ = make_unique<glow::GlTexture>(
      texture.cols, texture.rows, glow::TextureFormat::RGBA);
  texture_->assign(PixelFormat::BGRA, PixelType::UNSIGNED_BYTE, texture.data);

  std::vector<vec2> rect_coords;
  rect_coords.reserve(text_.size());
  std::vector<vec2> texture_coords;
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

  num_points_ = rect_coords.size();

  vao_ = make_unique<GlVertexArray>();
  vao_->bind();
  SetVertexAttribute(0, rect_coords, vao_.get());
  SetVertexAttribute(1, texture_coords, vao_.get());
  vao_->release();

  sampler_index_ = 1;
  uniforms_to_override_.emplace_back(
      make_unique<GlUniform<int>>("source", sampler_index_));
  uniforms_to_override_.emplace_back(
      make_unique<GlUniform<vec3>>("anchor", pos_));
  ready_to_draw_ = true;
}

}  // namespace vis
}  // namespace ipb