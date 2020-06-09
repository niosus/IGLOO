// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "gl/scene/font.h"
#include "gtest/gtest.h"

using namespace gl;

TEST(FontTest, Simple) {
  Font font{"gl/scene/fonts/ubuntu.fnt"};
  EXPECT_EQ(font.name(), "UbuntuNerdFont");
  EXPECT_EQ(font.texture().width(), 512);
  EXPECT_EQ(font.texture().height(), 512);
  EXPECT_GT(font.GetCharCoords('a').width, 0);
}
