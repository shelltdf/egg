#ifndef _EGG_RENDERPASS_H_
#define _EGG_RENDERPASS_H_

#include <eggEngine/Export.h>

#include <eggEngine/Components/Camera.h>
#include <eggEngine/Geometry.h>
#include <eggEngine/Material.h>
#include <eggEngine/RenderNode.h>


namespace egg
{
    class EGG_ENGINE_EXPORT RenderPass
        :public Object
    {
    public:
        RenderPass();
        virtual ~RenderPass();

        virtual void setCamera(egg::Camera* camera) = 0;
        virtual void setRenderScene(egg::SceneNode* scenenode) = 0;
        virtual void setMaterialPass(egg::Pass* pass) = 0;

        virtual egg::Camera* getCamera() = 0;
        virtual egg::SceneNode* getRenderScene() = 0;
        virtual egg::Pass* getMaterialPass() = 0;

        virtual egg::RenderNode* getRenderNode() { return mRenderNode.get(); }
        virtual void resetRenderNode() { mRenderNode = new RenderNode(); }

    private:

        //渲染图根节点
        ref_ptr<RenderNode> mRenderNode;

        //egg::Material::Pass* mPass = 0;
        //egg::Camera* mCamera = 0;
        //egg::SceneNode* mRenderScene = 0;
        //egg::Geometry* mImageGeometry = 0;
    };

    class EGG_ENGINE_EXPORT StandardRenderPass
        :public RenderPass
    {
    public:
        StandardRenderPass() {}
        virtual ~StandardRenderPass() {}

        virtual void setCamera(egg::Camera* camera) override;
        virtual void setRenderScene(egg::SceneNode* scenenode) override;
        virtual void setMaterialPass(egg::Pass* pass) override;

        virtual egg::Camera* getCamera() override { return mCamera; }
        virtual egg::SceneNode* getRenderScene() override { return mRenderScene; }
        virtual egg::Pass* getMaterialPass() override { return mMaterialPass; }

    private:

        //全局渲染的覆盖材质
        egg::Pass* mMaterialPass = 0;

        //关联摄像机
        egg::Camera* mCamera = 0;

        //关联场景图
        egg::SceneNode* mRenderScene = 0;
    };

    class EGG_ENGINE_EXPORT ImageRenderPass
        :public RenderPass
    {
    public:
        ImageRenderPass();
        virtual ~ImageRenderPass() {}

        virtual void setCamera(egg::Camera* camera) override {};
        virtual void setRenderScene(egg::SceneNode* scenenode) override {};
        virtual void setMaterialPass(egg::Pass* pass) override;

        virtual egg::Camera* getCamera() override { return 0; }
        virtual egg::SceneNode* getRenderScene() override { return 0; }
        virtual egg::Pass* getMaterialPass() override;

    private:

        //不记录成员变量 一切操作直接针对mRenderNode

        //egg::Material::Pass* mPass = 0;
        //egg::Geometry* mImageGeometry = 0;
    };


}//namespace egg

#endif // _EGG_RENDERPASS_H_
