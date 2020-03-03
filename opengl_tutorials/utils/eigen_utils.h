#ifndef OPENGL_TUTORIALS_UTILS_EIGEN_UTILS_H_
#define OPENGL_TUTORIALS_UTILS_EIGEN_UTILS_H_

#include "Eigen/StdVector"
#include "opengl_tutorials/core/buffer_traits.h"

namespace gl_tutorials {

namespace eigen {
template <typename EigenVectorT>
using vector =
    std::vector<EigenVectorT, Eigen::aligned_allocator<EigenVectorT>>;
}  // namespace eigen

namespace traits {
template <>
struct gl_underlying_type<Eigen::Vector3f> {
  static const int value{GL_FLOAT};
};
template <>
struct components_per_vertex_count<Eigen::Vector3f> {
  static const int value{3};
};

template <>
struct gl_underlying_type<Eigen::Vector3i> {
  static const int value{GL_INT};
};
template <>
struct components_per_vertex_count<Eigen::Vector3i> {
  static const int value{3};
};
}  // namespace traits

}  // namespace gl_tutorials

#endif  // OPENGL_TUTORIALS_UTILS_EIGEN_UTILS_H_
