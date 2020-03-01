#ifndef OPENGL_TUTORIALS_UTILS_EIGEN_UTILS_H_
#define OPENGL_TUTORIALS_UTILS_EIGEN_UTILS_H_

#include "Eigen/StdVector"

namespace gl_tutorials {
namespace eigen {

template <typename EigenVectorT>
using vector =
    std::vector<EigenVectorT, Eigen::aligned_allocator<EigenVectorT>>;

}  // namespace eigen
}  // namespace gl_tutorials

#endif  // OPENGL_TUTORIALS_UTILS_EIGEN_UTILS_H_

