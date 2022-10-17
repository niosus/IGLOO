add_library(core
  shader.cpp
  uniform.cpp
  texture.cpp
  program.cpp
)
target_link_libraries(core PUBLIC
    file_utils
    type_traits
    macro_utils
    image
    glad_gl_core_33
    glog::glog
    absl::strings
    absl::str_format
)

add_executable(core_test
  buffer_test.cpp
  shader_test.cpp
  program_test.cpp
  uniform_test.cpp
  main_test.cpp
)
target_link_libraries(core_test PRIVATE
  core
  glfw_viewer
  eigen_utils
  GTest::gtest
)
