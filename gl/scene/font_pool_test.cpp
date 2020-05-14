// Copyright Igor Bogoslavskyi, year 2020.
// In case of any problems with the code please contact me.
// Email: <name>.<family_name>@gmail.com.

#include "gl/scene/font_pool.h"
#include "gtest/gtest.h"

using namespace gl;

TEST(FontPoolTest, Simple) {
  FontPool::Instance().LoadFont("gl/scene/fonts/ubuntu.fnt");
  ASSERT_TRUE(FontPool::Instance().HasFont("UbuntuNerdFont"));
  ASSERT_EQ(FontPool::Instance().font_names(),
            std::vector<std::string>{"UbuntuNerdFont"});
  const auto font{FontPool::Instance().Get("UbuntuNerdFont")};
  EXPECT_EQ(font->name(), "UbuntuNerdFont");
}
