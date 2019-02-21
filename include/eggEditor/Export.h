#ifndef _EGG_EDITOR_EXPORT_H
#define _EGG_EDITOR_EXPORT_H

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#if defined(EGG_LIBRARY_STATIC)
#   define EGG_EDITOR_EXPORT
#elif defined(EGG_EDITOR_LIBRARY)
#   define EGG_EDITOR_EXPORT __declspec(dllexport)
#else
#   define EGG_EDITOR_EXPORT __declspec(dllimport)
#endif
#else
#define EGG_EDITOR_EXPORT
#endif

#endif//_EGG_EDITOR_EXPORT_H
