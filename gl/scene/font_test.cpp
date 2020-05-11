#include "gl/scene/font.h"
#include "gtest/gtest.h"

using namespace gl;

TEST(FontTest, Simple) {
  Font font{"gl/scene/fonts/ubuntu.fnt"};
  EXPECT_EQ(font.name(), "UbuntuNerdFont");
  EXPECT_EQ(font.texture().width(), 512);
  EXPECT_EQ(font.texture().height(), 512);
}
