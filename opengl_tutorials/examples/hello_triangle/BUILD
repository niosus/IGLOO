cc_binary(
    name = "hello_triangle",
    srcs = ["main.cpp"],
    deps = [
        "//opengl_tutorials/core:shader",
        "//opengl_tutorials/utils:file_utils",
        "//third_party/glad:glad",
        "@glfw//:glfw",
    ],
    data = [":shaders"],
)

filegroup(
    name = "shaders",
    srcs = glob(["shaders/*"])
)

