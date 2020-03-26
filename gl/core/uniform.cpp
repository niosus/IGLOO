#include "gl/core/uniform.h"

// Accept any number of args >= N, but expand to just the Nth one.
// Here, N == 6.
#define _GET_NTH_ARG(_1, _2, _3, _4, _5, N, ...) N

// Define some macros to help us create overrides based on the
// arity of a for-each-style macro.
#define _fe_0(_call, ...)
#define _fe_1(_call, x) _call(x)
#define _fe_2(_call, x, ...) _call(x), _fe_1(_call, __VA_ARGS__)
#define _fe_3(_call, x, ...) _call(x), _fe_2(_call, __VA_ARGS__)
#define _fe_4(_call, x, ...) _call(x), _fe_3(_call, __VA_ARGS__)

#define _PREFIX_float(what) float what
#define _PREFIX_int(what) int what

#define _PREFIX_EACH_OF(prefix_macro, ...)                                  \
  _GET_NTH_ARG("ignored", ##__VA_ARGS__, _fe_4, _fe_3, _fe_2, _fe_1, _fe_0) \
  (prefix_macro, ##__VA_ARGS__)

#define GENERATE_PACK_VALUE_UPDATER(                                           \
    type_name, type_letter, num_of_params, ...)                                \
  template <>                                                                  \
  void Uniform::UpdateValueFromPack(std::uint32_t id,                          \
                                    _PREFIX_EACH_OF(type_name, __VA_ARGS__)) { \
    glUniform##num_of_params##type_letter(id, __VA_ARGS__);                    \
  }

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
