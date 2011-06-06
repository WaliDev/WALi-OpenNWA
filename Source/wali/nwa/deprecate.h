#ifndef wali_nwa_DEPRECATE_H
#define wali_nwa_DEPRECATE_H

// Define the DEPRECATE macro

#if defined(__GNUC__)
#  define TEMP_GCC_VERSION (__GNUC__*10000 + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__)
#  if TEMP_GCC_VERSION > 40500 // if at least 4.5
#    define DEPRECATE(msg) __attribute__((__deprecated__(msg)))
#  else
#    define DEPRECATE(msg) __attribute__((__deprecated__))
#  endif
#elif defined(_MSC_VER)
#  define DEPRECATE(msg) __declspec(deprecated(msg))
#else
#  warning I do not know how to deprecate something with your compiler
#endif

// Define macros to turn on and off deprecation. Uh, I don't think
// these really work right now.
#if defined(__GNUC__)
#  define TEMP_GCC_VERSION (__GNUC__*10000 + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__)
#  if TEMP_GCC_VERSION > 40600 // if at least 4.6
#    define DEPRECATE_PUSH_AND_DISABLE _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#    define DEPRECATE_POP              _Pragma("GCC diagnostic pop")
#  else
//   We can't actually push and pop in GCC < 4.6, so just assume it was on and turn it back on when done
#    define DEPRECATE_PUSH_AND_DISABLE _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#    define DEPRECATE_POP              _Pragma("GCC diagnostic warning \"-Wdeprecated-declarations\"")
#  endif
#elif defined(_MSC_VER)
#  define DEPRECATE_PUSH_AND_DISABLE __pragma(warning(push)) __pragma(warning(disable:4996))
#  define DEPRECATE_POP              __pragma(warning(pop))
#else
#  warning I do not know how to turn on and off deprecated warnings
#endif


#endif
