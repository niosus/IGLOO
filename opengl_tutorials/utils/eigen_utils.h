#ifndef OPENGL_TUTORIALS_UTILS_EIGEN_UTILS_H_
#define OPENGL_TUTORIALS_UTILS_EIGEN_UTILS_H_

#include "Eigen/StdVector"
#include "opengl_tutorials/core/buffer_traits.h"

namespace gl_tutorials {

namespace eigen {
template <typename EigenVectorT>
using vector =
    std::vector<EigenVectorT, Eigen::aligned_allocator<EigenVectorT>>;

template <typename ScalarT, int N>
using Vector = Eigen::Matrix<ScalarT, N, 1>;

}  // namespace eigen

namespace traits {

template <typename T, int N>
struct components_per_vertex_count<eigen::Vector<T, N>> {
  static const int value{N};
};

template <typename T, int N>
struct gl_underlying_type<eigen::Vector<T, N>> {
  static const int value{gl_underlying_type<T>::value};
};

}  // namespace traits

}  // namespace gl_tutorials

#endif  // OPENGL_TUTORIALS_UTILS_EIGEN_UTILS_H_
