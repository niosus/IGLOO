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
      : OpenGlObject{0}, name_{name} {
    id_ = glGetUniformLocation(program_id, name_.c_str());
  }

  template <typename T, typename A>
  void UpdateValue(const std::vector<T, A>& data) {
    UpdateValue(data.data(), data.size());
  }

  template <typename T>
  void UpdateValue(const T* const data, std::size_t number_of_elements) {
    static_assert(
        ::traits::has_type_member<typename traits::underlying_type<T>>::value,
        "Missing specialization for trait 'underlying_type'");
    static_assert(
        ::traits::has_value_member<typename traits::number_of_rows<T>>::value,
        "Missing specialization for trait 'number_of_rows'");
    static_assert(
        ::traits::has_value_member<typename traits::number_of_cols<T>>::value,
        "Missing specialization for trait 'number_of_cols'");

    using UnderlyingType = typename traits::underlying_type<T>::type;
    const int rows = traits::number_of_rows<T>::value;
    const int cols = traits::number_of_cols<T>::value;
    if constexpr (traits::is_matrix<T>::value && rows > 1 && cols > 1) {
      // Now we are sure that T is actually a matrix type, not a vector type.
      static_assert(::traits::has_value_member<
                        typename traits::is_column_major<T>>::value,
                    "Missing specialization for trait 'is_column_major'");
      const bool transpose = !traits::is_column_major<T>::value;
      UpdateValueFromMatrix<rows, cols, UnderlyingType>(
          id_, static_cast<const void*>(data), number_of_elements, transpose);
    } else {
      // Now we are sure that T is a vector type, but not a matrix.
      const int entry_size = rows * cols;
      UpdateValueFromArray<entry_size, UnderlyingType>(
          id_, static_cast<const void*>(data), number_of_elements);
    }
  }

  template <typename T, typename... Ts>
  void UpdateValue(T number, Ts... numbers) {
    static_assert(::traits::all_types_are_same_v<T, Ts...>,
                  "All types in the pack must be the same.");
    static_assert(::traits::all_types_integral_v<T, Ts...> ||
                      ::traits::all_types_floating_point_v<T, Ts...>,
                  "Only use pack initialization for simple types like float or "
                  "int. Use vector initialization for more complex types.");
    UpdateValueFromPack(id_, number, numbers...);
  }

  inline const std::string& name() const { return name_; }

 private:
  template <typename... Ts>
  void UpdateValueFromPack(std::uint32_t id, Ts... n1);

  template <std::size_t kComponentCount, typename T>
  void UpdateValueFromArray(std::uint32_t id,
                            const void* const data,
                            std::size_t number_of_vectors);

  template <std::size_t kRows, std::size_t kCols, typename T>
  void UpdateValueFromMatrix(std::uint32_t id,
                             const void* const data,
                             std::size_t number_of_vectors,
                             bool transpose);

  std::string name_;
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_CORE_GL_UNIFORM_H_
