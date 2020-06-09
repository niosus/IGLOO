#ifndef OPENGL_TUTORIALS_UTILS_EIGEN_TRAITS_H_
#define OPENGL_TUTORIALS_UTILS_EIGEN_TRAITS_H_

#include "gl/core/traits.h"

#include <Eigen/Core>

namespace gl {
namespace traits {

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
struct gl_underlying_type<Eigen::Matrix<ScalarT, ROWS, COLS>>
    : gl_underlying_type<ScalarT> {};

template <typename ScalarT, int ROWS, int COLS>
struct underlying_type<Eigen::Matrix<ScalarT, ROWS, COLS>> {
  using type = ScalarT;
};

template <typename IdentityOpT, typename MatrixT>
struct number_of_entries<Eigen::CwiseNullaryOp<IdentityOpT, MatrixT>>
    : number_of_entries<MatrixT> {};

template <typename IdentityOpT, typename MatrixT>
struct number_of_rows<Eigen::CwiseNullaryOp<IdentityOpT, MatrixT>>
    : number_of_rows<MatrixT> {};

template <typename IdentityOpT, typename MatrixT>
struct number_of_cols<Eigen::CwiseNullaryOp<IdentityOpT, MatrixT>>
    : number_of_cols<MatrixT> {};

template <typename IdentityOpT, typename MatrixT>
struct is_column_major<Eigen::CwiseNullaryOp<IdentityOpT, MatrixT>>
    : is_column_major<MatrixT> {};

template <typename IdentityOpT, typename MatrixT>
struct gl_underlying_type<Eigen::CwiseNullaryOp<IdentityOpT, MatrixT>>
    : gl_underlying_type<MatrixT> {};

template <typename IdentityOpT, typename MatrixT>
struct underlying_type<Eigen::CwiseNullaryOp<IdentityOpT, MatrixT>>
    : underlying_type<MatrixT> {};

}  // namespace traits

}  // namespace gl

#endif  // OPENGL_TUTORIALS_UTILS_EIGEN_TRAITS_H_
