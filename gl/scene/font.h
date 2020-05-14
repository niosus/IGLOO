// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#ifndef CODE_OPENGL_TUTORIALS_GL_SCENE_FONT_H_
#define CODE_OPENGL_TUTORIALS_GL_SCENE_FONT_H_

#include "glog/logging.h"

#include "utils/image.h"

#include <map>
#include <memory>
#include <numeric>
#include <string>

namespace gl {

/// A class to work with font generated by https://github.com/scriptum/UBFG
/// The idea here is that we cut out the images of letters given a texture and
/// where these letters are in the texture.
class Font {
  struct TextureCoords;

 public:
  using UniquePtr = std::unique_ptr<Font>;
  using SharedPtr = std::shared_ptr<Font>;

  explicit Font(const std::string& file_name);

  /// Get the coordinates of the characted in the texture.
  inline const TextureCoords& GetCharCoords(char symbol) const {
    CHECK(char_coords_.count(symbol))
        << "Symbol '" << symbol << "' is not in font '" << name_ << "'.";
    return char_coords_.at(symbol);
  }

  /// Name of this font.
  const std::string& name() const { return name_; }

  /// Get the texture.
  const utils::Image& texture() const { return font_texture_; }

 private:
  struct TextureCoords {
    float x;
    float y;
    float width;
    float height;
  };

  std::string name_;

  utils::Image font_texture_;
  std::map<char, TextureCoords> char_coords_;
};

}  // namespace gl

#endif  // CODE_OPENGL_TUTORIALS_GL_SCENE_FONT_H_