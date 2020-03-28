#ifndef OPENGL_TUTORIALS_UTILS_MACRO_UTILS_H_
#define OPENGL_TUTORIALS_UTILS_MACRO_UTILS_H_

// Accept any number of args >= N, but expand to just the Nth one.
// Here, N == 6.
#define _GET_NTH_ARG(_1, _2, _3, _4, _5, N, ...) N

// Define a set of macros that call each other in a cascading fashion to create
// a chain of calls that covers all of the __VA_ARGS__. This is designed to call
// macro "macro" with a mandatory second argument "arg".
#define _fe_0(macro, arg, ...)
#define _fe_1(macro, arg, x) macro(arg, x)
#define _fe_2(macro, arg, x, ...) macro(arg, x), _fe_1(macro, arg, __VA_ARGS__)
#define _fe_3(macro, arg, x, ...) macro(arg, x), _fe_2(macro, arg, __VA_ARGS__)
#define _fe_4(macro, arg, x, ...) macro(arg, x), _fe_3(macro, arg, __VA_ARGS__)

// Some help macro to convert things to string.
#define QUOTE(...) #__VA_ARGS__
#define QUOTE_ALL(...) QUOTE(__VA_ARGS__)

/// Prefix var with a type.
#define PREFIX(type, var) type var

/// Prefix each of the __VA_ARGS__ with the prefix.
#define PREFIX_EACH_OF(prefix, ...)                                       \
  _GET_NTH_ARG("ignored", __VA_ARGS__, _fe_4, _fe_3, _fe_2, _fe_1, _fe_0) \
  (PREFIX, prefix, __VA_ARGS__)

/// Add a universal ref symbol (&&) before a type.
#define UNIVERSAL_REF(type) type&&

#endif  // OPENGL_TUTORIALS_UTILS_MACRO_UTILS_H_
