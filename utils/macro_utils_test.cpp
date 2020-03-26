#include "utils/macro_utils.h"
#include "gtest/gtest.h"

// Some help macro to convert things to string.
#define TRIVIAL_MACRO(input) input
#define QUOTE(...) #__VA_ARGS__
#define QUOTE_ALL(...) QUOTE(__VA_ARGS__)

TEST(MacroUtilsTests, Quoting) {
  EXPECT_EQ("aaa", QUOTE(aaa));
  EXPECT_EQ("aaa", QUOTE_ALL(TRIVIAL_MACRO(aaa)));
}

TEST(MacroUtilsTests, Prefix) {
  EXPECT_EQ("float aaa", QUOTE_ALL(_PREFIX_float(aaa)));
  EXPECT_EQ("int aaa", QUOTE_ALL(_PREFIX_int(aaa)));
  EXPECT_EQ("float aaa, float bbb",
            QUOTE_ALL(_PREFIX_EACH_OF(_PREFIX_float, aaa, bbb)));
  EXPECT_EQ("int aaa, int bbb",
            QUOTE_ALL(_PREFIX_EACH_OF(_PREFIX_int, aaa, bbb)));
}
