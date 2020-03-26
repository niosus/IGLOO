#ifndef OPENGL_TUTORIALS_UTILS_MACRO_UTILS_H_
#define OPENGL_TUTORIALS_UTILS_MACRO_UTILS_H_

// Accept any number of args >= N, but expand to just the Nth one.
// Here, N == 6.
#define _GET_NTH_ARG(_1, _2, _3, _4, _5, N, ...) N

// Define some macros to help us create overrides based on the
// arity of a for-each-style macro.
#define _fe_0(_call, ...)
#define _fe_1(_call, x) _call(x)
#define _fe_2(_call, x, ...) _call(x), _fe_1(_call, __VA_ARGS__)
#define _fe_3(_call, x, ...) _call(x), _fe_2(_call, __VA_ARGS__)
#define _fe_4(_call, x, ...) _call(x), _fe_3(_call, __VA_ARGS__)

#define _PREFIX_float(what) float what
#define _PREFIX_int(what) int what

#define _PREFIX_EACH_OF(prefix_macro, ...)                                  \
  _GET_NTH_ARG("ignored", ##__VA_ARGS__, _fe_4, _fe_3, _fe_2, _fe_1, _fe_0) \
  (prefix_macro, ##__VA_ARGS__)

#endif  // OPENGL_TUTORIALS_UTILS_MACRO_UTILS_H_
