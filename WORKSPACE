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
    sha256 = "08a33a512f29d7dbf78eab39bd7858576adcc95228c9efe8e4bc5f0f3261efc7",
    strip_prefix = "glfw-3.3.2",
    build_file = "@//:third_party/glfw/glfw.BUILD",
)

# abseil-cpp
http_archive(
  name = "com_google_absl",
  urls = ["https://github.com/abseil/abseil-cpp/archive/20190808.zip"],  # 2019-03-14
  strip_prefix = "abseil-cpp-20190808",
  sha256 = "0b62fc2d00c2b2bc3761a892a17ac3b8af3578bd28535d90b4c914b0a7460d4e",
)

# Google Test
http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/8b6d3f9c4a774bef3081195d422993323b6bb2e0.zip"],  # 2019-03-05
  strip_prefix = "googletest-8b6d3f9c4a774bef3081195d422993323b6bb2e0",
  sha256 = "d21ba93d7f193a9a0ab80b96e8890d520b25704a6fac976fe9da81fffb3392e3",
)

http_archive(
  name = "org_tuxfamily_eigen",
  build_file = "@//:third_party/eigen/eigen.BUILD",
  sha256 = "ca7beac153d4059c02c8fc59816c82d54ea47fe58365e8aded4082ded0b820c4",
  strip_prefix = "eigen-eigen-f3a22f35b044",
  urls = [
      "http://mirror.bazel.build/bitbucket.org/eigen/eigen/get/f3a22f35b044.tar.gz",
      "https://bitbucket.org/eigen/eigen/get/f3a22f35b044.tar.gz",
  ],
)

http_archive(
  name = "org_libpng_libpng",
  build_file = "@//:third_party/libpng/libpng.BUILD",
  sha256 = "7f415186d38ca71c23058386d7cf5135c8beda821ee1beecdc2a7a26c0356615",
  strip_prefix = "libpng-1.2.57",
  urls = [
      "https://mirror.bazel.build/github.com/glennrp/libpng/archive/v1.2.57.tar.gz",
      "https://github.com/glennrp/libpng/archive/v1.2.57.tar.gz",
  ],
)

http_archive (
  name = "net_zlib_zlib",
  sha256 = "6d4d6640ca3121620995ee255945161821218752b551a1a180f4215f7d124d45",
  build_file = "@//:third_party/zlib/zlib.BUILD",
  strip_prefix = "zlib-cacf7f1d4e3d44d871b605da3b647f07d718623f",
  urls = [
      "https://mirror.bazel.build/github.com/madler/zlib/archive/cacf7f1d4e3d44d871b605da3b647f07d718623f.tar.gz",
      "https://github.com/madler/zlib/archive/cacf7f1d4e3d44d871b605da3b647f07d718623f.tar.gz",
  ],
)
