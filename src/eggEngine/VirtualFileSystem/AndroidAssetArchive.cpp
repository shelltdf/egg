
#include "AndroidAssetArchive.h"

#include <dirent.h>

using namespace egg;

AndroidAssetEntry::AndroidAssetEntry()
    :mAsset(0)
{
}

AndroidAssetEntry::~AndroidAssetEntry()
{
    if(mAsset)
    {
        AAsset_close(mAsset);
        mAsset = 0;
    }
}

std::string AndroidAssetEntry::getName()
{
    return mName;
}

bool AndroidAssetEntry::isFile()
{
    return mAsset != 0;
}

bool AndroidAssetEntry::isDir()
{
    return false;
}

unsigned int AndroidAssetEntry::getLen()
{
    return AAsset_getLength(mAsset);
}

void AndroidAssetEntry::read(char* data)
{
    //data = (char*)AAsset_getBuffer(mAsset);
    memcpy( data , (char*)AAsset_getBuffer(mAsset) , AAsset_getLength(mAsset) );
}




AndroidAssetArchive::AndroidAssetArchive(nbBase::niubi_entry_arg& arg)
    :Archive(arg)
    ,mAssetManager(0)
{
}

AndroidAssetArchive::~AndroidAssetArchive()
{
}

// std::string AndroidAssetArchive::getExt()
// {
    // return "";
// }

int getdir (std::string dir, std::vector<std::string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) 
    {
        //std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) 
    {
        files.push_back(std::string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

void AndroidAssetArchive::open(const std::string path, int mode)
{
    ANativeActivity* nativeActivity = mArg.state->activity;
	mAssetManager = nativeActivity->assetManager;

    std::string ip = nativeActivity->internalDataPath;
    std::string ep = nativeActivity->externalDataPath;

    std::vector<std::string> files = std::vector<std::string>();
    getdir("/data/app",files);

    printf("aaa");
}

void AndroidAssetArchive::close()
{
    mAssetManager = 0;
}

bool AndroidAssetArchive::isOpen()
{
    return mAssetManager != 0;
}

Entry* AndroidAssetArchive::getEntry(const std::string path)
{

    //AAssetManager_open 输入的路径必须是  list.txt 这样前面没有任何 .和/的路径

    //修正assets路径
    nbBase::Path p(path);
    std::string path_str = p.getNativePath();
    while ( path_str[0] == '.' || path_str[0] == '\\' || path_str[0] == '/')
    {
        path_str = &path_str[1];
    }

    //open file
    AAsset* file = AAssetManager_open(mAssetManager, path_str.c_str() , AASSET_MODE_BUFFER);
	if(file == 0)
	{
        //如果失败 就可以试图打开目录
        
        
        
        
        //如果仍然失败
		LOGW("getEntry failed: %d %d %s",(unsigned int)mAssetManager,(unsigned int)file, path.c_str());
        
	}
	else
	{
        LOGW("getEntry successful: %d %d %s",(unsigned int)mAssetManager,(unsigned int)file, path.c_str());
        
        //如果存在
        AndroidAssetEntry* ent = new AndroidAssetEntry();

        //查询文件状态和属性
        ent->mAsset = file;
        ent->mName = nbBase::Path(path.c_str()).getLastName();
        
        //retrue file object
        return ent;
    }
    
    return 0;
}

void AndroidAssetArchive::setEntryAsFile(const std::string path, const char* data, unsigned int len, bool _override)
{
    //只读模式  无效操作
}

bool AndroidAssetArchive::newEntryAsDir(const std::string path)
{
    //只读模式  无效操作
    return false;
}

void AndroidAssetArchive::delEntry(const std::string path)
{
    //只读模式  无效操作
}

std::vector<std::string> AndroidAssetArchive::listFile(const char* path, bool ergodic)
{
    std::vector<std::string> ret;

#if 0
    //打开目录
    std::string str_path = nbBase::Path(path).getNativePath();
    AAssetDir* assetDir = AAssetManager_openDir(mAssetManager, str_path.c_str());

    //下一个文件名
    const char* filename = (const char*)NULL;
    while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL) 
    {
        //尝试使用文件方式打开
        //AAsset* asset = AAssetManager_open(mAssetManager, filename, AASSET_MODE_BUFFER);
        //if(asset)
        //{
        //    AAsset_close(asset);

            nbBase::Path p(path);
            p.push(filename);
            ret.push_back(p.getPathMel());
        //}
        //else
        //{
        //}
    }
    AAssetDir_close(assetDir);
#endif

    //直接读assets/list.txt
    Entry* ent_list = getEntry("list.txt");
    std::string buffer_list;
    buffer_list.resize(ent_list->getLen());
    ent_list->read(&buffer_list[0]);

    //切分成列表
    std::vector<std::string> list;
    std::string filename;
    for (int i = 0; i < buffer_list.size(); i++)
    {
        if(buffer_list[i] == '\n' || buffer_list[i] == '\r')
        {
            if(filename.size()>0)
            {
                list.push_back(filename);
                filename = "";
            }
        }
        else
        {
            filename.push_back(buffer_list[i]);
        }
    }
    if(filename.size()>0)
    {
        list.push_back(filename);
        filename = "";
    }

#if 0
    //循环整个列表
    for (int i = 0; i < list.size(); i++)
    {
        //如果列表中的目录在path之内就通过  如果不在就跳过

        //生成路径
        nbBase::Path p("/data/");
        //p.push(list[i]);

        //打开目录
        std::string str_path = p.getNativePath();
        //AAssetDir* assetDir0 = AAssetManager_openDir(mAssetManager, "/");
        //AAssetDir* assetDir = AAssetManager_openDir(mAssetManager, str_path.c_str());
        AAssetDir* assetDir = AAssetManager_openDir(mAssetManager, "/");

        //下一个文件名
        const char* filename = (const char*)NULL;
        while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL) 
        {
            nbBase::Path p(list[i]);
            p.push(filename);
            ret.push_back(p.getPathMel());
        }
        AAssetDir_close(assetDir);
    }
#endif

    //循环整个列表
    for (int i = 0; i < list.size(); i++)
    {
        nbBase::Path p("/");
        p.push(list[i]);
        ret.push_back(p.getPathMel());
    }

    return ret;
}