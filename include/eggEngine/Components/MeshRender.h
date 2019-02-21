#ifndef NIUBI_SG_MESHRENDER_H
#define NIUBI_SG_MESHRENDER_H

#include <eggEngine/Component.h>

#include <eggEngine/Material.h>
#include <eggEngine/Geometry.h>

namespace egg
{
    class EGG_ENGINE_EXPORT MeshRender
        :public ComponentBase
    {
    public:

        MeshRender() {}
        virtual ~MeshRender() {}

        virtual std::string ObjectClassName() override { return "meshrender"; }

        //ÏûÏ¢·Ö¼ð
        virtual void traverse(NodeVisitor& nv);

        virtual void onUpdate(nv_update* nv) override {}
        virtual void onCull(nv_cull* nv, Eigen::Matrix4f& vm) override;
        virtual void onRender(nv_render* nv, Eigen::Matrix4f& mv) override;
        virtual void onInput(nv_input* nv) override {}

        virtual void save(Path filename, std::ostream& os
            , Serializer* ser) const override;

        virtual void load(Path filename, std::istream& is
            , Serializer* ser, ObjectManager* om
            , Referenced* rmg, bool load_ext) override;


        RESOURCE_LINK_VECTOR(Material);
        RESOURCE_LINK(Geometry);

    private:

        Material* fixMaterial(unsigned int material_id);
    };
}//namespace egg

#endif//NIUBI_SG_MESHRENDER_H
