#ifndef OPENGL_TUTORIALS_CORE_GL_UNIFORM_H_
#define OPENGL_TUTORIALS_CORE_GL_UNIFORM_H_

#include "opengl_tutorials/core/buffer_traits.h"
#include "opengl_tutorials/core/gl_base.h"
#include "opengl_tutorials/utils/type_utils.h"

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

  template <typename T,
            typename std::enable_if<!std::is_floating_point<T>::value &&
                                    !std::is_integral<T>::value>::type>
  void UpdateValue(const T* const data, std::size_t number_of_elements) {
    static_assert(
        type_utils::has_value<
            typename traits::components_per_vertex_count<T>>::value,
        "Missing specialization for trait 'components_per_vertex_count'");
    UpdateValueArray<traits::components_per_vertex_count<T>::value, T>(
        data, number_of_elements);
  }

  template <typename... Ts>
  void UpdateValue(Ts... numbers) {
    UpdateValuePack<sizeof...(Ts), Ts...>(numbers...);
  }

 private:
  template <size_t kNumberOfElementsInPack, typename... Ts>
  void UpdateValuePack(Ts... numbers);

  template <size_t kComponentCount, typename T>
  void UpdateValueArray(const T* const data, std::size_t number_of_elements);

  std::string name_;
};

template <>
void Uniform::UpdateValuePack<1ul, float>(float n1) {
  glUniform1f(id_, n1);
}

template <>
void Uniform::UpdateValuePack<2ul, float>(float n1, float n2) {
  glUniform2f(id_, n1, n2);
}

template <>
void Uniform::UpdateValuePack<3ul, float>(float n1, float n2, float n3) {
  glUniform3f(id_, n1, n2, n3);
}

template <>
void Uniform::UpdateValuePack<4ul, float>(float n1,
                                          float n2,
                                          float n3,
                                          float n4) {
  glUniform4f(id_, n1, n2, n3, n4);
}

}  // namespace gl_tutorials

#endif  // OPENGL_TUTORIALS_CORE_GL_UNIFORM_H_
