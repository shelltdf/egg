#ifndef NIUBI_SG_MATERIAL_H
#define NIUBI_SG_MATERIAL_H

#include <eggEngine/Export.h>

#include <map>

//#include <eggBase/Public.h>

#include <eggEngine/Resource.h>
//#include <eggEngine/Program.h>
//#include <eggEngine/Texture.h>
#include <eggEngine/Pass.h>


/*
* Material
*   一个资源记录文件
*   主要表达一个effect的实例化数据
*   主要机构是tech下关联pass  每个pass关联对应的渲染状态和uniform数值
*   关于具体shader的定义 会提供一个IO定义
*/




#define NB_DEFAULT_THCH_NAME "default"


namespace egg
{

    /*
     *   建立材质后默认生成一下内容
     *       tech "default"
     *       pass 0
     *       shader类型 NB_SHADER_TYPE_ONLY_VERTEX 只有定点数据
     *       (默认只是保证了几何物体能被渲染出来 至于观感如何 不保证)
     */
    class EGG_ENGINE_EXPORT Material
        :public Resource
    {
    public:

        //Tech类型
        //typedef std::vector<Pass> Tech;
        class Tech
            :public Object
        {
        public:
            Tech() {}
            virtual ~Tech() {}

            std::vector< egg::ref_ptr< Pass > > mPasses;
        };


        Material();
        virtual ~Material();

        //virtual std::string getExt(){return "material";}
        virtual std::string ObjectClassName() override { return "material"; }

        //当前tech
        std::string getCurrentTech() { return mCurrentTech; }
        void setCurrentTech(std::string tech) { mCurrentTech = tech; }

        //tech管理
        std::vector<std::string > getTechNames();
        Tech* getOrCreateTech(std::string tech_name = NB_DEFAULT_THCH_NAME);
        Tech* getTech(std::string tech_name = NB_DEFAULT_THCH_NAME);
        void removeTech(std::string tech_name = NB_DEFAULT_THCH_NAME);
        void removeTech(Tech* t);


        //收集已经加载的资源链接
        virtual void collectionResourceObject(std::map< egg::Path, egg::ref_ptr< Resource > >& result) override;

        //收集尚未加载的资源路径
        virtual void collectionResourceLink(std::vector< egg::ref_ptr< ResourceLink > >& result) override;

        virtual void save(egg::Path filename, std::ostream& os, Serializer* ser) const override;
        virtual void load(egg::Path filename, std::istream& is, Serializer* ser, ObjectManager* om
            , egg::Referenced* rmg, bool load_ext) override;

    private:

        //当前tech
        std::string mCurrentTech;

        //Tech
        std::map<std::string, egg::ref_ptr<Tech> > mTechs;

        //定义VBO属性映射关系
        //std::map<std::string , unsigned int > mAttrbuteIDs;

    };

}//namespace nbSG

#endif//NIUBI_SG_MATERIAL_H
