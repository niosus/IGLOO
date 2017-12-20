add_library(glad glad.c)

add_executable(main main.cpp)
target_link_libraries(main
  glad
  ${GLFW_STATIC_LIBRARIES}
)

