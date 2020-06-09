cc_library(
    name = "scene",
    srcs = [
        "font.cpp",
        "font_pool.cpp",
        "program_pool.cpp",
        "scene_graph.cpp",
        "drawables/drawable.cpp",
        "drawables/all.cpp",
    ],
    hdrs = [
        "font.h",
        "font_pool.h",
        "program_pool.h",
        "scene_graph.h",
        "drawables/drawable.h",
        "drawables/all.h",
    ],
    visibility = [
        "//gl:__subpackages__",
        "//opengl_tutorials:__subpackages__",
    ],
    deps = [
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
    data = [":shaders"]
)

filegroup(
    name = "shaders",
    srcs = glob(["shaders/*"]),
)

cc_test(
    name = "scene_test",
    srcs = [
        "font_test.cpp",
        "font_pool_test.cpp",
        "scene_graph_test.cpp",
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
