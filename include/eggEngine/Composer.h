#ifndef _EGG_COMPOSER_H_
#define _EGG_COMPOSER_H_

#include <eggEngine/Export.h>

#include <eggEngine/Resource.h>
#include <eggEngine/RenderPass.h>
#include <eggEngine/SceneNode.h>


namespace egg
{
    /*
    *  合成器对象
    *
    *  默认建立合成器是空的，完全没有任何设置。
    *  在cull过程中会自动认为他是一个前向渲染。
    *  SceneNode和Camera对象默认情况下指向RenderWindow的默认设置。
    *
    *  SceneNode对应的slot设置跟RenderWindow的一致。
    *  Camera的设置跟RenderWindow的一致。
    *  所以这里仅仅记录SceneNode和Camera的对应名称。
    */
    class EGG_ENGINE_EXPORT Composer
        :public Resource
    {
    public:
        Composer();
        virtual ~Composer();


        virtual void save(Path filename, std::ostream& os, Serializer* ser) const override;
        virtual void load(Path filename, std::istream& is, Serializer* ser
            , ObjectManager* om, Referenced* rmg, bool load_ext) override;


        std::map<std::string, ref_ptr< RenderPass > > getPasses() { return mPasses; }
        std::map<std::string, ref_ptr< Texture > > getTargets() { return mTargets; }
        //std::map<std::string, ref_ptr< SceneNode > >getSceneNodes() { return mSceneNodes; }

        void addStandardPass(std::string pass_name);
        void addImagePass(std::string pass_name);

        void addRengerTarget(std::string target_name);

        void setInputSceneNode(std::string pass_name, std::string node_path);
        void setInputCamera(std::string pass_name, std::string node_path);

        void setRengerTarget(std::string pass_name, std::string target_name);


    private:

        std::map<std::string, ref_ptr< RenderPass > > mPasses;
        std::map<std::string, ref_ptr< Texture > > mTargets;
        //std::map<std::string, ref_ptr< SceneNode > > mSceneNodes;

        std::map<std::string, std::string > mInputCamera;
        std::map<std::string, std::string > mInputSceneNodeSlot;

    };

}//namespace egg

#endif // _EGG_COMPOSER_H_
