// Copyright Igor Bogoslavskyi, year 2018.
// In case of any problems with the code please contact me.
// Email: igor.bogoslavskyi@uni-bonn.de.

#include "gl/scene/font.h"
#include "absl/strings/str_split.h"

#include <fstream>
#include <vector>

namespace {
const char kFontExtension[]{".fnt"};
const char kImageExtension[]{".png"};
const char kPathDelimeter{'/'};
}  // namespace

namespace gl {

Font::Font(const std::string& file_name) {
  CHECK(absl::EndsWith(file_name, kFontExtension))
      << "We only read " << kFontExtension << " files";

  auto ConcatenateWithDelimeter = [](const std::string& a,
                                     const std::string& b) {
    if (a.empty()) { return b; }
    return a + kPathDelimeter + b;
  };
  const std::vector<std::string> split_path{
      absl::StrSplit(file_name, kPathDelimeter)};
  const auto folder{std::accumulate(split_path.begin(),
                                    split_path.end() - 1,
                                    std::string{},
                                    ConcatenateWithDelimeter)};

  const auto font_content{utils::ReadFileContents(file_name)};

  LOG(INFO) << "Reading font from " << file_name;
  std::ifstream file(file_name);

  auto SplitNextLine = [](std::ifstream* file) -> std::vector<std::string> {
    std::string line;
    if (!std::getline(*file, line, '\n')) { return {}; }
    return absl::StrSplit(line, absl::ByAnyChar("\t "));
  };

  auto GetTextureImageName = [&SplitNextLine](std::ifstream* file) {
    std::vector<std::string> split_string{SplitNextLine(file)};
    CHECK_EQ(split_string.size(), static_cast<size_t>(2))
        << "Wrong line format.";
    CHECK(absl::EndsWith(split_string[1], kImageExtension))
        << "Texture must be " << kImageExtension;
    return split_string[1];
  };

  auto GetFontName = [&SplitNextLine](std::ifstream* file) {
    std::vector<std::string> split_string{SplitNextLine(file)};
    return std::accumulate(
        split_string.begin(), split_string.end() - 1, std::string(""));
  };

  // Load image.
  const auto image_name{GetTextureImageName(&file)};
  const auto path_to_image{folder + kPathDelimeter + image_name};
  const auto texture{utils::Image::CreateFrom(path_to_image)};
  CHECK(texture.has_value()) << "Could not load image: " << path_to_image;
  font_texture_ = texture.value();
  // Get font name for storage purposes.
  name_ = GetFontName(&file);

  // Read all the chars and store their Texture coordinates. These are different
  // from image coordinates and are intended to be used with OpenGL.
  const float kYNormalizer{1.0f / font_texture_.height()};
  const float kXNormalizer{1.0f / font_texture_.width()};
  while (true) {
    const auto split{SplitNextLine(&file)};
    if (split.size() != 9ul) {
      LOG(INFO) << "Read all relevant chars.";
      break;
    }
    CHECK_GE(split.size(), 5ul) << "Wrong format. ";
    TextureCoords coords{kXNormalizer * std::stoi(split[1]),
                         1.0f - kYNormalizer * std::stoi(split[2]),
                         kXNormalizer * std::stoi(split[3]),
                         -kYNormalizer * std::stoi(split[4])};
    char_coords_.emplace(std::stoi(split[0]), coords);
  }
}

}  // namespace gl
