#ifndef CODE_OPENGL_TUTORIALS_GL_CORE_TEXTURE_H_
#define CODE_OPENGL_TUTORIALS_GL_CORE_TEXTURE_H_

#include "gl/core/opengl_object.h"
#include "utils/image.h"

#include <iostream>

namespace gl {

class Texture : public OpenGlObject {
 public:
  enum class Type : GLenum;
  enum class Identifier : GLenum;
  enum class WrappingDirection : GLenum;
  enum class FilteringType : GLenum;
  enum class FilteringMode : GLint;
  enum class WrappingMode : GLint;

  class Builder {
   public:
    Builder(Type type, Identifier identifier);
    Builder& WithSaneDefaults();
    Builder& WithWrapping(WrappingDirection wrapping_direction,
                          WrappingMode wrapping_mode,
                          float* border_color = nullptr);
    Builder& WithFiltering(FilteringType filtering_type,
                           FilteringMode filtering_mode);
    Builder& WithImage(const utils::Image& image,
                       int level_of_detail = 0,
                       int border = 0);
    std::unique_ptr<Texture> Build();

   private:
    std::unique_ptr<Texture> texture_{};
  };

  Texture(Type type, Identifier identifier)
      : OpenGlObject{}, texture_type_{type}, texture_identifier_{identifier} {
    glGenTextures(1, &id_);
  }

  inline void Bind() {
    glActiveTexture(static_cast<GLenum>(texture_identifier_));
    glBindTexture(static_cast<GLenum>(texture_type_), id_);
  }

  inline void UnBind() { glBindTexture(static_cast<GLenum>(texture_type_), 0); }

  void SetWrapping(WrappingDirection wrapping_direction,
                   WrappingMode wrapping_mode,
                   float* border_color = nullptr);

  void SetFiltering(FilteringType filtering_type, FilteringMode filtering_mode);

  void SetImage(const utils::Image& image,
                int level_of_detail = 0,
                int border = 0);

 private:
  Type texture_type_{};
  Identifier texture_identifier_{};
};

enum class Texture::FilteringType : GLenum {
  kMinifying = GL_TEXTURE_MIN_FILTER,
  kMagnifying = GL_TEXTURE_MAG_FILTER,
};

enum class Texture::FilteringMode : GLint {
  kNearest = GL_NEAREST,
  kLinear = GL_LINEAR,
};

enum class Texture::WrappingDirection : GLenum {
  kWrapS = GL_TEXTURE_WRAP_S,
  kWrapT = GL_TEXTURE_WRAP_T,
  kWrapR = GL_TEXTURE_WRAP_R,
};

enum class Texture::Type : GLenum {
  kTexture1D = GL_TEXTURE_1D,
  kTexture2D = GL_TEXTURE_2D,
  kTexture3D = GL_TEXTURE_3D
};

enum class Texture::WrappingMode : GLint {
  kClampToBorder = GL_CLAMP_TO_BORDER,
  kClampToEdge = GL_CLAMP_TO_EDGE,
  kMirroredRepeat = GL_MIRRORED_REPEAT,
  kRepeat = GL_REPEAT,
};

enum class Texture::Identifier : GLenum {
  kTexture0 = GL_TEXTURE0,
  kTexture1 = GL_TEXTURE1,
  kTexture2 = GL_TEXTURE2,
  kTexture3 = GL_TEXTURE3,
  kTexture4 = GL_TEXTURE4,
  kTexture5 = GL_TEXTURE5,
  kTexture6 = GL_TEXTURE6,
  kTexture7 = GL_TEXTURE7,
  kTexture8 = GL_TEXTURE8,
  kTexture9 = GL_TEXTURE9,
  kTexture10 = GL_TEXTURE10,
  kTexture11 = GL_TEXTURE11,
  kTexture12 = GL_TEXTURE12,
  kTexture13 = GL_TEXTURE13,
  kTexture14 = GL_TEXTURE14,
  kTexture15 = GL_TEXTURE15,
  kTexture16 = GL_TEXTURE16,
  kTexture17 = GL_TEXTURE17,
  kTexture18 = GL_TEXTURE18,
  kTexture19 = GL_TEXTURE19,
  kTexture20 = GL_TEXTURE20,
  kTexture21 = GL_TEXTURE21,
  kTexture22 = GL_TEXTURE22,
  kTexture23 = GL_TEXTURE23,
  kTexture24 = GL_TEXTURE24,
  kTexture25 = GL_TEXTURE25,
  kTexture26 = GL_TEXTURE26,
  kTexture27 = GL_TEXTURE27,
  kTexture28 = GL_TEXTURE28,
  kTexture29 = GL_TEXTURE29,
  kTexture30 = GL_TEXTURE30,
  kTexture31 = GL_TEXTURE31,
};

}  // namespace gl

#endif  // CODE_OPENGL_TUTORIALS_GL_CORE_TEXTURE_H_
