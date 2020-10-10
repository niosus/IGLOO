package(default_visibility = [
    "//gl:__subpackages__",
    "//examples:__subpackages__",
])

cc_library(
    name = "viewer",
    srcs = [
        "glfw_user_input_handler.cpp",
        "viewer.cpp",
    ],
    hdrs = [
        "glfw_user_input_handler.h",
        "viewer.h"
    ],
    deps = [
        "//gl/core:core",
        "//gl/ui/core:user_input_handler",
        "@glfw//:glfw",
    ],
)
