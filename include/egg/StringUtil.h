#ifndef EGG_STRING_UTIL_H
#define EGG_STRING_UTIL_H

#include <egg/Export.h>

#include<vector>
#include<string>
#include<algorithm>    // std::search

namespace egg
{

    EGG_RUNTIME_EXPORT std::vector<std::string> split(
		const std::string& s, const std::string& delim
		, const bool keep_empty = true);


}//namespace nbBase

#endif//EGG_STRING_UTIL_H
