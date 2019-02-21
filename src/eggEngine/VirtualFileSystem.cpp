

#include <eggEngine/VirtualFileSystem.h>

#include "VirtualFileSystem/NativeArchive.h"
#include "VirtualFileSystem/ZipArchive.h"
// #include <nbFS/SZPArchive.h>

#if ANDROID
#include "VirtualFileSystem/AndroidAssetArchive.h"
#endif

using namespace egg;

IMPLEMENT_SINGLETON(FileSystem)

Archive* FileSystem::newArchive(const std::string ext)
{
    Archive* a = 0;

    //移动设备APP目录
    //  android下是asset manager实现
    //  IOS下是application目录
    //  在PC设备上直接无效
    if (ext == "asset")
    {
#if ANDROID
        a = new AndroidAssetArchive(mArg);
#endif
#if IOS
#endif
    }


    //资源包
    if (ext == "zip")
    {
        //zip
        a = new ZipArchive(mArg);
    }
    if (ext == "szp")
    {
        // szp
        // a = new SZPArchive();
    }

    //默认打开本地路径
    if (a == 0)
    {
        //native
        a = new NativeArchive(mArg);
    }


    if (a)
    {
        mArchiveList.push_back(a);
    }
    return a;
}

Archive* FileSystem::getArchive(unsigned int i)
{
    if (i < mArchiveList.size())
    {
        return mArchiveList[i].get();
    }
    return 0;
}

unsigned int FileSystem::numArchive()
{
    return mArchiveList.size();
}

void FileSystem::delArchive(Archive* archive)
{
    for (unsigned int i = 0; i < mArchiveList.size(); i++)
    {
        if (mArchiveList[i].get() == archive)
        {
            mArchiveList.erase(mArchiveList.begin() + i);
            break;
        }
    }
}

