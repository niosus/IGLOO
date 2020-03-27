#include "utils/macro_utils.h"
#include "gtest/gtest.h"

#define QUOTE(...) #__VA_ARGS__
#define QUOTE_ALL(...) QUOTE(__VA_ARGS__)

TEST(MacroUtilsTests, Quoting) {
  EXPECT_EQ("aaa", QUOTE(aaa));
  EXPECT_EQ("aaa bbb", QUOTE_ALL(aaa bbb));
}

TEST(MacroUtilsTests, Prefix) {
  EXPECT_EQ("float aaa", QUOTE_ALL(_PREFIX_float(aaa)));
  EXPECT_EQ("std::int32_t aaa", QUOTE_ALL(_PREFIX_int32_t(aaa)));
  EXPECT_EQ("float aaa, float bbb",
            QUOTE_ALL(_PREFIX_EACH_OF(_PREFIX_float, aaa, bbb)));
  EXPECT_EQ("std::int32_t aaa, std::int32_t bbb",
            QUOTE_ALL(_PREFIX_EACH_OF(_PREFIX_int32_t, aaa, bbb)));
}
