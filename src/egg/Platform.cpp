
#include <egg/Platform.h>

using namespace egg;

IMPLEMENT_SINGLETON(Platform)

#if _WIN32
#include "./Platform/win32.cpp"
#endif

