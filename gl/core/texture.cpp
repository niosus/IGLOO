#include "gl/core/texture.h"
#include "utils/image.h"

#include <iostream>

namespace gl {

void Texture::SetWrapping(WrappingDirection wrapping_direction,
                          WrappingMode wrapping_mode,
                          float* border_color) {
  glTexParameteri(static_cast<GLenum>(texture_type_),
                  static_cast<GLenum>(wrapping_direction),
                  static_cast<GLint>(wrapping_mode));
  if (wrapping_mode == Texture::WrappingMode::kClampToBorder &&
      border_color != nullptr) {
    glTexParameterfv(static_cast<GLenum>(texture_type_),
                     GL_TEXTURE_BORDER_COLOR,
                     border_color);
  }
}

void Texture::SetFiltering(FilteringType filtering_type,
                           FilteringMode filtering_mode) {
  glTexParameteri(static_cast<GLenum>(texture_type_),
                  static_cast<GLenum>(filtering_type),
                  static_cast<GLint>(filtering_mode));
}

void Texture::SetImage(const utils::Image& image, int level_of_detail) {
  if (image.data() == nullptr) {
    std::cerr << "No data in the image." << std::endl;
    return;
  }
  GLenum color_mode = 0;
  switch (image.number_of_channels()) {
    case 3: color_mode = GL_RGB; break;
    case 4: color_mode = GL_RGBA; break;
    default: return;
  }
  glTexImage2D(static_cast<GLenum>(texture_type_),
               level_of_detail,
               GL_RGBA,
               image.width(),
               image.height(),
               0,  // Legacy stuff. Was border before.
               color_mode,
               GL_UNSIGNED_BYTE,
               image.data());
  glGenerateMipmap(static_cast<GLenum>(texture_type_));
}

Texture::Builder::Builder(Type type, Identifier identifier)
    : texture_{std::make_unique<Texture>(type, identifier)} {
  texture_->Bind();
}

Texture::Builder& Texture::Builder::WithSaneDefaults() {
  texture_->SetWrapping(gl::Texture::WrappingDirection::kWrapS,
                        gl::Texture::WrappingMode::kRepeat);
  texture_->SetWrapping(gl::Texture::WrappingDirection::kWrapT,
                        gl::Texture::WrappingMode::kRepeat);
  texture_->SetFiltering(gl::Texture::FilteringType::kMinifying,
                         gl::Texture::FilteringMode::kLinear);
  texture_->SetFiltering(gl::Texture::FilteringType::kMagnifying,
                         gl::Texture::FilteringMode::kLinear);
  return *this;
}

Texture::Builder& Texture::Builder::WithWrapping(
    WrappingDirection wrapping_direction,
    WrappingMode wrapping_mode,
    float* border_color) {
  texture_->SetWrapping(wrapping_direction, wrapping_mode);
  return *this;
}
Texture::Builder& Texture::Builder::WithFiltering(
    FilteringType filtering_type, FilteringMode filtering_mode) {
  texture_->SetFiltering(filtering_type, filtering_mode);
  return *this;
}
Texture::Builder& Texture::Builder::WithImage(const utils::Image& image,
                                              int level_of_detail) {
  texture_->SetImage(image, level_of_detail);
  return *this;
}
std::unique_ptr<Texture> Texture::Builder::Build() {
  texture_->UnBind();
  return std::move(texture_);
}

}  // namespace gl
