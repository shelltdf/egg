
#include <egg/Reflection/ObjectHandle.h>
#include <egg/Command/Console.h>
#include <string>

//#include <Runtime/Runtime.h>

//针对runtime对象的测试
#define FILENAME_HISTORY "testScriptHistory.nezha"
#define FILENAME_OBJECTS "testScriptObjects.nezha"

/*
* 命令行分两种形式
*   1 对象和属性记录 也就是存储用的形式
*   2 完整格式 记录历史 或者直接运行具体命令
*   记录形式只有有限的命令组成，并不记录高级命令。
*   因为整个系统是反射的。
*   记录形式只有对象操作和属性操作。还有版本信息。
*/

int main(int argc, char *argv[])
{
    //Runtime* runtime = Runtime::getInstance();
    egg::Context* context = new egg::Context();
    egg::Console* console = new egg::Console();
    egg::Parser* parser = new egg::Parser();

    //获取当前空项目
    auto project = context->getGloble("project");

    //通过命令行编辑project内容
    std::string script = {
        //"test 111 222 333;"
        //"test aaa bbb ccc;"
        //" test  AAA  BBB  CCC ; "

        //所有名称都不需要引号 名称也不能有空格
        //只有属性内容有引号

        "select project;"
        "setattr name \"狗狗大作战\";"

        "selattr actor;"
        "push;"

        "newobject actor;"
        "addref;"

        "pop;"
    };

    console->runScript(
        parser, context, script.c_str(), script.size());

    //保存命令历史记录到文本
    console->saveHistroyScriptFile(
        parser, context, FILENAME_HISTORY);

    //保存项目到文件 遍历反射对象树 生成对应命令行
    console->saveObjectScriptFile(
        parser, context, FILENAME_OBJECTS);

#if 0
    //新建项目
    runtime->mContext->setGloble("project", new Project());

    //打开项目文件 直接运行脚本文件
    runtime->mConsole->runScriptFile(runtime->mParser, runtime->mContext, FILENAME_HISTORY);



    //新建项目
    runtime->mContext->setGloble("project", new Project());

    //打开项目文件 直接运行脚本文件
    runtime->mConsole->runScriptFile(runtime->mParser, runtime->mContext, FILENAME_OBJECTS);
#endif

}
