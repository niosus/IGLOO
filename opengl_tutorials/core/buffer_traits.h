#ifndef OPENGL_TUTORIALS_CORE_BUFFER_TRAITS_H_
#define OPENGL_TUTORIALS_CORE_BUFFER_TRAITS_H_

#include "opengl_tutorials/core/gl_base.h"

#include <type_traits>
#include <vector>

namespace gl_tutorials {
namespace traits {

template <typename T, typename Enable = void>
struct components_per_vertex_count {};

template <typename T>
struct components_per_vertex_count<
    T,
    typename std::enable_if<std::is_floating_point<T>::value ||
                            std::is_integral<T>::value>::type> {
  static const int value{1};
};

template <typename T>
struct gl_underlying_type {};

template <>
struct gl_underlying_type<std::int8_t> {
  static const int value{GL_BYTE};
};

template <>
struct gl_underlying_type<std::uint8_t> {
  static const int value{GL_UNSIGNED_BYTE};
};

template <>
struct gl_underlying_type<std::int16_t> {
  static const int value{GL_SHORT};
};

template <>
struct gl_underlying_type<std::uint16_t> {
  static const int value{GL_UNSIGNED_SHORT};
};

template <>
struct gl_underlying_type<std::int32_t> {
  static const int value{GL_INT};
};

template <>
struct gl_underlying_type<std::uint32_t> {
  static const int value{GL_UNSIGNED_INT};
};

template <>
struct gl_underlying_type<float> {
  static const int value{GL_FLOAT};
};

template <>
struct gl_underlying_type<double> {
  static const int value{GL_DOUBLE};
};

}  // namespace traits
}  // namespace gl_tutorials

#endif  // OPENGL_TUTORIALS_CORE_BUFFER_TRAITS_H_
