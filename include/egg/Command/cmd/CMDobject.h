
#ifndef _NEZHA_CMDobject_H_
#define _NEZHA_CMDobject_H_

#include <egg/Command/Command.h>
#include <egg/Command/Context.h>
//#include <Runtime/Runtime.h>

/*
以后移动到 CLR 中

//建立删除
new 建立对象
del 删除对象

//对象堆栈操作
select    选择当前对象 通过全局名称
push      当前名称入栈
pop       出栈名称

//针对当前堆栈的操作
rename    重命名

*/

class CMDnew
    :public egg::Command
{
public:
    CMDnew(const char* name = "new") :egg::Command(name) {}
    virtual ~CMDnew() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("new");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        //参数检查
        if (pl.size() < 1) return "";

        //建立对象
        egg::ObjectManager* om = egg::ObjectManager::getInstance();
        egg::Object* obj = om->newObject(pl[0].c_str());

        //设置名称
        //Runtime* runtime = Runtime::getInstance();
        //std::string name = CLR::UniformNamingObject::getInstance()->getUniformName(pl[0]);
        //egg::ObjectHandle::handle(obj)->getAttr("name")->setDataString(obj, pl[0]);
        obj->setName(pl[0]);

        //设置为当前选择对象
        //Runtime* runtime = Runtime::getInstance();
        context->mResultStack.setCurrent(obj);

        //返回名称
        return obj->getName();
    }
};

class CMDdel
    :public egg::Command
{
public:
    CMDdel(const char* name = "del") :egg::Command(name) {}
    virtual ~CMDdel() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("del");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");


        //返回名称
        return "";
    }
};


class CMDselect
    :public egg::Command
{
public:
    CMDselect(const char* name = "select") :egg::Command(name) {}
    virtual ~CMDselect() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("select");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        //参数检查
        if (pl.size() < 1) return "";

        //搜索对象
        //Runtime* runtime = Runtime::getInstance();
        auto it = context->getGloble(pl[0].c_str());
        if (!it.valid()) return "";

        //设置为当前选择对象 使用全局路径
        context->mResultStack.setCurrent(it.get());

        //返回名称
        return "";
    }
};


class CMDpush
    :public egg::Command
{
public:
    CMDpush(const char* name = "push") :egg::Command(name) {}
    virtual ~CMDpush() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("push");
        printf("\n");

        //入栈
        //Runtime* runtime = Runtime::getInstance();
        context->mResultStack.push();

        //返回名称
        return "";
    }
};

class CMDpop
    :public egg::Command
{
public:
    CMDpop(const char* name = "pop") :egg::Command(name) {}
    virtual ~CMDpop() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("pop");
        printf("\n");

        //出栈
        //Runtime* runtime = Runtime::getInstance();
        context->mResultStack.pop();

        //返回名称
        return "";
    }
};


class CMDrename
    :public egg::Command
{
public:
    CMDrename(const char* name = "rename") :egg::Command(name) {}
    virtual ~CMDrename() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("rename");
        printf("\n");


        //返回名称
        return "";
    }
};

#endif //_NEZHA_CMDobject_H_
