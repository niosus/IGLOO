#include "gl/core/uniform.h"
#include "utils/macro_utils.h"

/// Generate a specialization that calls an appropriate version of glUniform
/// based on the number of parameters and their type.
#define GENERATE_FOR_PACKS(type, letter, num_of_params, ...)             \
  template <>                                                            \
  void Uniform::UpdateValueFromPack(std::int32_t location,               \
                                    PREFIX_EACH_OF(type, __VA_ARGS__)) { \
    glUniform##num_of_params##letter(location, __VA_ARGS__);             \
  }

/// Generate a specialization that calls an appropriate vector-based version of
/// glUniform based on the type of the input parameters.
#define GENERATE_FOR_ARRAYS(type, letter, num_of_params)              \
                                                                      \
  template <>                                                         \
  void Uniform::UpdateValueFromArray<num_of_params##ul, type>(        \
      std::int32_t location,                                          \
      const void* const data,                                         \
      std::size_t number_of_vectors) {                                \
    glUniform##num_of_params##letter##v(                              \
        location, number_of_vectors, static_cast<const type*>(data)); \
  }

/// Generate a specialization that calls an appropriate matrix-based version of
/// glUniform based on the type of the input parameters.
#define GENERATE_FOR_MATRICES(type, letter, rows, cols)                        \
                                                                               \
  template <>                                                                  \
  void Uniform::UpdateValueFromMatrix<rows##ul, cols##ul, type>(               \
      std::int32_t location,                                                   \
      const void* const data,                                                  \
      std::size_t number_of_vectors,                                           \
      bool transpose) {                                                        \
    glUniformMatrix##rows##x##cols##letter##v(location,                        \
                                              number_of_vectors,               \
                                              transpose,                       \
                                              static_cast<const type*>(data)); \
  }

/// Generate a specialization that calls an appropriate matrix-based version of
/// glUniform based on the type of the input parameters.
#define GENERATE_FOR_SQUARE_MATRICES(type, letter, size)              \
                                                                      \
  template <>                                                         \
  void Uniform::UpdateValueFromMatrix<size##ul, size##ul, type>(      \
      std::int32_t location,                                          \
      const void* const data,                                         \
      std::size_t number_of_vectors,                                  \
      bool transpose) {                                               \
    glUniformMatrix##size##letter##v(location,                        \
                                     number_of_vectors,               \
                                     transpose,                       \
                                     static_cast<const type*>(data)); \
  }

#define GENERATE_PACK_SPECIALIZATIONS(type, letter) \
  GENERATE_FOR_PACKS(type, letter, 1, n1);          \
  GENERATE_FOR_PACKS(type, letter, 2, n1, n2);      \
  GENERATE_FOR_PACKS(type, letter, 3, n1, n2, n3);  \
  GENERATE_FOR_PACKS(type, letter, 4, n1, n2, n3, n4);

#define GENERATE_ARRAY_SPECIALIZATIONS(type, letter) \
  GENERATE_FOR_ARRAYS(type, letter, 1);              \
  GENERATE_FOR_ARRAYS(type, letter, 2);              \
  GENERATE_FOR_ARRAYS(type, letter, 3);              \
  GENERATE_FOR_ARRAYS(type, letter, 4);

#define GENERATE_MATRIX_SPECIALIZATIONS(type, letter) \
  GENERATE_FOR_SQUARE_MATRICES(type, letter, 2);      \
  GENERATE_FOR_SQUARE_MATRICES(type, letter, 3);      \
  GENERATE_FOR_SQUARE_MATRICES(type, letter, 4);      \
                                                      \
  GENERATE_FOR_MATRICES(type, letter, 2, 3);          \
  GENERATE_FOR_MATRICES(type, letter, 3, 2);          \
  GENERATE_FOR_MATRICES(type, letter, 2, 4);          \
  GENERATE_FOR_MATRICES(type, letter, 4, 2);          \
  GENERATE_FOR_MATRICES(type, letter, 3, 4);          \
  GENERATE_FOR_MATRICES(type, letter, 4, 3);

namespace gl {

GENERATE_PACK_SPECIALIZATIONS(float, f);
GENERATE_ARRAY_SPECIALIZATIONS(float, f);
GENERATE_MATRIX_SPECIALIZATIONS(float, f);

GENERATE_PACK_SPECIALIZATIONS(std::int32_t, i);
GENERATE_ARRAY_SPECIALIZATIONS(std::int32_t, i);

GENERATE_PACK_SPECIALIZATIONS(std::uint32_t, ui);
GENERATE_ARRAY_SPECIALIZATIONS(std::uint32_t, ui);
}  // namespace gl
