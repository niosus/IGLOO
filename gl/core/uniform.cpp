#include "gl/core/uniform.h"
#include "utils/macro_utils.h"

/// Generate a specialization that calls an appropriate version of glUniform
/// based on the number of parameters and their type.
#define GENERATE_PACK_VALUE_UPDATER(                                           \
    type_name, type_letter, num_of_params, ...)                                \
  template <>                                                                  \
  void Uniform::UpdateValueFromPack(std::uint32_t id,                          \
                                    _PREFIX_EACH_OF(type_name, __VA_ARGS__)) { \
    glUniform##num_of_params##type_letter(id, __VA_ARGS__);                    \
  }

/// Generate a specialization that calls an appropriate vector-based version of
/// glUniform based on the type of the input parameters.
#define GENERATE_ARRAY_VALUE_UPDATER(type_name, type_letter, num_of_params) \
                                                                            \
  template <>                                                               \
  void Uniform::UpdateValueFromArray<num_of_params##ul, type_name>(         \
      std::uint32_t id,                                                     \
      const void* const data,                                               \
      std::size_t number_of_vectors) {                                      \
    glUniform##num_of_params##type_letter##v(                               \
        id, number_of_vectors, static_cast<const type_name*>(data));        \
  }

#define GENERATE_SPECIALIZATIONS(type_name, type_letter)                  \
  GENERATE_PACK_VALUE_UPDATER(type_name, type_letter, 1, n1);             \
  GENERATE_PACK_VALUE_UPDATER(type_name, type_letter, 2, n1, n2);         \
  GENERATE_PACK_VALUE_UPDATER(type_name, type_letter, 3, n1, n2, n3);     \
  GENERATE_PACK_VALUE_UPDATER(type_name, type_letter, 4, n1, n2, n3, n4); \
                                                                          \
  GENERATE_ARRAY_VALUE_UPDATER(type_name, type_letter, 1);                \
  GENERATE_ARRAY_VALUE_UPDATER(type_name, type_letter, 2);                \
  GENERATE_ARRAY_VALUE_UPDATER(type_name, type_letter, 3);                \
  GENERATE_ARRAY_VALUE_UPDATER(type_name, type_letter, 4)

namespace gl {

GENERATE_SPECIALIZATIONS(float, f);
GENERATE_SPECIALIZATIONS(int, i);

}  // namespace gl
