#ifndef _CLR_Context_H_
#define _CLR_Context_H_

#include <egg/Export.h>

#include <fstream>
//#include <string>
//#include <vector>
#include <map>
//
#include <egg/Reflection/ValueHandle.h>
#include <egg/Reflection/ObjectHandle.h>

//#include <egg/ref_ptr.h>
//#include <egg/Object.h>

#include <egg/Command/Command.h>
#include <egg/Command/Parser.h>
#include <egg/Command/History.h>
#include <egg/Command/ResultStack.h>


namespace egg
{


    //命令执行的环境
    class EGG_RUNTIME_EXPORT Context
    {
    public:
        Context() {}
        virtual ~Context() {}

        void registerDefault();

        void registorConmmand(Command* command)
        {
            mCommand[command->getName()] = command;
        }

        void unregistorConmmand(const char* cmd_name)
        {
            auto it = mCommand.find(cmd_name);
            if (it != mCommand.end())
            {
                mCommand.erase(it);
            }
        }

        Command* getConmmand(const char* cmd_name)
        {
            auto it = mCommand.find(cmd_name);
            if (it != mCommand.end())
            {
                return it->second;
            }
            return 0;
        }

        std::vector<std::string> getConmmandNameList()
        {
            std::vector<std::string> ret;
            for (auto it = mCommand.begin(); it != mCommand.end(); it++)
            {
                ret.push_back(it->first);
            }
            return ret;
        }

        std::vector<std::string> getConmmandNameAndCategoryList()
        {
            std::vector<std::string> ret;
            for (auto it = mCommand.begin(); it != mCommand.end(); it++)
            {
                ret.push_back(it->second->getCategory() + "|" + it->first);
            }
            return ret;
        }


        void runConmmand(/*Context* context, */CommandData data)
        {
            //如果为空
            if (data.size() == 0) return;

            //搜索注册的命令
            Command* cmd = getConmmand(data[0].c_str());
            if (cmd)
            {
                //追加参数
                ParameterList pl;
                for (size_t i = 1; i < data.size(); i++)
                {
                    //cmd->pushParameter(data[i].c_str());
                    pl.push_back(data[i].c_str());
                }

                //执行命令
                std::string ret = cmd->exec(this, pl);

                //保存返回值到历史
                History history;
                history.mCommand = cmd;
                history.mParameter = pl;
                history.mResult = ret;
                this->mHistory.push_back(history);
            }
            else
            {
                //如果没有找到注册的命令
                printf("cmd has no registored : %s\n", data[0].c_str());
            }
        }

        //输出历史脚本
        void exportHistory(Parser* parser, const char* filename)
        {
            //将历史命令全部输出
            std::ofstream ofs(filename, std::ios::binary);
            if (ofs.good())
            {
                ofs << "# histroy script\n";
                for (size_t i = 0; i < this->mHistory.size(); i++)
                {
                    //通过parser转换history成脚本信息
                    std::string str = parser->toData(mHistory[i].mCommand, mHistory[i].mParameter);
                    ofs << str;
                }
            }
        }

        //输出对象脚本
        void exportObject(Parser* parser, const char* filename)
        {
            //把 mGloble 里的对象全部转换成命令输出
            std::ofstream ofs(filename, std::ios::binary);
            if (ofs.good())
            {
                ofs << "# objects script\n";
                for (auto it = mGloble.begin(); it != mGloble.end(); it++)
                {
                    exportObject(parser, it->second.get(), ofs, true);
                }
            }
        }

        ref_ptr<Object> getGloble(std::string name)
        {
            auto it = mGloble.find(name);
            if (it != mGloble.end())
            {
                return it->second;
            }
            return 0;
        }

        void setGloble(std::string name, Object* object)
        {
            mGloble[name] = ref_ptr<Object>(object);
        }

        //结果堆
        ResultStack mResultStack;

    private:

        //注册命令表
        std::map<std::string, Command*> mCommand;

        //历史记录
        std::vector<History> mHistory;

        //顶级对象容器
        std::map< std::string, ref_ptr<Object> > mGloble;


        //递归输出对象
        void exportObject(Parser* parser
            , Object* object
            , std::ofstream& ofs
            , bool root
        )
        {
            //根据对象类型拼装 CommandDataList

            CommandData cd;
            if (root)
            {
                cd.push_back("select");
            }
            else
            {
                cd.push_back("new");
            }
            cd.push_back(ObjectHandle::handle(object)->getClassName());

            CommandDataList cdl;
            cdl.push_back(cd);
            std::string str = parser->toData(cdl);

            ofs << str;

            std::vector<std::string> attrs = ObjectHandle::handle(object)->getAttrNames();
            for (size_t i = 0; i < attrs.size(); i++)
            {
                exportAttr(parser
                    , object, ObjectHandle::handle(object)->getAttr(attrs[i].c_str())
                    , ofs);
            }
        }

        //递归输出属性
        void exportAttr(Parser* parser
            , Object* object, ref_ptr<ValueHandle> attr
            , std::ofstream& ofs)
        {

            if (attr->getValueType() == VHVT_Ref)
            {
                CommandDataList cdl_push;

                CommandData cd_sel;
                cd_sel.push_back("selectattr");
                cd_sel.push_back(attr->getName());
                cdl_push.push_back(cd_sel);

                CommandData cd_push;
                cd_push.push_back("push");
                cdl_push.push_back(cd_push);
                std::string str = parser->toData(cdl_push);
                ofs << str;

                if (attr->getSetType() == VHST_Single)
                {
                    auto subobject = attr->getObject(object);
                    exportObject(parser, subobject, ofs, false);
                    //printf("");

                    CommandDataList cdl_ref;
                    CommandData cd_ref;
                    cd_ref.push_back("setref");
                    //cd_ref.push_back(attr->getName());
                    cdl_ref.push_back(cd_ref);
                    std::string str = parser->toData(cdl_ref);
                    ofs << str;
                }
                if (attr->getSetType() == VHST_Vector)
                {
                    int dim = attr->getDimension(object);
                    for (size_t d = 0; d < dim; d++)
                    {
                        int dim_size = attr->getDimensionSize(object, d);
                        for (size_t i = 0; i < dim_size; i++)
                        {
                            attr->setIndex(object, d, i);
                            ValueHandleValueType attr_value_type = attr->getValueType();
                            Object* attr_object = attr->getObject(object);

                            exportObject(parser, attr_object, ofs, false);

                            CommandDataList cdl_ref;
                            CommandData cd_ref;
                            cd_ref.push_back("addref");
                            cdl_ref.push_back(cd_ref);
                            std::string str = parser->toData(cdl_ref);
                            ofs << str;
                        }//for
                    }//for
                }
                if (attr->getSetType() == VHST_Map)
                {
                    int dim = attr->getDimension(object);
                    for (size_t d = 0; d < dim; d++)
                    {
                        std::vector<std::string> keys = attr->getKeys(object, d);
                        for (size_t i = 0; i < keys.size(); i++)
                        {
                            attr->setKey(object, d, keys[i]);
                            ValueHandleValueType attr_value_type = attr->getValueType();
                            Object* attr_object = attr->getObject(object);

                            exportObject(parser, attr_object, ofs, false);

                            CommandDataList cdl_ref;
                            CommandData cd_ref;
                            cd_ref.push_back("insertref");
                            cd_ref.push_back(attr->getName());
                            cdl_ref.push_back(cd_ref);
                            std::string str = parser->toData(cdl_ref);
                            ofs << str;

                        }//for
                    }//for
                }

                CommandDataList cdl_pop;
                CommandData cd_pop;
                cd_pop.push_back("pop");
                cdl_pop.push_back(cd_pop);
                /*std::string*/ str = parser->toData(cdl_pop);
                ofs << str;
            }
            else
            {
                CommandDataList cdl;

                CommandData cd;
                cd.push_back("setattr");
                cd.push_back(attr->getName());

                std::string str;
                //str += "\"";
                str += attr->getDataString(object);
                //str += "\"";
                cd.push_back(str);

                cdl.push_back(cd);

                /*std::string*/ str = parser->toData(cdl);
                ofs << str;
            }//if

        }

    };

}//namespace egg

#endif // !_CLR_Context_H_
