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
        "//examples:__subpackages__",
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
    data = [":shaders", ":fonts"]
)

cc_test(
    name = "scene_test",
    srcs = [
        "font_test.cpp",
        "font_pool_test.cpp",
        "scene_graph_test.cpp",
        "program_pool_test.cpp",
        "main_test.cpp",
    ],
    deps = [
        ":scene",
        "//gl/ui/glfw:viewer",
        "@gtest//:gtest",
    ],
    size="small",
)

filegroup(
    name = "shaders",
    srcs = glob(["shaders/*"]),
)

filegroup(
    name = "fonts",
    srcs = glob(["fonts/*"]),
)
