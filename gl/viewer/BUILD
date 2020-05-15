cc_library(
    name = "viewer",
    srcs = [
        "viewer.cpp",
    ],
    hdrs = [
        "viewer.h",
    ],
    visibility = [
        "//gl:__subpackages__",
        "//opengl_tutorials:__subpackages__",
    ],
    deps = [
        "//gl/core:core",
        "//gl/scene:scene",
        "//gl/ui/glfw:viewer",
        "//gl/utils:eigen_traits",
        "//gl/utils:camera",
        "//utils:eigen_utils",
        "//utils:file_utils",
        "@abseil//absl/flags:flag",
        "@abseil//absl/flags:parse",
        "@abseil//absl/strings",
        "@com_nholthaus_units//:units",
        "@com_github_glog_glog//:glog",
        "@eigen//:eigen",
    ],
)
