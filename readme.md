<p align="center">
  <img width="200" height="200" align="right" src=".github/icon/igloo.png">
</p>

# IGLOO
#### **I**ntuitive open**GL** **O**bject **O**riented library

![GitHub Workflow Status (branch)](https://img.shields.io/github/workflow/status/niosus/IGLOO/CI/master?style=for-the-badge)

This is a library that wraps OpenGL library into objects. It aims to be close
to zero-overhead in terms of runtime while offering intuitive and safe
interfaces to underlying OpenGL library. The core idea is to make it hard to
make a mistake that is hard to debug while not incurring too high cost for
such guarantees. The library tries to use static memory allocation where
possible, using traits to make sure the sizes of types are guessed correctly
and are translated into OpenGL types properly.

#### 🚧 Under construction 🚧

I am by far not an expert in OpenGL and constructive feedback and/or
corrections are welcome. This library is also not complete and will be so for
the observable future. Things may change, APIs broken and all the hell can
break loose. That being said, I would love to see other people using this and
hopefully someday it will reach a stable state. 

## How to build and test

Building the library and running the tests should be as simple as:
```bash
bazel build //...  # build
bazel test --test_output=errors --test_env=DISPLAY=:0 //...  # test
```

### Prerequisites

The build is not fully hermetic and relies on some libraries present on your
system. On Ubuntu 20.04 you would need to install some libraries. You can
find the list in the [build job script](.github/workflows/main.yml).

## Examples

I try to follow the tutorials from this awesome tutorial:
https://learnopengl.com/ 

You can find the implementations for some of these using IGLOO in the folder
[examples/opengl_tutorials/](examples/opengl_tutorials/)

## Credits

The underlying library is inspired by other efforts to achieve OOP approach to
OpenGL. Most notably a library of my ex-colleague [Dr. Jens Behley]
(https://github.com/jbehley): https://github.com/jbehley/glow

<div>Icon made by <a href="https://www.flaticon.com/authors/vignesh-oviyan" title="Vignesh Oviyan">Vignesh Oviyan</a> from <a href="https://www.flaticon.com/" title="Flaticon">www.flaticon.com</a></div>

