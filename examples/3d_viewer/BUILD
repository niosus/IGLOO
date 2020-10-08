load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_library(
    name = "utils",
    hdrs = [
        "utils/point_cloud.h",
    ],
    srcs = [
        "utils/point_cloud.cpp"
    ],
    deps = [
        "@com_github_glog_glog//:glog",
        "@abseil//absl/strings",
        "//utils:eigen_utils",
    ],
)

cc_test(
    name = "utils_test",
    srcs = [
        "utils/point_cloud_test.cpp"
    ],
    deps = [
        ":utils",
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ],
    size="small",
    data = [":test_data"]
)


cc_binary(
    name = "3d_viewer_example",
    srcs = ["main.cpp"],
    deps = [
        ":utils",
        "//gl/core",
        "//gl/scene",
        "//gl/viewer",
    ],
    data = [":test_data"]
)

filegroup(
    name = "test_data",
    srcs = glob(["utils/test_data/*"]),
)

