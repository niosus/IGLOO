cc_library(
    name = "shader",
    srcs = ["shader.cpp"],
    hdrs = [
      "gl_base.h",
      "shader.h",
      "program.h",
      "buffer.h",
      "buffer_traits.h",
      "vertex_array_buffer.h",
    ],
    deps = [
        "//opengl_tutorials/utils:eigen_utils",
        "//opengl_tutorials/utils:file_utils",
        "//third_party/glad:glad",
        "@com_google_absl//absl/strings:strings",
        "@com_google_absl//absl/strings:str_format",
    ],
    visibility = ["//opengl_tutorials:__subpackages__"],
)
