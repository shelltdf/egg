#ifndef _CLR_Parser_H_
#define _CLR_Parser_H_

#include <egg/Export.h>

//#include <fstream>
//#include <string>
//#include <vector>
//#include <map>
//
//#include <egg/Reflection/ValueHandle.h>
//#include <egg/Reflection/ObjectHandle.h>

#include <egg/Command/Command.h>

namespace egg
{
   

    //命令解析和字符化对象
    //  这里支持一个默认的文本记录格式 以后通过继承来扩展信的方式
    class EGG_RUNTIME_EXPORT Parser
    {
    public:
        Parser() {}
        ~Parser() {}

        /*
        *   文件形式
        *   cmd p0 p1 p2 p3 p4 p5 "a b c d";
        *
        *   空格为分隔符 除了""字符串内
        *   分号为逻辑行结束
        *   # 为物理行注释
        *
        *   cmd 为命令名称
        *   p0-5 是任意类型的参数
        *   遇到 "" 忽略其中的空格
        *
        */

        virtual CommandDataList pushData(const char* data, int size)
        {
            CommandDataList ret;

            //记录到缓冲区
            mBuffer.insert(mBuffer.end(), data, data + size);

            //按照分号拆分
            std::vector < std::string > strlist;
            std::string str;
            int endindex = -1;
            for (size_t i = 0; i < mBuffer.size(); i++)
            {
                char ch = mBuffer[i];
                if (ch == '#')
                {
                    //注释直接到行末尾
                    while ((i < mBuffer.size()) && (mBuffer[i] != '\n'))
                    {
                        i++;
                    }
                    continue;
                }
                else if (ch == ';')
                {
                    if (str.size() > 0)strlist.push_back(str);
                    str.clear();
                    endindex = i;
                }
                else
                {
                    str.push_back(ch);
                    endindex = i;
                }
            }

            //删除有效部分
            std::vector<char> mBuffer2;
            int copysize = mBuffer.size() - 1 - endindex;
            if (copysize > 0)
            {
                mBuffer2.resize(copysize);
                memcpy(&mBuffer2[0], &mBuffer[0], copysize);
                mBuffer = mBuffer2;
            }
            else
            {
                mBuffer.clear();
            }

            //解析脚本
            //CommandDataList cmds;
            bool string_state = false;
            ret.resize(strlist.size());
            for (size_t i = 0; i < strlist.size(); i++)
            {
                //cmds[i].resize(strlist[i].size());
                std::string str;
                for (size_t j = 0; j < strlist[i].size(); j++)
                {
                    char ch = strlist[i][j];
                    //if (ch == '#')
                    //{
                    //    //注释直接到行末尾
                    //    while ((strlist[i][j] != '\n') && (j < strlist[i].size()))
                    //    {
                    //        j++;
                    //    }
                    //    continue;
                    //}
                    //反转字符串状态
                    if (ch == '\"')
                    {
                        string_state = !string_state;
                    }
                    //如果不在字符串内部
                    if (!string_state)
                    {
                        if (ch == ' ')
                        {
                            if (str.size() > 0)ret[i].push_back(str);
                            str.clear();
                        }
                        else
                        {
                            str.push_back(ch);
                        }
                    }
                    else
                    {
                        str.push_back(ch);
                    }
                }//for

                //追加最后一个字符串
                if (str.size() > 0)
                {
                    if (str.size() > 0)ret[i].push_back(str);
                    str.clear();
                }
            }//for

            return ret;
        }

        virtual std::string toData(Command* command, ParameterList pl)
        {
            std::string ret;
            ret += command->getName();
            for (size_t i = 0; i < pl.size(); i++)
            {
                ret += " ";
                ret += pl[i];
                //if (i + 1 < pl.size())ret += " ";
            }
            if (ret.size() > 0)ret += ";\n";
            return ret;
        }

        virtual std::string toData(CommandDataList cdl)
        {
            std::string ret;
            for (size_t i = 0; i < cdl.size(); i++)
            {
                for (size_t j = 0; j < cdl[i].size(); j++)
                {
                    ret += cdl[i][j];
                    if (j + 1 < cdl[i].size())ret += " ";
                }
                if (cdl[i].size() > 0)ret += ";\n";
            }
            return ret;
        }

        void clear()
        {
            mBuffer.clear();
        }

    private:

        std::vector<char> mBuffer;
    };


}//namespace egg

#endif // !_CLR_Parser_H_
