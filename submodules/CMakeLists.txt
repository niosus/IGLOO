add_subdirectory(eigen)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
target_include_directories(glfw PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/submodules/glfw/include>  
    $<INSTALL_INTERFACE:include>)

add_subdirectory(glog)

add_subdirectory(abseil)
add_subdirectory(googletest)

add_library(stb_image INTERFACE stb/stb_image.h)
target_include_directories(
    stb_image INTERFACE ${PROJECT_SOURCE_DIR}/submodules)

add_subdirectory(glad/cmake glad_cmake)
glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)
