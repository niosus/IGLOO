#include "utils/macro_utils.h"
#include "gtest/gtest.h"

#define QUOTE(...) #__VA_ARGS__
#define QUOTE_ALL(...) QUOTE(__VA_ARGS__)

TEST(MacroUtilsTests, Quoting) {
  EXPECT_EQ("aaa", QUOTE(aaa));
  EXPECT_EQ("aaa bbb", QUOTE_ALL(aaa bbb));
}

TEST(MacroUtilsTests, Prefix) {
  EXPECT_EQ("float aaa", QUOTE_ALL(PREFIX(float, aaa)));
  EXPECT_EQ("aaa&&", QUOTE_ALL(UNIVERSAL_REF(aaa)));
  EXPECT_EQ("std::int32_t aaa", QUOTE_ALL(PREFIX(std::int32_t, aaa)));
  EXPECT_EQ("float aaa, float bbb", QUOTE_ALL(PREFIX_EACH_OF(float, aaa, bbb)));
  EXPECT_EQ("std::int32_t aaa, std::int32_t bbb",
            QUOTE_ALL(PREFIX_EACH_OF(std::int32_t, aaa, bbb)));
  EXPECT_EQ("std::int32_t&& aaa, std::int32_t&& bbb",
            QUOTE_ALL(PREFIX_EACH_OF(UNIVERSAL_REF(std::int32_t), aaa, bbb)));
}
