#ifndef NIUBI_FS_ZIPARCHIVE_EXPORT_H
#define NIUBI_FS_ZIPARCHIVE_EXPORT_H

#include <eggEngine/VirtualFileSystem.h>

namespace egg
{

    /*
    * 基于zip的实现
    *   依赖zlib minizip
    */


    class EGG_ENGINE_EXPORT ZipEntry
        :public Entry
    {
    public:
        ZipEntry();
        virtual ~ZipEntry();

        virtual std::string getName();
        virtual bool isFile();
        virtual bool isDir();
        virtual unsigned int getLen();
        virtual void read(char* data);
    };

    class EGG_ENGINE_EXPORT ZipArchive
        :public Archive
    {
    public:
        ZipArchive(egg::niubi_entry_arg& arg);
        virtual ~ZipArchive();

        //virtual std::string getExt();

        virtual void open(const std::string path, int mode = 0);
        virtual void close();
        virtual bool isOpen();

        virtual Entry* getEntry(const std::string path);
        virtual void setEntryAsFile(const std::string path, const char* data, unsigned int len, bool _override);
        virtual bool newEntryAsDir(const std::string path);
        virtual void delEntry(const std::string path);
        virtual std::vector<std::string> listFile(const char* path, bool ergodic = false);
    };

}//namespace nbFS

#endif//NIUBI_FS_ZIPARCHIVE_EXPORT_H

