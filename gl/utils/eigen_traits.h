#ifndef OPENGL_TUTORIALS_UTILS_EIGEN_TRAITS_H_
#define OPENGL_TUTORIALS_UTILS_EIGEN_TRAITS_H_

#include "gl/core/traits.h"

#include <Eigen/Core>

namespace gl {
namespace traits {

template <typename ScalarT, int ROWS>
struct number_of_entries<Eigen::Matrix<ScalarT, ROWS, 1>> {
  static const int value{ROWS * number_of_entries<ScalarT>::value};
};

template <typename ScalarT, int ROWS, int COLS>
struct number_of_entries<Eigen::Matrix<ScalarT, ROWS, COLS>> {
  static const int value{ROWS * COLS * number_of_entries<ScalarT>::value};
};

template <typename ScalarT, int ROWS, int COLS>
struct number_of_rows<Eigen::Matrix<ScalarT, ROWS, COLS>> {
  static const int value{ROWS};
};

template <typename ScalarT, int ROWS, int COLS>
struct number_of_cols<Eigen::Matrix<ScalarT, ROWS, COLS>> {
  static const int value{COLS};
};

template <typename ScalarT, int ROWS, int COLS>
struct is_column_major<Eigen::Matrix<ScalarT, ROWS, COLS>> : std::true_type {};

template <typename ScalarT, int ROWS, int COLS>
struct gl_underlying_type<Eigen::Matrix<ScalarT, ROWS, COLS>> {
  static const int value{gl_underlying_type<ScalarT>::value};
};

template <typename ScalarT, int ROWS, int COLS>
struct underlying_type<Eigen::Matrix<ScalarT, ROWS, COLS>> {
  using type = ScalarT;
};

}  // namespace traits

}  // namespace gl

#endif  // OPENGL_TUTORIALS_UTILS_EIGEN_TRAITS_H_
