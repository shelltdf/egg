#ifndef _EGG_ENGINE_H_
#define _EGG_ENGINE_H_

#include <eggEngine/Export.h>

#include <eggRuntime/Singleton.h>
#include <eggRuntime/Object.h>

#include <eggEngine/SceneNode.h>
#include <eggEngine/Material.h>
#include <eggEngine/Image.h>
#include <eggEngine/Geometry.h>
#include <eggEngine/Animation.h>

namespace egg
{

    class EGG_ENGINE_EXPORT Engine
        :public Object
    {
    public:
        Engine();
        virtual ~Engine();

        DECLARE_SINGLETON(Engine)


        SceneNode* importSceneNode(const std::string& filename) { return 0; }
        Material* importMaterial(const std::string& filename) { return 0; }
        Image* importImage(const std::string& filename) { return 0; }
        Geometry* importGeometry(const std::string& filename) { return 0; }
        Animation* importAnimation(const std::string& filename) { return 0; }

        bool exportSceneNode(SceneNode* scenenode, const std::string& filename) { return false; }
        bool exportMaterial(Material* material, const std::string& filename) { return false; }
        bool exportImage(Image* image, const std::string& filename) { return false; }
        bool exportGeometry(Geometry* geometry, const std::string& filename) { return false; }
        bool exportAnimation(Animation* animation, const std::string& filename) { return false; }


    };
}

#endif // !_EGG_ENGINE_H_
