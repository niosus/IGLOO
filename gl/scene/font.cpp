// Copyright Igor Bogoslavskyi, year 2018.
// In case of any problems with the code please contact me.
// Email: igor.bogoslavskyi@uni-bonn.de.

#include "gl/scene/font.h"

#include <fstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>

namespace ipb {
namespace vis {

Font::Font(const std::string& path_to_font) {
  CHECK(boost::ends_with(path_to_font, ".fnt")) << "We only read *.fnt files";
  auto full_path = boost::filesystem::path(kIpbOpenGlPath) / path_to_font;
  CHECK(boost::filesystem::exists(full_path)) << full_path;
  LOG(INFO) << "Reading font from " << full_path;
  std::ifstream file(full_path.string());

  auto SplitNextLine = [](std::ifstream* file) {
    std::string line;
    std::vector<std::string> split_string;
    if (!std::getline(*file, line, '\n')) { return split_string; }
    boost::split(split_string, line, boost::is_any_of("\t "));
    return split_string;
  };

  auto GetTextureImageName = [&SplitNextLine](std::ifstream* file) {
    std::vector<std::string> split_string = SplitNextLine(file);
    CHECK_EQ(split_string.size(), static_cast<size_t>(2))
        << "Wrong line format.";
    CHECK(boost::ends_with(split_string[1], ".png")) << "Texture must be png.";
    return split_string[1];
  };

  auto GetFontName = [&SplitNextLine](std::ifstream* file) {
    std::vector<std::string> split_string = SplitNextLine(file);
    return std::accumulate(
        split_string.begin(), split_string.end() - 1, std::string(""));
  };

  // Load image.
  auto image_name = GetTextureImageName(&file);
  full_path.replace_extension(".png");
  CHECK(boost::filesystem::exists(full_path)) << full_path;
  font_texture_ = cv::imread(full_path.string(), cv::IMREAD_UNCHANGED);
  cv::flip(font_texture_, font_texture_, 0);  // Flip image vertically.

  // Get font name for storage purposes.
  name_ = GetFontName(&file);

  // Real all the chars and store their Texture coordinates. These are different
  // from image coordinates and are intended to be used with OpenGL.
  const float kYNormalizer = 1.0f / font_texture_.rows;
  const float kXNormalizer = 1.0f / font_texture_.cols;
  while (true) {
    auto split = SplitNextLine(&file);
    if (split.size() != 9) {
      LOG(INFO) << "Read all relevant chars.";
      break;
    }
    CHECK_GE(split.size(), static_cast<size_t>(5)) << "Wrong format. ";
    TextureCoords coords{kXNormalizer * std::stoi(split[1]),
                         1.0f - kYNormalizer * std::stoi(split[2]),
                         kXNormalizer * std::stoi(split[3]),
                         -kYNormalizer * std::stoi(split[4])};
    char_coords_.emplace(std::stoi(split[0]), coords);
  }
}

}  // namespace vis
}  // namespace ipb
