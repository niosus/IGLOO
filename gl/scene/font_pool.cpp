// Copyright Igor Bogoslavskyi, year 2018.
// In case of any problems with the code please contact me.
// Email: igor.bogoslavskyi@uni-bonn.de.

#include "ipb_opengl_tools/gl/font_pool.h"

#include <ipb_core/utils/memory.h>

#include <glog/logging.h>

namespace ipb {
namespace vis {

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
  auto font = ipb::make_shared<Font>(font_path);
  font_names_.push_back(font->name());
  fonts_.emplace(font->name(), std::move(font));
  return font_names_.back();
}

}  // namespace vis
}  // namespace ipb
