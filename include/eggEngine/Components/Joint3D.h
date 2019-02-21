#ifndef _EGG_Component_Joint3D_H_
#define _EGG_Component_Joint3D_H_

#include <eggEngine/Component.h>

#include <eggEngine/Components/Transfrom3D.h>

namespace egg
{

    //¿Õ¼ä¹Ç÷À½Úµã
    class EGG_ENGINE_EXPORT Joint3D
        :public Transfrom3D
    {
    public:
        Joint3D() {}
        virtual ~Joint3D() {}

        //virtual std::string getTypeName(){return "joint3d";}
        virtual std::string ObjectClassName() override { return "joint3d"; }

        virtual void save(Path filename, std::ostream& os, Serializer* ser) const override
        {
            Transfrom3D::save(filename, os, ser);
        }

        virtual void load(Path filename, std::istream& is, Serializer* ser, ObjectManager* om, Referenced* rmg, bool load_ext) override
        {
            Transfrom3D::load(filename, is, ser, om, rmg, load_ext);
        }

    };

}//namespace egg

#endif //_EGG_Component_Joint3D_H_

