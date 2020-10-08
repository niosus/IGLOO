package(default_visibility = [
    "//gl:__subpackages__",
    "//examples:__subpackages__",
])

cc_library(
    name = "viewer",
    srcs = ["viewer.cpp"],
    hdrs = [
        "mouse_event_handler.h",
        "viewer.h"
    ],
    deps = [
        "//gl/core:core",
        "//gl/ui/core:user_input_handler",
        "@glfw//:glfw",
    ],
)
