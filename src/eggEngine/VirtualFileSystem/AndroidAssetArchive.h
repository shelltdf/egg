#ifndef NIUBI_FS_ANDROIDASSETARCHIVE_EXPORT_H
#define NIUBI_FS_ANDROIDASSETARCHIVE_EXPORT_H

#include "FileSystem.h"

#include <android/asset_manager.h>

namespace egg
{

/*
* 基于Android Asset Manager的实现
*   只读模式
*/


class AndroidAssetEntry
    :public Entry
{
    friend class AndroidAssetArchive;
public:
    AndroidAssetEntry();
    virtual ~AndroidAssetEntry();

    virtual std::string getName();
    virtual bool isFile();
    virtual bool isDir();
    virtual unsigned int getLen();
    virtual void read(char* data);
    
 private:   
    AAsset* mAsset;
    std::string mName;
};

class AndroidAssetArchive
    :public Archive
{
public:
    AndroidAssetArchive(nbBase::niubi_entry_arg& arg);
    virtual ~AndroidAssetArchive();

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
    AAssetManager*   mAssetManager;
};

}//namespace egg

#endif//NIUBI_FS_ANDROIDASSETARCHIVE_EXPORT_H

