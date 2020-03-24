cc_library(
    name = "core",
    srcs = ["shader.cpp"],
    hdrs = [
      "gl_base.h",
      "shader.h",
      "program.h",
      "buffer.h",
      "buffer_traits.h",
      "uniform.h",
      "vertex_array_buffer.h",
    ],
    deps = [
        "//opengl_tutorials/utils:file_utils",
        "//opengl_tutorials/utils:type_utils",
        "//third_party/glad:glad",
        "@com_google_absl//absl/strings:strings",
        "@com_google_absl//absl/strings:str_format",
    ],
    visibility = ["//opengl_tutorials:__subpackages__"],
)

cc_test(
    name = "core_test",
    srcs = [
        "uniform_test.cpp",
        "main_test.cpp"
    ],
    deps = [
        "//opengl_tutorials/ui/glfw:viewer",
        "//opengl_tutorials/utils:eigen_utils",
        ":core",
        "@com_google_googletest//:gtest",
    ],
)


