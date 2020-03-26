load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
    "//visibility:public",
])

cc_library(
    name = "eigen_utils",
    hdrs = ["eigen_utils.h"],
    deps = [
        "@eigen//:eigen",
    ],
)

cc_library(
    name = "file_utils",
    hdrs = ["file_utils.h"],
    deps = [
        "@abseil//absl/types:optional",
    ],
)

cc_library(
    name = "type_traits",
    hdrs = ["type_traits.h"],
)

cc_library(
    name = "image",
    hdrs = ["image.h"],
    deps = [
        ":file_utils",
        "@abseil//absl/types:optional",
        "@stb//:image"
    ],
)


cc_test(
    name = "image_test",
    srcs = [
        "image_test.cpp",
    ],
    deps = [
        ":image",
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ],
    size="small",
    data = [":test_images"]
)

filegroup(
    name = "test_images",
    srcs = glob(["test_images/*"]),
)
