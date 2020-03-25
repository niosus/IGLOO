#include "utils/image.h"
#include "gtest/gtest.h"

using utils::Image;

TEST(ImageTest, InitEmpty) {
  Image image{};
  EXPECT_EQ(0, image.width());
  EXPECT_EQ(0, image.height());
  EXPECT_EQ(0, image.number_of_channels());
  EXPECT_EQ(nullptr, image.data());
}

TEST(ImageTest, Read) {
  const auto image = Image::CreateFrom("utils/test_images/container.jpg");
  ASSERT_EQ(true, image.has_value());
  EXPECT_EQ(512, image->width());
  EXPECT_EQ(512, image->height());
  EXPECT_EQ(3, image->number_of_channels());
  EXPECT_NE(nullptr, image->data());
}

TEST(ImageTest, ReadWrong) {
  const auto image = Image::CreateFrom("non_existing_path");
  ASSERT_EQ(false, image.has_value());
}
