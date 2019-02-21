#ifndef _eggNet_EXPORT_H
#define _eggNet_EXPORT_H_

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#if defined(EGG_LIBRARY_STATIC)
#   define EGGNET_EXPORT
#elif defined(EGGNET_LIBRARY)
#   define EGGNET_EXPORT __declspec(dllexport)
#else
#   define EGGNET_EXPORT __declspec(dllimport)
#endif
#else
#define EGGNET_EXPORT
#endif

#endif //_eggNet_EXPORT_H_

