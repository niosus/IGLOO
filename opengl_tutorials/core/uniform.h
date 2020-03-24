#ifndef OPENGL_TUTORIALS_CORE_GL_UNIFORM_H_
#define OPENGL_TUTORIALS_CORE_GL_UNIFORM_H_

#include "opengl_tutorials/core/buffer_traits.h"
#include "opengl_tutorials/core/gl_base.h"
#include "opengl_tutorials/utils/type_utils.h"

#include <iostream>
#include <string>
#include <vector>

namespace gl_tutorials {

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
        type_utils::has_value_member<
            typename traits::components_per_vertex_count<T>>::value,
        "Missing specialization for trait 'components_per_vertex_count'");
    static_assert(
        type_utils::has_type_member<typename traits::underlying_type<T>>::value,
        "Missing specialization for trait 'underlying_type'");
    using UnderlyingType = typename traits::underlying_type<T>::type;
    UpdateValueFromArray<traits::components_per_vertex_count<T>::value,
                         UnderlyingType>(static_cast<const void*>(data),
                                         number_of_elements);
  }

  template <typename... Ts>
  void UpdateValue(Ts... numbers) {
    UpdateValueFromPack<sizeof...(Ts), Ts...>(numbers...);
  }

 private:
  template <size_t kNumberOfElementsInPack, typename... Ts>
  void UpdateValueFromPack(Ts... numbers) {
    std::cerr << "Base UpdateValueFromPack should not be called." << std::endl;
  }

  template <size_t kComponentCount, typename T>
  void UpdateValueFromArray(const void* const data,
                            std::size_t number_of_vectors) {
    std::cerr << "Base UpdateValueFromArray should not be called." << std::endl;
  }

  std::string name_;
};

template <>
void Uniform::UpdateValueFromPack<1ul, float>(float n1) {
  glUniform1f(id_, n1);
}

template <>
void Uniform::UpdateValueFromPack<2ul, float>(float n1, float n2) {
  glUniform2f(id_, n1, n2);
}

template <>
void Uniform::UpdateValueFromPack<3ul, float>(float n1, float n2, float n3) {
  glUniform3f(id_, n1, n2, n3);
}

template <>
void Uniform::UpdateValueFromPack<4ul, float>(float n1,
                                              float n2,
                                              float n3,
                                              float n4) {
  glUniform4f(id_, n1, n2, n3, n4);
}

template <>
void Uniform::UpdateValueFromArray<1, float>(const void* const data,
                                             std::size_t number_of_vectors) {
  glUniform1fv(id_, number_of_vectors, static_cast<const float*>(data));
}

template <>
void Uniform::UpdateValueFromArray<2, float>(const void* const data,
                                             std::size_t number_of_vectors) {
  glUniform2fv(id_, number_of_vectors, static_cast<const float*>(data));
}

template <>
void Uniform::UpdateValueFromArray<3, float>(const void* const data,
                                             std::size_t number_of_vectors) {
  glUniform3fv(id_, number_of_vectors, static_cast<const float*>(data));
}

template <>
void Uniform::UpdateValueFromArray<4, float>(const void* const data,
                                             std::size_t number_of_vectors) {
  glUniform4fv(id_, number_of_vectors, static_cast<const float*>(data));
}

}  // namespace gl_tutorials

#endif  // OPENGL_TUTORIALS_CORE_GL_UNIFORM_H_
