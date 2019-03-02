#ifndef NIUBI_SG_RESOURCEMANAGER_H
#define NIUBI_SG_RESOURCEMANAGER_H

#include <eggEngine/Export.h>

#include <map>

//#include <eggBase/Public.h>
#include <egg/Log.h>
#include <egg/SystemArg.h>

#include <eggEngine/Resource.h>
#include <eggEngine/VirtualFileSystem.h>


namespace egg
{

    /*
    * 资源管理器
    *
    * 资源类型：
    *       mesh(skeleton bindpose weight) geometry(subgeometry)
    *       image texture
    *       shader program material
    *       scenenode
    */

    /*
    * 内存资源的容器
    *   1 完成资源内存管理
    *   2 作为资源容器使用
    */
#define NB_RESOURCEGROUP_DEFAULT_NAME "default"
    class EGG_ENGINE_EXPORT ResourceGroup
    {
    public:
        ResourceGroup(Resource* resource = 0);

        void add(Resource* resource = 0);

        //遍历所有可能存在的资源
        void searchAll();

        void cleanupLink();

        void clear();

        std::map< egg::Path, egg::ref_ptr< Resource > > mResourceObjectMap;
        std::vector< egg::ref_ptr< ResourceLink > > mResourceLinkList;
    };

    /*
        resource manager
        负责文件和内存对象的转换过程
    */
    class EGG_ENGINE_EXPORT ResourceManager
        :public egg::Referenced
    {
    public:

        ResourceManager();
        virtual ~ResourceManager();

        //manager(ResourceManager)
        DECLARE_SINGLETON(ResourceManager)

        void init(egg::niubi_entry_arg& arg, bool binary);

        //统一asset文件路径 支持覆盖关系
        class UniformAssetFile
        {
        public:
            egg::Path uniform_path;   //内路径
            unsigned char type;         //文件类型 0文件 1目录
            unsigned int archive_index; //最新一个所在index
            unsigned int repeat_count;  //重名次数
        };

        //包管理
        void registerArchive(const std::string& type, const std::string& path, bool write);
        void unRegisterArchive(const std::string& path);
        void updateUniformAssetPath();
        std::map< egg::Path, UniformAssetFile > getUniformAssetFileMap() { return mUniformAssetFileMap; }


        //包内读写文件
        //    path为包内绝对路径处理
        void writeFile(const egg::Path& path, const char* data, unsigned int len);
        void readFile(const egg::Path& path, std::vector<char>& data, unsigned int& len);
        void setWriteArchiveIndex(unsigned int index) { mWriteArchiveIndex = index; }
        unsigned int getWriteArchiveIndex() { return mWriteArchiveIndex; }


        //读写单独资源 不处理引用链接
        Resource* readResource(const egg::Path& uniform_path, bool load_ext, const std::string& resource_group_name = NB_RESOURCEGROUP_DEFAULT_NAME);
        Resource* readResource(const egg::Path& uniform_path, bool load_ext, ResourceGroup* resource_group);
        bool writeResource(Resource* resource /*, const std::string& uniform_path*/);


        //读写资源组 处理所有引用链接
        //ResourceGroup* readResourceGroup(const std::string& uniform_path);
        Resource* readResourceGroup(const egg::Path& uniform_path, ResourceGroup* result, bool load_ext);
        bool writeResourceGroup(ResourceGroup* resource_group);


        //查询资源组
        ResourceGroup* getResourceGroup(const std::string& name = NB_RESOURCEGROUP_DEFAULT_NAME);

        //获取对象管理器
        //ObjectManager* getObjectManager() { return mObjectManager.get(); }

    private:

        //外部存储介质资源管理
        //egg::FileSystem mFileSystem;
        unsigned int mWriteArchiveIndex;
        std::map< egg::Path, UniformAssetFile > mUniformAssetFileMap;

        //内存管理
        std::map< std::string, ResourceGroup > mResourceGroupMap;

        //对象序列化
        //egg::ref_ptr< ObjectManager > mObjectManager;
        bool use_binary;
        egg::Serializer* ser;

    };

}//namespace egg

#endif//NIUBI_SG_RESOURCEMANAGER_H
