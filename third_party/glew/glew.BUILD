cc_library(
    name = "glew",
    srcs = [
        "src/glew.c",
        "src/glewinfo.c",
        "src/visualinfo.c"
    ],
    hdrs = [
        "include/GL/glew.h",
        "include/GL/eglew.h",
        "include/GL/glxew.h",
        "include/GL/wglew.h",
    ],
    linkopts = ["-lGL", "-ldl", "-lpthread"],
    includes = ["include"],
    visibility = ["//visibility:public"],
)
