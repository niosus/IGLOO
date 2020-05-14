// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#ifndef CODE_OPENGL_TUTORIALS_GL_SCENE_FONT_POOL_H_
#define CODE_OPENGL_TUTORIALS_GL_SCENE_FONT_POOL_H_

#include "gl/scene/font.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace gl {

/// Container for all available fonts.
class FontPool {
 public:
  static FontPool& Instance();

  /// Get the font with this tag.
  const Font::SharedPtr& Get(const std::string& font_tag) const;
  /// Load a font from a *.fnt file and return its name.
  std::string LoadFont(const std::string& font_path);
  /// Check that the font is present.
  bool HasFont(const std::string& font_tag) const;

  const std::vector<std::string>& font_names() const { return font_names_; }
  const std::map<std::string, Font::SharedPtr>& fonts() const { return fonts_; }

 private:
  FontPool() = default;
  std::vector<std::string> font_names_;
  std::map<std::string, Font::SharedPtr> fonts_;
};

}  // namespace gl

#endif  // CODE_OPENGL_TUTORIALS_GL_SCENE_FONT_POOL_H_
