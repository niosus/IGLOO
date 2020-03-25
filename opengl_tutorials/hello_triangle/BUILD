load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "hello_triangle",
    srcs = ["main.cpp"],
    data = [":shaders"],
    deps = [
        "//gl/core",
        "//gl/ui/glfw:viewer",
        "//gl/utils:eigen_traits",
        "//third_party/glad",
        "//utils:eigen_utils",
        "@abseil//absl/flags:flag",
        "@abseil//absl/flags:parse",
        "@abseil//absl/strings",
        "@glfw",
        "@eigen//:eigen",
    ],
)

filegroup(
    name = "shaders",
    srcs = glob(["shaders/*"]),
)
