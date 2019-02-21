
#include <egg/Reflection/ObjectHandle.h>
#include <egg/Command/Console.h>
#include <string>


class CMD_test
    :public egg::Command
{
public:
    CMD_test(const char* name = "test") :egg::Command(name) {}
    virtual ~CMD_test() {}
    virtual std::string exec(egg::Context* context, egg::ParameterList pl)
    {
        printf("test");
        for (size_t i = 0; i < pl.size(); i++)
        {
            printf(" %s", pl[i].c_str());
        }
        printf("\n");

        return "return string";
    }
};



void test_cmd()
{
    //测试命令

    //执行命令需要有一个context容器
    egg::Context* context = new egg::Context();

    //注册命令对象
    context->registorConmmand(new CMD_test());

    //执行命令 这里可以简化输入
    egg::CommandData cmdd;
    cmdd.push_back("test");
    cmdd.push_back("p0");
    cmdd.push_back("p1");
    cmdd.push_back("p2");
    cmdd.push_back("p3");
    context->runConmmand(cmdd);

}


void test_parser()
{
    //执行命令需要有一个context容器
    egg::Context* context = new egg::Context();

    //注册命令对象
    context->registorConmmand(new CMD_test());

    //命令解析器
    egg::Parser* parser = new egg::Parser();

    std::string cmd;

    //测试正常命令
    cmd = "test q0 q1 q2 q3;test q5;";
    egg::CommandDataList cmds0 = parser->pushData(cmd.c_str(), cmd.size());
    for (size_t i = 0; i < cmds0.size(); i++)
    {
        context->runConmmand(cmds0[i]);
    }

    //测试注释
    cmd = "#test x0 x1 x2 x3;\ntest x5;";
    egg::CommandDataList cmds = parser->pushData(cmd.c_str(), cmd.size());
    for (size_t i = 0; i < cmds.size(); i++)
    {
        context->runConmmand(cmds[i]);
    }

    //测试连续输入
    std::vector<std::string> cmdlist;
    cmdlist.push_back("test ");
    cmdlist.push_back("ww ");
    cmdlist.push_back("; ");
    for (size_t i = 0; i < cmdlist.size(); i++)
    {
        egg::CommandDataList cmds = parser->pushData(cmdlist[i].c_str(), cmdlist[i].size());

        for (size_t i = 0; i < cmds.size(); i++)
        {
            context->runConmmand(cmds[i]);
        }
    }

}

void test_console()
{
    //测试控制台
    egg::Context* context = new egg::Context();
    context->registorConmmand(new CMD_test());
    egg::Parser* parser = new egg::Parser();
    egg::Console* console = new egg::Console();


    //测试内存脚本
    std::string script = {
        "test 111 222 333;"
        "test aaa bbb ccc;"
        " test  AAA  BBB  CCC ; "
    };
    console->runScript(parser, context, script.c_str(), script.size());


    //写入测试文件
    std::string filename = "testCLRScript.txt";
    std::ofstream ofs(filename);
    ofs.write(script.c_str(), script.size());
    ofs.close();

    //测试文件脚本
    console->runScriptFile(parser, context, filename.c_str());


}


int main(int argc, char *argv[])
{
    test_cmd();
    test_parser();
    test_console();

}

