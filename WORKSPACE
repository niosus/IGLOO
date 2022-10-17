load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")


load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "rules_python",
    sha256 = "9fcf91dbcc31fde6d1edb15f117246d912c33c36f44cf681976bd886538deba6",
    strip_prefix = "rules_python-0.8.0",
    url = "https://github.com/bazelbuild/rules_python/archive/refs/tags/0.8.0.tar.gz",
)

load("@rules_python//python:repositories.bzl", "python_register_toolchains")

python_register_toolchains(
    name = "python3_9",
    # Available versions are listed in @rules_python//python:versions.bzl.
    # We recommend using the same version your team is already standardized on.
    python_version = "3.9",
)

load("@python3_9//:defs.bzl", "interpreter")

load("@rules_python//python:pip.bzl", "pip_install")

pip_install(
   name = "pip_dependencies",
   requirements = "//:requirements.txt",
)


http_archive(
    name = "glfw",
    urls = ["https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip"],
    strip_prefix = "glfw-3.3.8",
    build_file = "@//:third_party/glfw/glfw.BUILD",
)

# abseil-cpp
http_archive(
  name = "abseil",
  urls = ["https://github.com/abseil/abseil-cpp/archive/20190808.zip"],  # 2019-03-14
  strip_prefix = "abseil-cpp-20190808",
  sha256 = "0b62fc2d00c2b2bc3761a892a17ac3b8af3578bd28535d90b4c914b0a7460d4e",
)

# Google Test
http_archive(
  name = "gtest",
  urls = ["https://github.com/google/googletest/archive/8b6d3f9c4a774bef3081195d422993323b6bb2e0.zip"],  # 2019-03-05
  strip_prefix = "googletest-8b6d3f9c4a774bef3081195d422993323b6bb2e0",
  sha256 = "d21ba93d7f193a9a0ab80b96e8890d520b25704a6fac976fe9da81fffb3392e3",
)

http_archive(
  name = "eigen",
  build_file = "@//:third_party/eigen/eigen.BUILD",
  sha256 = "ca7beac153d4059c02c8fc59816c82d54ea47fe58365e8aded4082ded0b820c4",
  strip_prefix = "eigen-eigen-f3a22f35b044",
  urls = [
      "http://mirror.bazel.build/bitbucket.org/eigen/eigen/get/f3a22f35b044.tar.gz",
      "https://bitbucket.org/eigen/eigen/get/f3a22f35b044.tar.gz",
  ],
)

git_repository(
    name = "stb",
    init_submodules = True,
    commit = "f33d98e3a41a41eccd83d17fa88c4a6fedf552f9",
    shallow_since = "1572259405 +0000",
    remote = "https://github.com/nitronoid/rules_stb"
)

http_archive(
    name = "com_github_glog_glog",
    strip_prefix = "glog-0.4.0",
    sha256 = "9e1b54eb2782f53cd8af107ecf08d2ab64b8d0dc2b7f5594472f3bd63ca85cdc",
    urls = ["https://github.com/google/glog/archive/v0.4.0.zip"],
)

http_archive(
    name = "com_github_gflags_gflags",
    strip_prefix = "gflags-2.2.2",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    urls = [
        "https://mirror.bazel.build/github.com/gflags/gflags/archive/v2.2.2.tar.gz",
        "https://github.com/gflags/gflags/archive/v2.2.2.tar.gz",
    ],
)

http_archive(
  name = "com_nholthaus_units",
  urls = ["https://github.com/nholthaus/units/archive/v2.3.1.zip"],  # 2019-03-14
  strip_prefix = "units-2.3.1",
  build_file = "@//:third_party/units/units.BUILD",
)
