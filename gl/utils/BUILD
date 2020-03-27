package(default_visibility = [
    "//gl:__subpackages__",
    "//opengl_tutorials:__subpackages__",
])

cc_library(
    name = "eigen_traits",
    hdrs = ["eigen_traits.h"],
    deps = [
        "//gl/core:core",
        "@eigen//:eigen",
    ]
)

cc_test(
    name = "eigen_traits_test",
    srcs = [
        "eigen_traits_test.cpp",
    ],
    deps = [
        ":eigen_traits",
        "@eigen//:eigen",
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ],
    size="small",
)
