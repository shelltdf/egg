#ifndef _EGG_Save_Load_H_
#define _EGG_Save_Load_H_

#include <eggRuntime/Runtime.h>
#include <eggEngine/Engine.h>
#include <eggEngine/RenderWindow.h>
#include <eggEngine/Animation.h>


namespace egg
{

    class EGG_ENGINE_EXPORT Saver
        :public Object
    {
    public:
        Saver() {}
        virtual ~Saver() {}

    protected:

    };


    class EGG_ENGINE_EXPORT SaverFactory
        :public Factory<Saver>
    {
    public:
        SaverFactory() {}
        ~SaverFactory() {}
        DECLARE_SINGLETON(SaverFactory)
    };


    class EGG_ENGINE_EXPORT Loader
        :public Object
    {
    public:
        Loader() {}
        ~Loader() {}

    protected:

    };


    class EGG_ENGINE_EXPORT LoaderFactory
        :public Factory<Loader>
    {
    public:
        LoaderFactory() {}
        ~LoaderFactory() {}
        DECLARE_SINGLETON(LoaderFactory)
    };


}//namespace egg

#endif //_EGG_Save_Load_H_

