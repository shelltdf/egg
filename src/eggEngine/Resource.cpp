
#include <eggEngine/Resource.h>

using namespace egg;


void Resource::save(egg::Path filename , std::ostream& os , Serializer* ser ) const
{
    Object::save(filename,os,ser);

    //资源路径只保留资源名 没有后缀
    std::string name = uniform_path.getLastName();
    ser->writeString( os , name );
}

void Resource::load(egg::Path filename , std::istream& is , Serializer* ser 
    , ObjectManager* om, egg::Referenced* rmg, bool load_ext)
{
    Object::load(filename,is,ser,om,rmg,load_ext);

    //当前文件目录+记录名称=资源路径 没有后缀
    std::string name;
    ser->readString( is , name );
    uniform_path.set(filename.getPathMel(true,true).c_str()); //当前文件目录
    uniform_path.push(name.c_str());
}

//设置统一路径 如果结尾不是class后缀就补充上去
void Resource::setUniformPath(const std::string& path)
{
    setUniformPath(egg::Path(path));
}

void Resource::setUniformPath(egg::Path path)
{
    //复制路径
    uniform_path = path;

    //检查末尾是否有正确的后缀
    std::vector< std::string > sl = path.splitLastName();
    std::string lastname = path.getLastName();
    if (sl.size() <= 1)  //无后缀 避免文件名称跟后缀相同的情况
    {
        uniform_path.pop();
        uniform_path.push((lastname + "." + this->ObjectClassName()).c_str());
    }
    else //文件名称有后缀
    {
        if ((*sl.rbegin()) != this->ObjectClassName()) //末尾字符不是class后缀
        {
            uniform_path.pop();
            uniform_path.push((lastname + "." + this->ObjectClassName()).c_str());
        }
    }
}