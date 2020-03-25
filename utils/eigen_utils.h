#ifndef OPENGL_TUTORIALS_UTILS_EIGEN_UTILS_H_
#define OPENGL_TUTORIALS_UTILS_EIGEN_UTILS_H_

#include "Eigen/StdVector"

namespace eigen {
template <typename EigenVectorT>
using vector =
    std::vector<EigenVectorT, Eigen::aligned_allocator<EigenVectorT>>;

template <typename ScalarT, int N>
using Vector = Eigen::Matrix<ScalarT, N, 1>;

}  // namespace eigen

#endif  // OPENGL_TUTORIALS_UTILS_EIGEN_UTILS_H_
