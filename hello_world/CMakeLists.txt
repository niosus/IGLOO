cmake_minimum_required(VERSION 3.0.0)
project(opengl_hello_world)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(
  include
  ${GLFW_INCLUDE_DIRS}
)

add_subdirectory(src)
