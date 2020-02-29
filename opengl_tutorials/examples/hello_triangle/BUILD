cc_binary(
    name = "hello_triangle",
    srcs = ["main.cpp"],
    deps = [
        "//third_party/glad:glad",
        "//opengl_tutorials/core:shader",
        "//opengl_tutorials/utils:file_utils",
        "@glew//:glew",
        "@glfw//:glfw",
    ],
    data = [":shaders"],
)

filegroup(
    name = "shaders",
    srcs = glob(["shaders/*"])
)

