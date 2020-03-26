#ifndef OPENGL_TUTORIALS_CORE_GL_UNIFORM_H_
#define OPENGL_TUTORIALS_CORE_GL_UNIFORM_H_

#include "gl/core/buffer_traits.h"
#include "gl/core/opengl_object.h"
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
        ::traits::has_value_member<
            typename traits::components_per_vertex_count<T>>::value,
        "Missing specialization for trait 'components_per_vertex_count'");
    static_assert(
        ::traits::has_type_member<typename traits::underlying_type<T>>::value,
        "Missing specialization for trait 'underlying_type'");
    using UnderlyingType = typename traits::underlying_type<T>::type;
    UpdateValueFromArray<traits::components_per_vertex_count<T>::value,
                         UnderlyingType>(
        id_, static_cast<const void*>(data), number_of_elements);
  }

  template <typename... Ts>
  void UpdateValue(Ts... numbers) {
    UpdateValueFromPack(id_, numbers...);
  }

  inline const std::string& name() const { return name_; }

 private:
  template <typename... Ts>
  void UpdateValueFromPack(std::uint32_t id, Ts... n1);

  template <std::size_t kComponentCount, typename T>
  void UpdateValueFromArray(std::uint32_t id,
                            const void* const data,
                            std::size_t number_of_vectors);

  std::string name_;
};

}  // namespace gl

#endif  // OPENGL_TUTORIALS_CORE_GL_UNIFORM_H_
