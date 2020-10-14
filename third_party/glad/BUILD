load("@requirements//:requirements.bzl", "requirement")

py_binary(
    name = "run_glad",
    srcs = ["run_glad.py"],
    deps = [requirement("glad")],
)

RUN_GLAD_CMD =\
"$(location :run_glad) " +\
    "--profile=core " +\
    "--generator=c " +\
    "--reproducible " +\
    "--local-files " +\
    "--out-path=$(@D)"

genrule(
    name = "gen_glad_hdrs",
    tools = [":run_glad"],
    outs = [
        "glad.h",
        "khrplatform.h",
    ],
    cmd = RUN_GLAD_CMD
)

genrule(
    name = "gen_glad_srcs",
    tools = [":run_glad"],
    outs = [
        "glad.c",
    ],
    cmd = RUN_GLAD_CMD
)

cc_library(
    name = "glad",
    srcs = [":gen_glad_srcs"],
    hdrs = [":gen_glad_hdrs"],
    linkopts = ["-ldl"],
    visibility = ["//visibility:public"],
)
