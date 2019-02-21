
#include <eggEngine/RenderPass.h>

using namespace egg;

RenderPass::RenderPass()
{
    mRenderNode = new RenderNode();
}

RenderPass::~RenderPass()
{
}

void StandardRenderPass::setCamera(egg::Camera* camera)
{
    mCamera = camera;
}

void StandardRenderPass::setRenderScene(egg::SceneNode* scenenode)
{
    mRenderScene = scenenode;
}

void StandardRenderPass::setMaterialPass(egg::Pass* pass)
{
    mMaterialPass = pass;
}


ImageRenderPass::ImageRenderPass()
{
}

void ImageRenderPass::setMaterialPass(egg::Pass* pass)
{
}

egg::Pass* ImageRenderPass::getMaterialPass()
{
    return 0;
}

