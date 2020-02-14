load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "io_bazel_rules_python",
    remote = "https://github.com/bazelbuild/rules_python.git",
    commit = "965d4b4a63e6462204ae671d7c3f02b25da37941",
)

load("@io_bazel_rules_python//python:pip.bzl", "pip_repositories")
pip_repositories()

load("@io_bazel_rules_python//python:pip.bzl", "pip_import")

pip_import(
   name = "requirements",
   requirements = "//:requirements.txt",
)

load("@requirements//:requirements.bzl", "pip_install")
pip_install()

http_archive(
    name = "glfw",
    urls = ["https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.zip"],
    strip_prefix = "glfw-3.3.2",
    build_file = "@//:third_party/glfw/glfw.BUILD",
)

http_archive(
    name = "glew",
    urls = ["https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.zip"],
    strip_prefix = "glew-2.1.0",
    build_file = "@//:third_party/glew/glew.BUILD",
)

