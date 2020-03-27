#ifndef OPENGL_TUTORIALS_UTILS_TYPE_UTILS_H_
#define OPENGL_TUTORIALS_UTILS_TYPE_UTILS_H_

#include <type_traits>

namespace traits {

template <typename T, typename = std::void_t<>>
struct has_value_member : std::false_type {};

template <typename T>
struct has_value_member<T, decltype((void)T::value, void())> : std::true_type {
};

template <class T, class U = std::void_t<>>
struct has_type_member : public std::false_type {};

template <class T>
struct has_type_member<T, std::void_t<typename T::type>> : std::true_type {};

template <class T, typename... Ts>
inline constexpr bool all_types_are_same_v =
    std::conjunction_v<std::is_same<T, Ts>...>;

template <class T, typename... Ts>
inline constexpr bool all_types_integral_v =
    std::conjunction_v<std::is_integral<T>, std::is_integral<Ts>...>;

template <class T, typename... Ts>
inline constexpr bool all_types_floating_point_v =
    std::conjunction_v<std::is_floating_point<T>,
                       std::is_floating_point<Ts>...>;

}  // namespace traits

#endif  // OPENGL_TUTORIALS_UTILS_TYPE_UTILS_H_
