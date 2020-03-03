#ifndef OPENGL_TUTORIALS_UTILS_TYPE_UTILS_H_
#define OPENGL_TUTORIALS_UTILS_TYPE_UTILS_H_

#include <type_traits>

namespace gl_tutorials {
namespace type_utils {
template <typename T, typename = void>
struct has_value : std::false_type {};

template <typename T>
struct has_value<T, decltype((void)T::value, void())> : std::true_type {};
}  // namespace type_utils
}  // namespace gl_tutorials

#endif  // OPENGL_TUTORIALS_UTILS_TYPE_UTILS_H_
