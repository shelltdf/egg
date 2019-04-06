#ifndef NIUBI_Level_H
#define NIUBI_Level_H

#include <eggEngine/Resource.h>

#include <egg/Math.h>
#include <eggEngine/Matrix.h>
#include <eggEngine/EntityManager.h>
#include <eggEngine/SceneNode.h>

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

        //读写函数
        virtual void save(Path filename, std::ostream& os, Serializer* ser) const override;
        virtual void load(Path filename, std::istream& is, Serializer* ser
            , ObjectManager* om, Referenced* rmg, bool load_ext) override;

        //托管世界
        void setRoot(SceneNode* root) { mRoot = root; }

        //实体操作
        Entity* createEntity(const std::string& type_name, unsigned int id = 0/*auto*/);
        void destroyEntity(unsigned int id);
        void spawnEntity(unsigned int id
            , Eigen::Vector3f pos = Eigen::Vector3f(0, 0, 0)
            , Eigen::Vector3f rot = Eigen::Vector3f(0, 0, 0)
            , Eigen::Vector3f scale = Eigen::Vector3f(1, 1, 1));
        void unspawnEntity(unsigned int id);
        Entity* getEntity(unsigned int id);

        const std::map<unsigned int/*UID*/, ref_ptr<Entity> > getEntityList()& { return mEntityList; }

    private:

        //产品列表 每个entity都有唯一的UID
        std::map<unsigned int/*UID*/, ref_ptr<Entity> > mEntityList;

        //UID起算点
        unsigned int mLastID = 0;

        //场景图
        SceneNode* mRoot = 0;

    };


}//namespace egg

#endif//NIUBI_Level_H
