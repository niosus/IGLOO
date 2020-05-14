// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "gl/scene/font_pool.h"

#include <glog/logging.h>

#include <memory>

namespace gl {

FontPool& FontPool::Instance() {
  static FontPool pool;
  return pool;
}

bool FontPool::HasFont(const std::string& font_tag) const {
  return fonts_.count(font_tag) > 0;
}

const Font::SharedPtr& FontPool::Get(const std::string& font_tag) const {
  CHECK(fonts_.count(font_tag)) << "Must load the font before using it.";
  return fonts_.at(font_tag);
}

std::string FontPool::LoadFont(const std::string& font_path) {
  auto font = std::make_shared<Font>(font_path);
  font_names_.push_back(font->name());
  fonts_.emplace(font->name(), std::move(font));
  return font_names_.back();
}

}  // namespace gl
