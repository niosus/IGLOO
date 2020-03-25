#ifndef OPENGL_TUTORIALS_UTILS_EIGEN_TRAITS_H_
#define OPENGL_TUTORIALS_UTILS_EIGEN_TRAITS_H_

#include "gl/core/buffer_traits.h"
#include "utils/eigen_utils.h"

namespace gl {
namespace traits {

template <typename T, int N>
struct components_per_vertex_count<eigen::Vector<T, N>> {
  static const int value{N * components_per_vertex_count<T>::value};
};

template <typename T, int N>
struct gl_underlying_type<eigen::Vector<T, N>> {
  static const int value{gl_underlying_type<T>::value};
};

template <typename T, int N>
struct underlying_type<eigen::Vector<T, N>> {
  using type = T;
};

}  // namespace traits

}  // namespace gl

#endif  // OPENGL_TUTORIALS_UTILS_EIGEN_TRAITS_H_
