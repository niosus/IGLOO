cc_library(
  name = "imgui",
  srcs = glob([
    '*.cpp',
    'examples/libs/include/gl3w/GL/gl3w.c',
    'examples/imgui_impl_glfw.cpp',
    'examples/imgui_impl_opengl3.cpp'
  ]),
  hdrs = glob([
    '*.h',
    'examples/libs/include/gl3w/GL/gl3w.h',
    'examples/imgui_impl_glfw.h',
    'examples/imgui_impl_opengl3.h'
  ]),
  deps = [
    "@glfw//:glfw",
  ],
  copts = [
      "-DIMGUI_IMPL_OPENGL_LOADER_GL3W",
  ],
  includes = ['examples/libs/include/gl3w'],
  visibility = ["//visibility:public"],
)
