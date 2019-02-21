
#ifndef _NEZHA_CMDattr_H_
#define _NEZHA_CMDattr_H_

#include <egg/Command/Command.h>
#include <egg/Command/Context.h>
//#include <Runtime/Runtime.h>

/*
以后移动到 CLR 中

//针对当前object的的属性操作
typeattr 获取属性类型
setattr  设置属性值
getattr  查询属性值
addattr  追加扩展属性元素（有必要吗）
delattr  删除扩展属性元素（有必要吗）

//属性堆栈
selectattr 选择当前对象的属性到缓冲区

//ref操作
setref      设置堆栈top到当前属性
addref      追加堆栈top到当前属性
insertref   追加堆栈top到当前属性

*/

class CMDsetattr
    :public egg::Command
{
public:
    CMDsetattr(const char* name = "setattr") :egg::Command(name) {}
    virtual ~CMDsetattr() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("setattr");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        //参数检查
        if (pl.size() < 2) return "";

        //get当前对象
        //Runtime* runtime = Runtime::getInstance();
        auto obj = context->mResultStack.getCurrent();
        if (obj == 0) return "";

        //设置属性
        egg::ObjectHandle::handle(obj)->getAttr(pl[0].c_str())->setDataString(obj, pl[1].c_str());

        return "";
    }
};

class CMDselectattr
    :public egg::Command
{
public:
    CMDselectattr(const char* name = "selectattr") :egg::Command(name) {}
    virtual ~CMDselectattr() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("selectattr");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        //参数检查
        if (pl.size() < 1) return "";

        //get当前对象
        //Runtime* runtime = Runtime::getInstance();
        auto obj = context->mResultStack.getCurrent();
        if (obj == 0) return "";

        //设置当前属性
        //Runtime* runtime = Runtime::getInstance();
        context->mResultStack.setCurrentAttr(obj, pl[0].c_str());

        return "";
    }
};


class CMDsetref
    :public egg::Command
{
public:
    CMDsetref(const char* name = "setref") :egg::Command(name) {}
    virtual ~CMDsetref() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("setref");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        //设置当前属性
        //Runtime* runtime = Runtime::getInstance();
        auto obj = context->mResultStack.getCurrentAttrObject();
        auto attrname = context->mResultStack.getSelectAttrName();
        if (obj == 0) return "";

        //当前对象
        auto obj_cur = context->mResultStack.getCurrent();
        if (obj_cur == 0) return "";

        //设置属性ref object
        egg::ObjectHandle::handle(obj)->getAttr(attrname.c_str())->setObject(obj, obj_cur);

        return "";
    }
};

class CMDaddref
    :public egg::Command
{
public:
    CMDaddref(const char* name = "addref") :egg::Command(name) {}
    virtual ~CMDaddref() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("addref");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        //设置当前属性
        //Runtime* runtime = Runtime::getInstance();
        auto obj = context->mResultStack.getCurrentAttrObject();
        auto attrname = context->mResultStack.getSelectAttrName();
        if (obj == 0) return "";

        //当前对象
        auto obj_cur = context->mResultStack.getCurrent();
        if (obj_cur == 0) return "";

        //设置属性ref object
        egg::ObjectHandle::handle(obj)->getAttr(attrname.c_str())->pushObject(obj, 0, obj_cur);

        return "";
    }
};

class CMDinsertref
    :public egg::Command
{
public:
    CMDinsertref(const char* name = "insertref") :egg::Command(name) {}
    virtual ~CMDinsertref() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("insertref");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        //参数检查
        if (pl.size() < 1) return "";

        //设置当前属性
        //Runtime* runtime = Runtime::getInstance();
        auto obj = context->mResultStack.getCurrentAttrObject();
        auto attrname = context->mResultStack.getSelectAttrName();
        if (obj == 0) return "";

        //当前对象
        auto obj_cur = context->mResultStack.getCurrent();
        if (obj_cur == 0) return "";

        //设置属性ref object
        egg::ObjectHandle::handle(obj)->getAttr(attrname.c_str())->insertObject(obj, 0, pl[0].c_str(), obj_cur);

        return "";
    }
};


#endif //_NEZHA_CMDattr_H_
