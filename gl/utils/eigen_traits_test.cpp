#include "gl/utils/eigen_traits.h"
#include "utils/macro_utils.h"
#include "gtest/gtest.h"

using namespace gl::traits;

TEST(EigenTraitsTest, CheckVector) {
  const auto is_matrix = gl::traits::is_matrix_v<Eigen::Vector3f>;
  const auto is_column_major =
      gl::traits::is_column_major<Eigen::Vector3f>::value;
  const auto size = gl::traits::number_of_entries<Eigen::Vector3f>::value;
  const auto rows = gl::traits::number_of_rows<Eigen::Vector3f>::value;
  const auto cols = gl::traits::number_of_cols<Eigen::Vector3f>::value;
  EXPECT_FALSE(is_matrix);
  EXPECT_TRUE(is_column_major);
  EXPECT_EQ(3, size);
  EXPECT_EQ(3, rows);
  EXPECT_EQ(1, cols);
}

TEST(EigenTraitsTest, CheckMatrix) {
  const int kRows = 4;
  const int kCols = 2;
  using ScalarT = float;
  using MatrixType = Eigen::Matrix<ScalarT, kRows, kCols>;
  const auto is_matrix = gl::traits::is_matrix_v<MatrixType>;
  const auto is_column_major = gl::traits::is_column_major<MatrixType>::value;
  const auto size = gl::traits::number_of_entries<MatrixType>::value;
  const auto rows = gl::traits::number_of_rows<MatrixType>::value;
  const auto cols = gl::traits::number_of_cols<MatrixType>::value;
  EXPECT_TRUE(is_matrix);
  EXPECT_TRUE(is_column_major);
  EXPECT_EQ(kRows * kCols, size);
  EXPECT_EQ(kRows, rows);
  EXPECT_EQ(kCols, cols);
  using GotType = gl::traits::underlying_type<MatrixType>::type;
  bool same_type = std::is_same<ScalarT, GotType>::value;
  EXPECT_TRUE(same_type);
}
