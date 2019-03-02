
#include <eggEngine/ResourceManager.h>

#include <egg/Platform.h>

#if 1

//#include <nbSG/Node.h>
#include <eggEngine/SceneNode.h>

#include <eggEngine/Geometry.h>
#include <eggEngine/Image.h>
#include <eggEngine/Material.h>
#include <eggEngine/Program.h>
#include <eggEngine/Scene.h>
#include <eggEngine/Shader.h>
#include <eggEngine/Texture.h>
#include <eggEngine/Animation.h>

#include <eggEngine/Components/MeshRender.h>
#include <eggEngine/Components/ActorControler.h>
//#include <eggEngine/Components/SkeletonActorControler.h>
#include <eggEngine/Components/Transfrom3D.h>
#include <eggEngine/Components/Joint3D.h>
#include <eggEngine/Components/Light.h>
#include <eggEngine/Components/Camera.h>

#endif 

using namespace egg;

IMPLEMENT_SINGLETON(ResourceManager)

ResourceGroup::ResourceGroup(Resource* resource)
{
    add(resource);
}

void ResourceGroup::add(Resource* resource)
{
    if (resource)
    {
        //保存到当前列表
        mResourceObjectMap[resource->getUniformPath()] = resource;

        //mResourceObjectMap.clear();
        mResourceLinkList.clear(); //清空空连接表 重新生成

        for (auto it = mResourceObjectMap.begin(); it != mResourceObjectMap.end(); it++)
        {
            it->second->collectionResourceObject(mResourceObjectMap);
            it->second->collectionResourceLink(mResourceLinkList);
        }
    }
}

//遍历所有可能存在的资源
void ResourceGroup::searchAll()
{
    unsigned int size = 0;
    unsigned int size2 = 0;
    do
    {
        size = mResourceObjectMap.size() + mResourceLinkList.size();
        for (auto it = mResourceObjectMap.begin(); it != mResourceObjectMap.end(); it++)
        {
            it->second->collectionResourceObject(mResourceObjectMap);
            it->second->collectionResourceLink(mResourceLinkList);
        }
        size2 = mResourceObjectMap.size() + mResourceLinkList.size();
    } while (size != size2);
}

void ResourceGroup::cleanupLink()
{
    std::vector< egg::ref_ptr< ResourceLink > > links;
    for (unsigned int i = 0; i < mResourceLinkList.size(); i++)
    {
        if (!mResourceLinkList[i]->isLoaded())
        {
            links.push_back(mResourceLinkList[i]);
        }
    }
    mResourceLinkList = links;
}

void ResourceGroup::clear()
{
    mResourceObjectMap.clear();
    mResourceLinkList.clear();
}


ResourceManager::ResourceManager()
    :mWriteArchiveIndex(0)
    , use_binary(true)
{
}

ResourceManager::~ResourceManager()
{
}

#define ROC(CLASS) ObjectManager::getInstance()->registerObjectClass<CLASS>(#CLASS)

void ResourceManager::init(egg::niubi_entry_arg& arg, bool binary)
{
    mWriteArchiveIndex = 0;
    use_binary = binary;

    if (use_binary)
    {
        ser = new egg::Serializer_Binary();
    }
    else
    {
        ser = new egg::Serializer_Text();
    }


    mResourceGroupMap[NB_RESOURCEGROUP_DEFAULT_NAME] = ResourceGroup();
    //mFileSystem = new nbFS::FileSystem ();
    //mFileSystem.setArg(arg);

    //注册默认的对象管理器 并且初始化默认类型
    //mObjectManager = new ObjectManager();
    //mObjectManager->registerDefaultObjectClass();

#if 1
    //资源相关
    ROC(Geometry);
    ROC(Image);
    ROC(Material);
    ROC(Program);
    ROC(Scene);
    ROC(Shader);
    ROC(Texture);
    ROC(Animation);

    //场景相关
    ROC(Node);
    ROC(SceneNode);

    //行为组件
    ROC(Transfrom3D);
    ROC(Joint3D);
    ROC(MeshRender);
    ROC(ActorControler);
    //ROC(SkeletonActorControler);
    ROC(Light);
    ROC(Camera);
    //ROC(Line);

#endif
}

void ResourceManager::registerArchive(const std::string& type, const std::string& path, bool write)
{
    //注册一个包
    egg::Archive* archive = FileSystem::getInstance()->newArchive(type);

    //打开包
    int mode = 0;
    if (write) mode = 1;
    archive->open(Platform::getInstance()->getApplicationDir() + "/" + path, mode);
    if (archive->isOpen())
    {
        LOGI("archive is open");
        updateUniformAssetPath();
    }
    else
    {
        LOGI("archive is close");
    }

}

void ResourceManager::unRegisterArchive(const std::string& path)
{
}

void ResourceManager::updateUniformAssetPath()
{
    for (unsigned int i = 0; i < FileSystem::getInstance()->numArchive(); i++)
    {
        egg::Archive* archive = FileSystem::getInstance()->getArchive(i);
        std::vector<std::string> ls = archive->listFile("|", true);
        for (unsigned int j = 0; j < ls.size(); j++)
        {
            auto it = mUniformAssetFileMap.find(egg::Path(ls[j]));
            if (it != mUniformAssetFileMap.end())
            {
                it->second.archive_index = i;
                it->second.repeat_count++;
                //it->second.type !=
                //这里如何处理文件类型
            }
            else
            {
                UniformAssetFile uf;
                uf.archive_index = i;
                uf.repeat_count = 0;
                uf.uniform_path = ls[j];
                //uf.type = ??
                mUniformAssetFileMap[uf.uniform_path] = uf;
            }
        }
    }
}

void ResourceManager::writeFile(const egg::Path& path, const char* data, unsigned int len)
{
    //变成相对路径
    //nbBase::Path p = path;
    //p.setAbsolutePath(false);

    if (mWriteArchiveIndex < FileSystem::getInstance()->numArchive())
    {
        egg::Archive* archive = FileSystem::getInstance()->getArchive(mWriteArchiveIndex);
        archive->setEntryAsFile(path.getPathMel(), data, len, true);
    }
}

void ResourceManager::readFile(const egg::Path& path, std::vector<char>& data, unsigned int& len)
{
    //变成绝对路径 为了find的正确性
    egg::Path p = path;
    p.setAbsolutePath(true);

    auto it = mUniformAssetFileMap.find(p);
    if (it != mUniformAssetFileMap.end())
    {
        egg::Archive* archive = FileSystem::getInstance()->getArchive(it->second.archive_index);
        egg::Entry* entry = archive->getEntry(p.getPathMel());
        len = entry->getLen();
        if (len > 0)
        {
            data.resize(len);
            entry->read(&data[0]);
        }
        else
        {
            printf("missing file %s\n", p.getPathMel().c_str());
        }
    }
}


Resource* ResourceManager::readResource(const egg::Path& uniform_path, bool load_ext
    , const std::string& resource_group_name)
{
    ResourceGroup* rg = this->getResourceGroup(resource_group_name);

    //不管当前的ResourceGroup是否存在 都进行下去
    return readResource(uniform_path, load_ext, rg);
}

Resource* ResourceManager::readResource(const egg::Path& uniform_path, bool load_ext
    , ResourceGroup* resource_group)
{
    //ResourceGroup 有可能是NULL

    //这里只能允许绝对路径
    if (!uniform_path.isAbsolutePath())
    {
        return 0;
    }

    //如果已经存在 直接返回
    auto it = resource_group->mResourceObjectMap.find(uniform_path);
    if (it != resource_group->mResourceObjectMap.end())
    {
        return it->second.get();
    }

    //读文件
    std::vector<char> buffer;
    unsigned int len = 0;
    readFile(uniform_path, buffer, len);

    if (len == 0)
    {
        printf("file not open : %s\n", uniform_path.getPathMel().c_str());
        return nullptr;
    }

    //初始化ISS
    std::stringstream iss(std::ios::binary | std::ios::in | std::ios::out);
    //iss.rdbuf()->sputn(reinterpret_cast<char*>(&buffer[0]), /*sizeof(int) **/ buffer.size());
    //iss.rdbuf()->pubsetbuf(&buffer[0],buffer.size());
    iss.write(&buffer[0], buffer.size());
    //auto a = iss.gcount();


    //根据类型注册生成资源
    //std::string filename = uniform_path;
    //nbBase::Path p = filename;
    std::vector< std::string > ext = uniform_path.splitLastName();
    //nbSG::Serializer* ser = new nbSG::Serializer_Text();
    egg::ref_ptr< egg::Object > obj = ObjectManager::getInstance()->newObject(*ext.rbegin());
    if (obj.valid())
    {
        Resource* resource = dynamic_cast<Resource*>(obj.get());
        if (resource)
        {
            resource->load(uniform_path, iss, ser, ObjectManager::getInstance(), this, load_ext);

            if (resource_group)
            {
                resource_group->add(resource);
            }

            obj.release();
            return resource;
        }
    }

    return NULL;
}
bool ResourceManager::writeResource(Resource* resource /*, const std::string& uniform_path*/)
{
    if (resource == 0)
    {
        printf("resource is null \n");
        return false;
    }

    std::ostringstream oss(std::ios::binary);
    //nbSG::Serializer* ser = new nbSG::Serializer_Text();

    std::string filename = resource->getUniformPath().getPathMel();// + "." + resource->ObjectClassName();
    resource->save(filename, oss, ser);
    if (oss.str().size() > 0)
    {
        writeFile(filename, oss.str().c_str(), oss.str().size());
    }

    return true;
}

Resource* ResourceManager::readResourceGroup(const egg::Path& uniform_path, ResourceGroup* result, bool load_ext)
{
    //这里只能允许绝对路径
    if (!uniform_path.isAbsolutePath())
    {
        return 0;
    }

    //找到资源对象//读取主资源
    Resource* resource = this->readResource(uniform_path, load_ext, result);
    if (resource)
    {
        //如果没有引用资源就直接结束
        std::vector< egg::ref_ptr< ResourceLink > > link;
        resource->collectionResourceLink(link);
        //if( link.size() == 0 ) return;

        //循环列表
        for (unsigned int i = 0; i < link.size(); i++)
        {
            //连地址都不存在 直接无视
            if (link[i]->getPath().isNull()) continue;

            //如果已经存在 直接连接
            auto it = result->mResourceObjectMap.find(link[i]->getPath());
            if (it != result->mResourceObjectMap.end())
            {
                link[i]->setRes(it->second.get());
                continue;
            }

            //读取资源
            Resource* res = readResourceGroup(link[i]->getPath(), result, load_ext);
            if (res)
            {
                //如果正确的读取了链接资源  就更新这个link的res
                link[i]->setRes(res);
            }
        }

        result->cleanupLink();
    }
    return resource;
}

bool ResourceManager::writeResourceGroup(ResourceGroup* resource_group)
{
    //轮询所有资源 全部写入到文件 资源链接只写入名字
    for (auto it = resource_group->mResourceObjectMap.begin();
        it != resource_group->mResourceObjectMap.end();
        it++)
    {
        //it->second->save();
        this->writeResource(it->second.get());
    }
    return true;
}

ResourceGroup* ResourceManager::getResourceGroup(const std::string& name)
{
    auto it = mResourceGroupMap.find(name);
    if (it != mResourceGroupMap.end())
    {
        return &(it->second);
    }
    return 0;
}

