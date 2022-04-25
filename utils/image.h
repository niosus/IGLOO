#ifndef OPENGL_TUTORIALS_UTILS_IMAGE_H_
#define OPENGL_TUTORIALS_UTILS_IMAGE_H_

#include "absl/types/optional.h"
#include "stb/stb_image.h"
#include "utils/file_utils.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>

namespace utils {

class Image {
 public:
  static std::optional<Image> CreateFrom(const std::filesystem::path& path,
                                         bool flip_vertically = false) {
    if (!utils::FileExists(path)) { return {}; }
    Image image{};
    image.LoadFromPath(path, flip_vertically);
    return image;
  }

  std::int32_t width() const { return width_; }
  std::int32_t height() const { return height_; }
  std::int32_t number_of_channels() const { return number_of_channels_; }
  std::uint8_t* data() const { return data_.get(); }

 private:
  using ImagePtr = std::shared_ptr<std::uint8_t[]>;

  void LoadFromPath(const std::filesystem::path& path, bool flip_vertically) {
    stbi_set_flip_vertically_on_load(flip_vertically);
    data_ = ImagePtr{
        stbi_load(path.c_str(), &width_, &height_, &number_of_channels_, 0),
        stbi_image_free};
  }

  std::int32_t width_{};
  std::int32_t height_{};
  std::int32_t number_of_channels_{};
  ImagePtr data_;
};

}  // namespace utils

#endif  // OPENGL_TUTORIALS_UTILS_IMAGE_H_
