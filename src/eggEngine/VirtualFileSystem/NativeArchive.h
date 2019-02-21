#ifndef NIUBI_FS_NATIVEARCHIVE_EXPORT_H
#define NIUBI_FS_NATIVEARCHIVE_EXPORT_H

#include <eggEngine/VirtualFileSystem.h>

#include <fstream>
#include <ostream>

namespace egg
{

    /*
    * 基于本地目录的实现
    */


    class EGG_ENGINE_EXPORT NativeEntry
        :public Entry
    {
        friend class NativeArchive;
    public:
        NativeEntry();
        virtual ~NativeEntry();

        virtual std::string getName();
        virtual bool isFile();
        virtual bool isDir();
        virtual unsigned int getLen();
        virtual void read(char* data);

    private:

        std::string path;
        std::string dir;
    };

    class EGG_ENGINE_EXPORT NativeArchive
        :public Archive
    {
    public:
        NativeArchive(egg::niubi_entry_arg& arg);
        virtual ~NativeArchive();

        //virtual std::string getExt();

        virtual void open(const std::string path, int mode = 0);
        virtual void close();
        virtual bool isOpen();

        virtual Entry* getEntry(const std::string path);
        virtual void setEntryAsFile(const std::string path, const char* data, unsigned int len, bool _override);
        virtual bool newEntryAsDir(const std::string path);
        virtual void delEntry(const std::string path);
        virtual std::vector<std::string> listFile(const char* path, bool ergodic = false);
    private:

        std::string dir; //打开的路径

        egg::Path getAbsPath( const std::string& path );
    };

}//namespace nbFS

#endif//NIUBI_FS_NATIVEARCHIVE_EXPORT_H
