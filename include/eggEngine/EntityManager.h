#ifndef _EGG_EntityManager_H_
#define _EGG_EntityManager_H_

#include <egg/Runtime.h>
#include <eggEngine/Engine.h>
#include <eggEngine/RenderWindow.h>
#include <eggEngine/Animation.h>


namespace egg
{
    //简单属性对象
    //  支持default设置
    //  支持dirty状态
    //  支持简单序列化
    class EGG_ENGINE_EXPORT Property
    {
    public:
        Property() {}
        ~Property() {}
    };

    //模板
    template<typename T>
    class PropertyT
        :public Property
    {
    public:
        PropertyT() {}
        ~PropertyT() {}

        void set(T v) { mValue = v; }
        T get() { return mIsSet ? mValue : mDefaultValue; }

        bool isSet() { return mIsSet; }
        void setDefault() { mValue = mDefaultValue; }
        void clear() { mIsSet = false; }

    private:

        bool mIsSet = false;
        T mDefaultValue;
        T mValue;
    };

    //基础数据定义
    typedef PropertyT< int > PropertyInt;
    typedef PropertyT< float > PropertyFloat;
    typedef PropertyT< double > PropertyDouble;


    //实体对象
    class EGG_ENGINE_EXPORT Entity
        :public Object
    {
    public:
        Entity(unsigned int UID = 0)
            :mUID(UID)
            , mSpawn(false)
            , mBind(false)
        {}
        virtual ~Entity() {}

        //基本信息
        void setUID(unsigned int uid) { mUID = uid; }
        unsigned int getUID() { return mUID; }
        //virtual std::string getClassName() = 0;

        //父子关系
        void addChild(Entity* child);
        int getNumChildren();
        Entity* getChild(int index);
        void removeChild(Entity* entity);
        void removeChild(int index);

        //new后的消息
        virtual void onCreate() {};

        //delete之前的消息
        virtual void onDestroy() {};

        //直接产生
        //  由实体负责建立SN内容，也可能是空的。
        virtual void onSpawn(SceneNode* parent, Eigen::Vector3f pos, Eigen::Vector3f rot, Eigen::Vector3f scale) {};

        //绑定
        //  SN已经存在,给他初始化一个entity对象。也可能是空sn。
        virtual void onBind(Entity* parent, SceneNode* self) {};

        //销毁
        //  销毁实体对象。并且回收由entity管理的资源。
        virtual void onUnSpawn(SceneNode* parent) {};

        //更换父节点
        //  如果父节点发生变化。比如工具交给不同的人。
        virtual void onChangeParent(Entity* parent) {};

        //修改属性
        //  某个属性变化后，给出一个更新提示。
        virtual void onPropertyChange(Property old_property, Property new_property) {};

        //属性被修改
        virtual void onPropertyDirty(Property property) {};

        //渲染流程消息
        virtual void onBegin() {};
        virtual void onUpdate() {};
        virtual void onCull() {};
        virtual void onDraw() {};
        virtual void onSwapBuffer() {};

        //通用消息
        virtual void onMessage(Entity* from, Entity* to, const std::string& message) {};

        //逻辑消息
        virtual void onEnter() {}; /*进入区域*/
        virtual void onExit() {};  /*离开区域*/
        virtual void onHit() {};   /*被攻击*/
        virtual void onKill() {};  /*强制死亡*/
        virtual void onStateChange() {}; /*状态改变*/


        //属性操作
        //   这里有两种属性，反射属性，简单属性。
        //   我们不需要反射这么复杂的支持。
        //   我们需要一个有默认值，可以记录dirty的属性。只需要记录简单的数据条。根本不需要复杂的对象。
        //   所以不需要反射。只要支持简单的序列化即可。所以要继承自Object。
        Property* getProperty(const std::string& name) { return 0; }
        void setProperty(const std::string& name, Property* p) {}

    protected:

    private:

        //全局UID
        unsigned int mUID;

        //是否已经被产生
        bool mSpawn;

        //是否是绑定对象
        bool mBind;

        //子Entity
        std::vector< ref_ptr<Entity> > mChildren;
    };


    //全局实体工厂
    class EGG_ENGINE_EXPORT EntityFactory
        :public Factory<Entity>
    {
    public:
        EntityFactory() {}
        ~EntityFactory() {}
        DECLARE_SINGLETON(EntityFactory)

        Entity* createEntity(const std::string& type_name);
    };


}//namespace egg

#endif //_EGG_EntityManager_H_

