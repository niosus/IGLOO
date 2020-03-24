#ifndef OPENGL_TUTORIALS_UTILS_TYPE_UTILS_H_
#define OPENGL_TUTORIALS_UTILS_TYPE_UTILS_H_

#include <type_traits>

namespace gl_tutorials {
namespace type_utils {

template <typename T, typename = void>
struct has_value_member : std::false_type {};

template <typename T>
struct has_value_member<T, decltype((void)T::value, void())> : std::true_type {
};

template <class...>
using void_t = void;

template <class T, class U = void>
struct has_type_member : public std::false_type {};

template <class T>
struct has_type_member<T, void_t<typename T::type>> : public std::true_type {};

}  // namespace type_utils
}  // namespace gl_tutorials

#endif  // OPENGL_TUTORIALS_UTILS_TYPE_UTILS_H_
