#ifndef NIUBI_FS_SZPARCHIVE_EXPORT_H
#define NIUBI_FS_SZPARCHIVE_EXPORT_H

#include "FileSystem.h"

#include <sqlite3.h>

namespace egg
{

/*
* 基于sqlite+QuickLZ的实现
*   后缀sqlite
*   依赖sqlite3 QuickLZ
*/


class SZPEntry
    :public Entry
{
public:
    SZPEntry();
    virtual ~SZPEntry();

    virtual std::string getName();
    virtual bool isFile();
    virtual bool isDir();
    virtual unsigned int getLen();
    virtual void read(char* data);
};

class SZPArchive
    :public Archive
{
public:
    SZPArchive();
    virtual ~SZPArchive();

    virtual std::string getExt();

    virtual void open(const std::string path, int mode = 0);
    virtual void close();
    virtual bool isOpen();

    virtual Entry* getEntry(const std::string path);
    virtual void setEntryAsFile(const std::string path, const char* data, unsigned int len);
    virtual bool newEntryAsDir(const std::string path);
    virtual void delEntry(const std::string path);
    virtual std::vector<std::string> listFile(const char* path, bool ergodic = false);
private:
    sqlite3* db;
};

}//namespace egg

#endif//NIUBI_FS_SZPARCHIVE_EXPORT_H

