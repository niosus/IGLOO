#ifndef OPENGL_TUTORIALS_CORE_GL_UNIFORM_H_
#define OPENGL_TUTORIALS_CORE_GL_UNIFORM_H_

#include "gl/core/opengl_object.h"
#include "gl/core/traits.h"
#include "utils/type_traits.h"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

namespace gl {

class Uniform : public OpenGlObject {
 public:
  Uniform(const std::string& name, std::uint32_t program_id)
      : OpenGlObject{0},
        name_{name},
        location_{glGetUniformLocation(program_id, name_.c_str())} {}

  GLint location() const noexcept { return location_; }

  template <typename T, typename A>
  void UpdateValue(const std::vector<T, A>& data) {
    UpdateValue(data.data(), data.size());
  }

  template <typename T>
  void UpdateValue(const T* const data, std::size_t number_of_elements) {
    UpdateArrayLike<T>(static_cast<const void*>(data), number_of_elements);
  }

  template <typename T,
            typename = std::enable_if_t<traits::is_matrix_v<T> ||
                                        traits::is_vector_v<T>>>
  void UpdateValue(const T& matrix_or_vector) {
    UpdateArrayLike<std::remove_reference_t<T>>(
        static_cast<const void*>(&matrix_or_vector), 1ul);
  }

  template <typename T,
            typename... Ts,
            typename = std::enable_if_t<
                ::traits::all_types_are_same_v<T, Ts...> &&
                (::traits::all_types_integral_v<T, Ts...> ||
                 ::traits::all_types_floating_point_v<T, Ts...>)>>
  void UpdateValue(T number, Ts... numbers) {
    UpdateValueFromPack(location_, number, numbers...);
  }

  inline const std::string& name() const { return name_; }

 private:
  template <typename T>
  static constexpr int GetRowsOfType() {
    static_assert(
        ::traits::has_value_member<typename traits::number_of_rows<T>>::value,
        "Missing specialization for trait 'number_of_rows'");
    return traits::number_of_rows<T>::value;
  }

  template <typename T>
  static constexpr int GetColsOfType() {
    static_assert(
        ::traits::has_value_member<typename traits::number_of_cols<T>>::value,
        "Missing specialization for trait 'number_of_cols'");
    return traits::number_of_cols<T>::value;
  }

  template <typename T>
  void UpdateArrayLike(const void* const data, std::size_t number_of_elements) {
    static_assert(
        ::traits::has_type_member<typename traits::underlying_type<T>>::value,
        "Missing specialization for trait 'underlying_type'");
    using UnderlyingType = typename traits::underlying_type<T>::type;
    const auto rows = GetRowsOfType<T>();
    const auto cols = GetColsOfType<T>();
    if constexpr (traits::is_matrix_v<T>) {
      static_assert(::traits::has_value_member<
                        typename traits::is_column_major<T>>::value,
                    "Missing specialization for trait 'is_column_major'");
      const bool transpose = !traits::is_column_major<T>::value;
      UpdateValueFromMatrix<rows, cols, UnderlyingType>(
          location_, data, number_of_elements, transpose);
    } else if (traits::is_vector_v<T>) {
      const int entry_size = rows * cols;
      UpdateValueFromArray<entry_size, UnderlyingType>(
          location_, data, number_of_elements);
    }
  }

  template <typename... Ts>
  void UpdateValueFromPack(std::int32_t location, Ts... n1);

  template <std::size_t kComponentCount, typename T>
  void UpdateValueFromArray(std::int32_t location,
                            const void* const data,
                            std::size_t number_of_vectors);

  template <std::size_t kRows, std::size_t kCols, typename T>
  void UpdateValueFromMatrix(std::int32_t location,
                             const void* const data,
                             std::size_t number_of_vectors,
                             bool transpose);

  std::string name_;
  std::int32_t location_;
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_CORE_GL_UNIFORM_H_
