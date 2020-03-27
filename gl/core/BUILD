load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "core",
    srcs = [
        "shader.cpp",
        "uniform.cpp",
    ],
    hdrs = [
        "buffer.h",
        "texture.h",
        "traits.h",
        "opengl_object.h",
        "program.h",
        "shader.h",
        "uniform.h",
        "vertex_array_buffer.h",
    ],
    visibility = [
        "//gl:__subpackages__",
        "//opengl_tutorials:__subpackages__",
    ],
    deps = [
        "//third_party/glad",
        "//utils:file_utils",
        "//utils:type_traits",
        "//utils:macro_utils",
        "//utils:image",
        "@abseil//absl/strings",
        "@abseil//absl/strings:str_format",
    ],
)

cc_test(
    name = "core_test",
    srcs = [
        "buffer_test.cpp",
        "main_test.cpp",
        "uniform_test.cpp",
    ],
    deps = [
        ":core",
        "//gl/ui/glfw:viewer",
        "//gl/utils:eigen_traits",
        "//utils:eigen_utils",
        "@gtest//:gtest",
    ],
    size="small",
)
