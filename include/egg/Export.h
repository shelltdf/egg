#ifndef _EGG_RUNTIME_EXPORT_H
#define _EGG_RUNTIME_EXPORT_H

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#if defined(EGG_LIBRARY_STATIC)
#   define EGG_RUNTIME_EXPORT
#elif defined(EGG_RUNTIME_LIBRARY)
#   define EGG_RUNTIME_EXPORT __declspec(dllexport)
#else
#   define EGG_RUNTIME_EXPORT __declspec(dllimport)
#endif
#else
#define EGG_RUNTIME_EXPORT
#endif


#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#if defined(EGG_LIBRARY_STATIC)
#   define EGG_MODULE_EXPORT
#else
#   define EGG_MODULE_EXPORT __declspec(dllexport)
#endif
#else
#define EGG_RUNTIME_EXPORT
#endif

#endif//_EGG_RUNTIME_EXPORT_H
