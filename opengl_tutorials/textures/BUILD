load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "textures_tutorial",
    srcs = ["main.cpp"],
    data = [
        ":shaders",
        ":textures"
    ],
    deps = [
        "//gl/core",
        "//gl/ui/glfw:viewer",
        "//gl/utils:eigen_traits",
        "//third_party/glad",
        "//utils:eigen_utils",
        "//utils:image",
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

filegroup(
    name = "textures",
    srcs = glob(["textures/*"]),
)
