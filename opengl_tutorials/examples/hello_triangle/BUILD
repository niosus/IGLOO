cc_binary(
    name = "hello_triangle",
    srcs = ["main.cpp"],
    deps = [
        "//third_party/glad:glad",
        "//opengl_tutorials/utils:shader_loader",
        "@glew//:glew",
        "@glfw//:glfw",
    ],
    data = [":shaders"],
)

filegroup(
    name = "shaders",
    srcs = glob(["shaders/*"])
)

