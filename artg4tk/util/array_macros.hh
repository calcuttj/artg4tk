#ifndef artg4tk_util_array_macros_hh
#define artg4tk_util_array_macros_hh

/** @file array_macros.hh

    This file provides a set of types, macros, and functions to derive
    the length of an array whose name is in scope.  Templates are so
    cool.  :-)

    Most of the code is from "Imperfect C++" by Matthew Wilson

    Kevin Lynch 2009

    Copied to artg4tk
    Brendan Kiburg 2013
 */

namespace {
  typedef unsigned char byte_t;

  template <int N>
  struct array_size_struct {
    byte_t c[N];
  };
}

/** This function declaration has no associated implementation; it
    exists solely for compile time argument type deduction. Don't even
    think about using it. */
template <typename T, int N>
array_size_struct<N> static_array_size_fn(T (&)[N]);

/** This macro should only be applied when @a x is the name of an
    array which is in scope, or all hell will break loose, with many
    pages of template instantiation errors pouring forth from the
    compiler. */
#define dimensionof(x) sizeof(static_array_size_fn(x).c)

#if !defined(ARRAYEND)
/** Returns the "end iterator" (pointer) given an in-scope array name
    @a x. */
#define ARRAYEND(x) (x + dimensionof(x))
#else
#error ARRAYEND defined already!
#endif

#if !defined(ARRAYBEGIN)
/** Returns the "begin iterator" (pointer) given by the in-scope array
    name @a x. */
#define ARRAYBEGIN(x) x
#else
#error ARRAYBEGIN defined already!
#endif

#endif /* artg4tk_util_array_macros_hh */
