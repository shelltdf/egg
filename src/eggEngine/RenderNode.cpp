
#include <eggEngine/RenderNode.h>

using namespace egg;

void RenderLeaf::traverse(NodeVisitor& nv)
{
    if (nv.as<nv_render>())
    {
        nv_render* _nv_render = nv.as<nv_render>();

        //modify view matrix and projection matrix
        RenderState* rs = _nv_render->render->getRenderState();
        rs->setModelViewMatrix(mViewMatrix.data());
        rs->setProjectionMatrix(mProjectionMatrix.data());
        rs->applyMatrix(_nv_render->gc, _nv_render->render);

        //render geometry
        for (size_t i = 0; i < mDrawableSet.size(); i++)
        {
            //render->render->bindGeometry(render->gc, mSubGeometryList[i].geometry);
            //render->render->drawGeometry(render->gc, mSubGeometryList[i].geometry, mSubGeometryList[i].index);
            //render->render->unbindGeometry(render->gc, mSubGeometryList[i].geometry);

            mDrawableSet[i].drawable->draw(_nv_render, mDrawableSet[i].subgeometry_index);
        }

    }
}


RenderNode::RenderNode()
{
}

RenderNode::~RenderNode()
{
}

void RenderNode::traverse(NodeVisitor& nv)
{
    if (nv.as<nv_render>())
    {
        nv_render* render = nv.as<nv_render>();

        //push render state
        RenderState* rs = nv.as<nv_render>()->render->getRenderState();
        if (mPass.valid())rs->push(mPass.get());

        //traverse RenderLeaf
        if (mRenderLeaf.valid())
        {
            //enable render state
            RenderState* rs = render->render->getRenderState();
            rs->bindRenderPass(render->gc, render->render);
            //rs->applyMatrix(render->gc, render->render);

            //traverse
            mRenderLeaf->accept(nv);
        }

        //traverse children 
        Node::traverse(nv);

        //pop render state
        if (mPass.valid())rs->pop();

        //disable render state
        if (mRenderLeaf.valid())
        {
            RenderState* rs = render->render->getRenderState();
            rs->unbindRenderPass();
        }
    }
}

RenderNode* RenderNode::getOrCreateChild(Pass* pass)
{
    for (size_t i = 0; i < this->getNumChild(); i++)
    {
        RenderNode* child = dynamic_cast<RenderNode*>(this->getChild(i));
        if (child == 0) continue;

        //对比pass内容 这里暂时只对比指针
        if (child->getPass() == pass)
        {
            return child;
        }
    }

    RenderNode* rn = new RenderNode();
    this->addChild(rn);

    return rn;
}

RenderLeaf* RenderNode::getOrCreateRenderLeaf()
{
    if (!mRenderLeaf.valid())
    {
        mRenderLeaf = new RenderLeaf();
    }
    return mRenderLeaf.get();
}

