#ifndef _EGG_ENGINE_EXPORT_H
#define _EGG_ENGINE_EXPORT_H

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#if defined(EGG_LIBRARY_STATIC)
#   define EGG_ENGINE_EXPORT
#elif defined(EGG_ENGINE_LIBRARY)
#   define EGG_ENGINE_EXPORT __declspec(dllexport)
#else
#   define EGG_ENGINE_EXPORT __declspec(dllimport)
#endif
#else
#define EGG_ENGINE_EXPORT
#endif

#endif//_EGG_ENGINE_EXPORT_H
