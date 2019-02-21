#include <egg/Codes.h>

#include <windows.h>

//using namespace egg;
namespace egg
{
    std::string gbk2utf8(const std::string& str)
    {
        return w2m(m2w(str, CP_ACP), CP_UTF8);
    }

    std::wstring m2w(const std::string& ch, unsigned int CodePage)
    {
        if (ch.empty())return L"";
        std::wstring ret;
        DWORD dwOutSize = 0;
        dwOutSize = MultiByteToWideChar(CodePage, 0, ch.c_str(), -1, NULL, 0);
        //wchar_t* pwText = new wchar_t[dwOutSize]; //可能的内存泄漏提示
        //pwText[dwOutSize-1] = L'\0';
        ret.resize(dwOutSize - 1);
        MultiByteToWideChar(CodePage, 0, ch.c_str(), ch.size(), &ret[0], dwOutSize);
        //ret = pwText;
        //if(!pwText) delete[] pwText;
        return ret;
    }

    std::string w2m(const std::wstring& wch, unsigned int CodePage)
    {
        std::string ret;
        DWORD dwOutSize = 0;
        dwOutSize = WideCharToMultiByte(CodePage, 0, wch.c_str(), -1, NULL, 0, NULL, FALSE);

        char *pwText = 0;
        pwText = new char[dwOutSize];
        pwText[dwOutSize - 1] = '\0';

        WideCharToMultiByte(CodePage, 0, wch.c_str(), wch.size(), pwText, dwOutSize, NULL, FALSE);

        ret = pwText;
        if (pwText)delete[]pwText;

        return ret;
    }


}
