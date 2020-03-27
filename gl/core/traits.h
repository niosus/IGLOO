#ifndef OPENGL_TUTORIALS_CORE_BUFFER_TRAITS_H_
#define OPENGL_TUTORIALS_CORE_BUFFER_TRAITS_H_

#include "gl/core/opengl_object.h"

#include <type_traits>
#include <vector>

namespace gl {
namespace traits {

template <typename T, typename Enable = void>
struct number_of_entries {};

template <typename T>
struct number_of_entries<
    T,
    typename std::enable_if<std::is_floating_point<T>::value ||
                            std::is_integral<T>::value>::type> {
  static const int value{1};
};

template <typename T, typename Enable = void>
struct number_of_rows {};

template <typename T>
struct number_of_rows<
    T,
    typename std::enable_if<std::is_floating_point<T>::value ||
                            std::is_integral<T>::value>::type> {
  static const int value{1};
};

template <typename T, typename Enable = void>
struct number_of_cols {};

template <typename T>
struct number_of_cols<
    T,
    typename std::enable_if<std::is_floating_point<T>::value ||
                            std::is_integral<T>::value>::type> {
  static const int value{1};
};

template <typename T, typename Enable = void>
struct is_matrix : std::false_type {};

template <typename T, typename Enable = void>
struct is_column_major : std::false_type {};

template <typename T, typename Enable = void>
struct underlying_type {};

template <typename T>
struct underlying_type<
    T,
    typename std::enable_if<std::is_floating_point<T>::value ||
                            std::is_integral<T>::value>::type> {
  using type = T;
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
}  // namespace gl

#endif  // OPENGL_TUTORIALS_CORE_BUFFER_TRAITS_H_
