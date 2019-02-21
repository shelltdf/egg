
#ifndef _EGG_DATATYPE_H_
#define _EGG_DATATYPE_H_

#if _MSC_VER
#if _WIN64
#define HANDLE_TYPE unsigned long long
#else
#define HANDLE_TYPE unsigned int
#endif
#else
#define HANDLE_TYPE unsigned int
#endif


#endif // !_EGG_DATATYPE_H_
