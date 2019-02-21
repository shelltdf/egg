#ifndef NIUBI_FS_FILESYSTEM_EXPORT_H
#define NIUBI_FS_FILESYSTEM_EXPORT_H

//#include <nbBase/Public.h>
#include <eggEngine/Export.h>

#include <eggRuntime/Log.h>
#include <eggRuntime/SystemArg.h>
#include <eggRuntime/Path.h>
#include <eggRuntime/ref_ptr.h>
#include <eggRuntime/Singleton.h>

#include <string>
#include <vector>


/*
*
*本地路径
*    就是本地操作系统的路径方式 windows c:\aaaa\bbb  linux /usr/local
*内路径
*    类似mel的形式  |aaa|bbb
*
*编码方式
*    目前使用本地ANSI编码作为默认支持 等string的方案确定了再说
*
*/

namespace egg
{
    ///文件对象
    ///
    ///包中的一个文件或者目录
    class EGG_ENGINE_EXPORT Entry
        :public Referenced
    {
    public:
        Entry(){}
        virtual ~Entry(){}

        virtual std::string getName() = 0;
        virtual bool isFile() = 0;
        virtual bool isDir() = 0;
        virtual unsigned int getLen() = 0;
        virtual void read(char* data) = 0;
    };

    ///包对象
    ///
    ///比如:本地目录,zip包,sqlite+QuickLZ
    class EGG_ENGINE_EXPORT Archive
        :public Referenced
    {
    public:
        Archive(egg::niubi_entry_arg& arg)
            :mArg(arg){}
        virtual ~Archive(){}

        egg::niubi_entry_arg mArg;//系统入口参数对象

        //virtual std::string getExt() = 0; //支持的后缀名称

        //打开包
        //   path 本地文件路径        asset类型时此参数无效
        //   mode 0只读 1覆盖 2编辑   asset类型时此参数只能为只读
        virtual void open(const std::string path, int mode = 0) = 0;

        //关闭包
        virtual void close() = 0;

        //查询是否打开
        virtual bool isOpen() = 0;


        //获取对象
        //  path 内路径
        virtual Entry* getEntry(const std::string path) = 0;

        //覆盖对象
        //  path 内路径
        //  data 写入数据
        //  len 写入字节长度
        virtual void setEntryAsFile(const std::string path, const char* data, unsigned int len , bool _override) = 0;

        //新建目录
        //  path 内路径
        virtual bool newEntryAsDir(const std::string path) = 0;

        //删除对象
        //  path 内路径
        virtual void delEntry(const std::string path) = 0;

        //列举包内路径
        //  path 内路径
        //  ergodic 递归子目录
        virtual std::vector<std::string> listFile(const char* path, bool ergodic = false) = 0;

    };

    ///文件系统
    ///
    ///  使用统一路径来管理所有硬盘上的文件\n
    ///  支持多个包的同名覆盖策略\n
    ///  支持对比包差异生成增量更新包\n
    ///  以后可以考虑升级管理远程文件\n
    class EGG_ENGINE_EXPORT FileSystem
        :public Referenced
    {
    public:
        FileSystem(){}
        ~FileSystem(){}

        DECLARE_SINGLETON(FileSystem)

        void setArg(egg::niubi_entry_arg& arg){ mArg = arg; }

        //建立包 根据路径后缀判断
        //  参数为"asset"时在PC模式下只能返回本地路径 如果为android得到assetmanager路径 如果为IOS得到application路径
        Archive* newArchive(const std::string ext);

        //获取包
        Archive* getArchive(unsigned int i);

        //当前包数量
        unsigned int numArchive();

        //删除包
        void delArchive(Archive* archive);

    private:

        std::vector< ref_ptr<Archive> > mArchiveList;

        egg::niubi_entry_arg mArg;//系统入口参数对象
    };

}//namespace nbFS

#endif//NIUBI_FS_FILESYSTEM_EXPORT_H
