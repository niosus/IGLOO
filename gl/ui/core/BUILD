package(default_visibility = [
    "//gl:__subpackages__",
    "//opengl_tutorials:__subpackages__",
])

cc_library(
    name = "user_input_handler",
    hdrs = ["user_input_handler.h"],
)

cc_test(
    name = "user_input_handler_test",
    srcs = [
        "user_input_handler_test.cpp",
    ],
    deps = [
        ":user_input_handler",
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ],
    size="small",
)
