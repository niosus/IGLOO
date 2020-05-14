cc_library(
    name = "drawables",
    srcs = [
        "drawables/drawable.cpp",
        "drawables/all.cpp",
    ],
    hdrs = [
        "drawables/drawable.h",
        "drawables/all.h",
    ],
    visibility = [
        "//gl:__subpackages__",
        "//opengl_tutorials:__subpackages__",
    ],
    deps = [
        ":scene",
        "//gl/core:core",
        "//gl/utils:eigen_traits",
        "//utils:eigen_utils",
        "//utils:file_utils",
        "@abseil//absl/flags:flag",
        "@abseil//absl/flags:parse",
        "@abseil//absl/strings",
        "@com_github_glog_glog//:glog",
        "@eigen//:eigen",
    ],
)

cc_library(
    name = "scene",
    srcs = [
        "font.cpp",
        "font_pool.cpp",
        "program_pool.cpp",
    ],
    hdrs = [
        "font.h",
        "font_pool.h",
        "program_pool.h",
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
        "font_pool_test.cpp",
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
