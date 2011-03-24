#ifndef wali_nwa_DEPRECATE_H
#define wali_nwa_DEPRECATE_H

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


#endif
