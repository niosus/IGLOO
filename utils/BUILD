load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
    "//visibility:public",
])

cc_library(
    name = "eigen_utils",
    hdrs = ["eigen_utils.h"],
    deps = [
        "@org_tuxfamily_eigen//:eigen",
    ],
)

cc_library(
    name = "file_utils",
    hdrs = ["file_utils.h"],
    deps = [
        "@com_google_absl//absl/types:optional",
    ],
)

cc_library(
    name = "type_traits",
    hdrs = ["type_traits.h"],
)
