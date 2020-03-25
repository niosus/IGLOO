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
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings",
        "@glfw",
        "@org_tuxfamily_eigen//:eigen",
    ],
)

filegroup(
    name = "shaders",
    srcs = glob(["shaders/*"]),
)
