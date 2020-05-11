cc_library(
    name = "scene",
    srcs = [
        "font.cpp",
    ],
    hdrs = [
        "font.h",
    ],
    visibility = [
        "//gl:__subpackages__",
        "//opengl_tutorials:__subpackages__",
    ],
    deps = [
        "//gl/core:core",
        "//utils:file_utils",
        "@abseil//absl/strings",
        "@com_github_glog_glog//:glog",
    ],
)

cc_test(
    name = "scene_test",
    srcs = [
        "font_test.cpp",
    ],
    deps = [
        ":scene",
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ],
    data = [":fonts"],
    size="small",
)

filegroup(
    name = "fonts",
    srcs = glob(["fonts/*"]),
)
