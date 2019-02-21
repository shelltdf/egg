
#ifndef _NEZHA_CMDsystem_H_
#define _NEZHA_CMDsystem_H_

#include <egg/Command/Command.h>

/*
程序系统级别的命令

version 检查版本兼容
date    设置日期
info    设置文件附加信息

*/

class CMDversion
    :public egg::Command
{
public:
    CMDversion(const char* name = "exit") :egg::Command(name) {}
    virtual ~CMDversion() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        //printf("exit");
        //for (size_t i = 0; i < pl.size(); i++)
        //{
        //    printf(" %s", pl[i].c_str());
        //}
        //printf("\n");

        return "return string";
    }
};

class CMDdate
    :public egg::Command
{
public:
    CMDdate(const char* name = "exit") :egg::Command(name) {}
    virtual ~CMDdate() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        //printf("exit");
        //for (size_t i = 0; i < pl.size(); i++)
        //{
        //    printf(" %s", pl[i].c_str());
        //}
        //printf("\n");

        return "return string";
    }
};

class CMDinfo
    :public egg::Command
{
public:
    CMDinfo(const char* name = "exit") :egg::Command(name) {}
    virtual ~CMDinfo() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        //printf("exit");
        //for (size_t i = 0; i < pl.size(); i++)
        //{
        //    printf(" %s", pl[i].c_str());
        //}
        //printf("\n");

        return "return string";
    }
};


#endif //_NEZHA_CMDsystem_H_
