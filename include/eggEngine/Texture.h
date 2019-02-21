#ifndef NIUBI_SG_TEXTURE_H
#define NIUBI_SG_TEXTURE_H

#include <eggEngine/Export.h>

//#include <eggBase/Public.h>

#include <eggEngine/Resource.h>
#include <eggEngine/DataDefine.h>
#include <eggEngine/Image.h>



/*
* 场景资源
*/
namespace egg
{

    class EGG_ENGINE_EXPORT Texture
        :public Resource
    {
    public:
        Texture() {}
        virtual ~Texture() {}

        //virtual std::string getExt(){return "texture";}
        virtual std::string ObjectClassName() override { return "texture"; }

        virtual void save(egg::Path filename, std::ostream& os, Serializer* ser) const override;

        virtual void load(egg::Path filename, std::istream& is, Serializer* ser, ObjectManager* om
            , egg::Referenced* rmg, bool load_ext) override;

        //收集已经加载的资源链接
        virtual void collectionResourceObject(std::map< egg::Path, egg::ref_ptr< Resource > >& result) override;

        //收集尚未加载的资源路径
        virtual void collectionResourceLink(std::vector< egg::ref_ptr< ResourceLink > >& result) override;

        //GL句柄
        //    在具体渲染的时候产生句柄
        RendererResourceSet<handle_texture> mGLHandle;


        RESOURCE_LINK(Image)
    };


}//namespace nbSG

#endif//NIUBI_SG_TEXTURE_H
