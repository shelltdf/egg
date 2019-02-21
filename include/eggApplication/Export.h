#ifndef _EGG_APPLICITION_EXPORT_H
#define _EGG_APPLICITION_EXPORT_H

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#if defined(EGG_LIBRARY_STATIC)
#   define EGG_APPLICITION_EXPORT
#elif defined(EGG_APPLICITION_LIBRARY)
#   define EGG_APPLICITION_EXPORT __declspec(dllexport)
#else
#   define EGG_APPLICITION_EXPORT __declspec(dllimport)
#endif
#else
#define EGG_APPLICITION_EXPORT
#endif

#endif//_EGG_APPLICITION_EXPORT_H
