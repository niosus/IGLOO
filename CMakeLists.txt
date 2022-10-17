cmake_minimum_required(VERSION 3.16...3.23)

project(igloo VERSION 0.0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(submodules)
add_subdirectory(utils)
add_subdirectory(gl)
