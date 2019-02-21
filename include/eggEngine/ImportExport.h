#ifndef _EGG_Import_Export_H_
#define _EGG_Import_Export_H_

#include <eggRuntime/Runtime.h>
#include <eggEngine/Engine.h>
#include <eggEngine/RenderWindow.h>
#include <eggEngine/Animation.h>


namespace egg
{

    class EGG_ENGINE_EXPORT Importer
        :public Object
    {
    public:
        Importer() {}
        virtual ~Importer() {}

        virtual SceneNode* importSceneNode() { return 0; }
        virtual Material* importMaterial() { return 0; }
        virtual Image* importImage() { return 0; }
        virtual Geometry* importGeometry() { return 0; }
        virtual Animation* importAnimation() { return 0; }

    protected:

    };


    class EGG_ENGINE_EXPORT ImporterFactory
        :public Factory<Importer>
    {
    public:
        ImporterFactory() {}
        ~ImporterFactory() {}
        DECLARE_SINGLETON(ImporterFactory)
    };


    class EGG_ENGINE_EXPORT Exporter
        :public Object
    {
    public:
        Exporter() {}
        ~Exporter() {}

        virtual bool exportSceneNode(SceneNode*) { return false; }
        virtual bool exportMaterial(Material*) { return false; }
        virtual bool exportImage(Image*) { return false; }
        virtual bool exportGeometry(Geometry*) { return false; }
        virtual bool exportAnimation(Animation*) { return false; }

    protected:

    };


    class EGG_ENGINE_EXPORT ExporterFactory
        :public Factory<Exporter>
    {
    public:
        ExporterFactory() {}
        ~ExporterFactory() {}
        DECLARE_SINGLETON(ExporterFactory)
    };


}//namespace egg

#endif //_EGG_Import_Export_H_

