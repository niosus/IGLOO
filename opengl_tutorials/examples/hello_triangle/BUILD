cc_binary(
    name = "hello_triangle",
    srcs = ["main.cpp"],
    deps = [
        "//opengl_tutorials/core:core",
        "//opengl_tutorials/ui/glfw:viewer",
        "//opengl_tutorials/utils:eigen_utils",
        "//third_party/glad:glad",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings",
        "@glfw//:glfw",
        "@org_tuxfamily_eigen//:eigen"
    ],
    data = [":shaders"],
)

filegroup(
    name = "shaders",
    srcs = glob(["shaders/*"])
)

