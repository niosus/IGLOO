add_library(glfw_viewer
  glfw_user_input_handler.cpp
  viewer.cpp
)
target_include_directories(glfw_viewer PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(glfw_viewer PUBLIC 
    glfw
    glad_gl_core_33 
    glog::glog)

# cc_library(
#     name = "viewer",
#     srcs = [
#         "glfw_user_input_handler.cpp",
#         "viewer.cpp",
#     ],
#     hdrs = [
#         "glfw_user_input_handler.h",
#         "viewer.h"
#     ],
#     deps = [
#         "//gl/core:core",
#         "//gl/ui/core:user_input_handler",
#         "@glfw//:glfw",
#     ],
# )
