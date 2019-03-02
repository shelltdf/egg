#ifndef _EGG_EntityManager_H_
#define _EGG_EntityManager_H_

#include <egg/Runtime.h>
#include <eggEngine/Engine.h>
#include <eggEngine/RenderWindow.h>
#include <eggEngine/Animation.h>


namespace egg
{

    class EGG_ENGINE_EXPORT Entity
        :public Object
    {
    public:
        Entity() {}
        virtual ~Entity() {}

        virtual void onSpawn() {}
        virtual void onUnSpawn() {}
        virtual void onReSpawn() {}

    protected:

    };


    class EGG_ENGINE_EXPORT EntityFactory
        :public Factory<Entity>
    {
    public:
        EntityFactory() {}
        ~EntityFactory() {}
        DECLARE_SINGLETON(EntityFactory)
    };


}//namespace egg

#endif //_EGG_EntityManager_H_

