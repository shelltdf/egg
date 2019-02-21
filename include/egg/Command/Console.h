#ifndef _CLR_CONSOLE_H_
#define _CLR_CONSOLE_H_

#include <egg/Export.h>

#include <vector>
#include <string>
#include <fstream>
#include <map>

#include <egg/Command/Command.h>
#include <egg/Command/Parser.h>
#include <egg/Command/Context.h>


namespace egg
{

    //控制台
    //  Console Parser Context 三者是并列关系
    //  Parser 可能同时存在多个 由主程序负责选择具体使用哪一个
    //  Context 表示的是语言的作用域 也就是实际执行语言的容器
    //      包含所有注册的 Command 对象 Command 本身不保存任何状态信息
    //      包含所有历史记录 历史记录包括 Command 指针 执行参数 返回值
    //      Command 并不包含参数 Parameter 为独立信息
    //      Command 包含自说明信息
    //  Console 模拟一个控制台的逻辑
    //  
    class EGG_RUNTIME_EXPORT Console
    {
    public:
        Console() {}
        ~Console() {}

        //通过继承实现不同的输出形式
        class Output
        {
        public:
            Output() {}
            virtual ~Output() {}

            virtual void info(std::string str) = 0;
            virtual void warning(std::string str) = 0;
            virtual void error(std::string str) = 0;

        private:

        };

        //std::string toString(Parser* parser, Command* command)
        //{
        //    return parser->toString(command);
        //}

        //void runConmmand(Parser* parser, const char* command, int size)
        //{
        //}

        void saveHistroyScriptFile(Parser* parser, Context* context, const char* filename)
        {
            context->exportHistory(parser, filename);
        }
        void saveObjectScriptFile(Parser* parser, Context* context, const char* filename)
        {
            context->exportObject(parser, filename);
        }

        void runScript(Parser* parser, Context* context, const char* script, int size)
        {
            //送入解析器 返回有效的 CommandDataList
            CommandDataList cdl = parser->pushData(script, size);
            for (size_t i = 0; i < cdl.size(); i++)
            {
                //在context中运行脚本
                context->runConmmand(/*context, */cdl[i]);
            }
        }

        void runScriptFile(Parser* parser, Context* context, const char* filename)
        {
            // buffer_size 只是一个递进读取的步长设置 所以是多少都没有关系
            const int buffer_size = 1024;
            //char buffer[buffer_size] = {0};

            std::ifstream ifs(filename, std::ios::binary);
            int length = 0;
            if (ifs.good())
            {
                ifs.seekg(0, ifs.end);
                length = ifs.tellg();
                ifs.seekg(0, ifs.beg);
            }
            while (ifs.good())
            {
                char buffer[buffer_size] = { 0 };

                //这里遇到空文件会无限循环
                //if (ifs.eof()) { break; }

                //读取文件到末尾
                ifs.read(buffer, buffer_size);
                int read_size = buffer_size;
                if (length < buffer_size)
                {
                    read_size = length;
                }
                else
                {
                    length -= buffer_size;
                }

                //渐进式输入内容
                runScript(parser, context, buffer, buffer_size);
            }
            ifs.close();
        }

    };

}//namespace egg

#endif // !_CLR_CONSOLE_H_
