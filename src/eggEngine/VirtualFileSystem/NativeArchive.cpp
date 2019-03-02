
#include "NativeArchive.h"

#include <egg/Path.h>
#include <egg/Platform.h>


using namespace egg;


NativeEntry::NativeEntry()
{
}

NativeEntry::~NativeEntry()
{
}

std::string NativeEntry::getName()
{
    egg::Path p(path.c_str());
    return p.getLastName();
}

bool NativeEntry::isFile()
{
    egg::Path p(dir.c_str());
    p.push(path.c_str());
    if (Platform::getInstance()->isExist(p.getNativePath().c_str()) == 1) return true;
    return false;
}

bool NativeEntry::isDir()
{
    egg::Path p(dir.c_str());
    p.push(path.c_str());
    if (Platform::getInstance()->isExist(p.getNativePath().c_str()) == 2) return true;
    return false;
}

unsigned int NativeEntry::getLen()
{
    egg::Path p(dir.c_str());
    p.push(path.c_str());
    return Platform::getInstance()->Length(p.getNativePath().c_str());
}

void NativeEntry::read(char* data)
{
    egg::Path p(dir.c_str());
    p.push(path.c_str());

    //打开文件 把文件对象传递给entry
    std::ifstream ifs(p.getNativePath(), std::ios::binary | std::ios::in);
    if (ifs.good())
    {
        ifs.read(data, getLen());
    }
    ifs.close();
}


NativeArchive::NativeArchive(egg::niubi_entry_arg& arg)
    :Archive(arg)
    , dir("")
{
}

NativeArchive::~NativeArchive()
{
}

//std::string NativeArchive::getExt()
//{
//    return "";
//}

void NativeArchive::open(const std::string path, int mode)
{
    //验证路径是否存在
    if (Platform::getInstance()->isExist(path.c_str()) != 2)
    {
        //如果不是目录就是错误
        if(mode == 0)return;

        //创建路径
        Platform::getInstance()->createDirectory( path.c_str() );
    }
    dir = egg::Path(path).getNativePath();
}

void NativeArchive::close()
{
    dir = "";
}

bool NativeArchive::isOpen()
{
    return !dir.empty();
}

Entry* NativeArchive::getEntry(const std::string path)
{
    //验证路径是否存在
    egg::Path p = getAbsPath( path );
    int state = Platform::getInstance()->isExist(p.getNativePath().c_str());
    if (state == 0)
    {
        return 0;
    }

    //如果存在
    NativeEntry* ent = new NativeEntry();

    //查询文件状态和属性
    ent->dir = dir; //挂载路径
    ent->path = path;    //内路径

    return ent;
}

void NativeArchive::setEntryAsFile(const std::string path, const char* data, unsigned int len, bool _override)
{
    //查询文件是否存在 如果不存在 就创建 如果存在就覆盖

    egg::Path p2 = path;
    if( p2.size() > 1 )//如果路径长度超过1 就说明有子目录需求
    {
        p2.pop(); //得到目录

        //目录不存在创建之
        Platform::getInstance()->createDirectory(getAbsPath( p2.getPathMel() ).getNativePath().c_str());
    }

    //完整路径
    egg::Path p = getAbsPath( path );

    //文件不存在
    if (Platform::getInstance()->isExist(p.getNativePath().c_str()) == 0
        || _override )
    {
        //打开文件 把文件对象传递给entry
        std::ofstream ofs;
        ofs.open(p.getNativePath(), std::ofstream::out|std::ios::binary);
        if (ofs.good())
        {
            ofs.write(data, len);
        }
        ofs.flush(); //必须强制完成缓冲区 不然立即读这个文件会出错
        ofs.close();
    }
}

bool NativeArchive::newEntryAsDir(const std::string path)
{
    //尝试建立一个目录
    egg::Path p = getAbsPath( path );

    //目录已经存在 直接返回
    if (Platform::getInstance()->isExist(p.getNativePath().c_str()) == 2) return true;

    //目录不存在创建之
    Platform::getInstance()->createDirectory(p.getNativePath().c_str());

    return true;
}

void NativeArchive::delEntry(const std::string path)
{
    //删除目录或者文件对象
    egg::Path p = getAbsPath( path );
    Platform::getInstance()->removeFile(p.getNativePath().c_str());
}


std::vector<std::string> NativeArchive::listFile(const char* path, bool ergodic)
{
    //如果内路径是个绝对路径就转换成相对路径
    egg::Path rp(path);
    rp.setAbsolutePath(false);
    std::string str = rp.getPathMel();
    //if (rp.isAbsolutePath())
    //{
    //    str = "." + rp.getPathMel();
    //}
    //else
    //{
    //    str = rp.getPathMel();
    //}

    std::vector<std::string> l = Platform::getInstance()->listFile(dir.c_str(), str.c_str(), ergodic);
    std::vector<std::string> ret;

    //把本地路径转化成内路径形式
    for (unsigned int i = 0; i < l.size(); i++)
    {
        egg::Path p(l[i].c_str());
        ret.push_back(p.getPathMel(false, true));
    }

    return ret;
}


//输包内绝对路径  得到本地系统的绝对路径
egg::Path NativeArchive::getAbsPath( const std::string& path )
{
    //完整路径
    egg::Path p( dir ); //当前目录
    egg::Path p2(path);         //输入路径
    p2.setAbsolutePath(false);     //强制转换成相对路径 为了下面路径合并的正确
    p.push( p2 );

    return p;
}
