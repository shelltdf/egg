#ifndef _egg_Codes_H
#define _egg_Codes_H

#include <egg/Export.h>
//#include <egg/Object.h>
#include <string>

namespace egg
{

    //зЊТы
    EGG_RUNTIME_EXPORT std::wstring m2w(const std::string& ch, unsigned int CodePage = /*CP_ACP*/0);
    EGG_RUNTIME_EXPORT std::string w2m(const std::wstring& wch, unsigned int CodePage = /*CP_ACP*/0);
    EGG_RUNTIME_EXPORT std::string gbk2utf8(const std::string& str);

}

#endif //_egg_Codes_H

