#ifndef NIUBI_Level_H
#define NIUBI_Level_H

#include <eggEngine/Resource.h>

#include <egg/Math.h>
#include <eggEngine/Matrix.h>

namespace egg
{

    /*
    * Level
    */

    class EGG_ENGINE_EXPORT Level
        :public Resource
    {
    public:

        Level() {}
        virtual ~Level() {}

        virtual std::string ObjectClassName() override { return "level"; }

        //¶ÁÐ´º¯Êý
        virtual void save(Path filename, std::ostream& os, Serializer* ser) const override;
        virtual void load(Path filename, std::istream& is, Serializer* ser
            , ObjectManager* om, Referenced* rmg, bool load_ext) override;

    private:

    };


}//namespace egg

#endif//NIUBI_Level_H
