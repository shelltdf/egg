#ifndef _EGG_RENDERNODE_H_
#define _EGG_RENDERNODE_H_

#include <eggEngine/Export.h>

#include <eggRuntime/Math.h>
#include <eggRuntime/Node.h>
#include <eggEngine/Geometry.h>
#include <eggEngine/Material.h>
#include <eggEngine/RenderVisitor.h>


namespace egg
{

    class EGG_ENGINE_EXPORT RenderLeaf
        :public Node
    {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        RenderLeaf() {}
        virtual ~RenderLeaf() {}

        void traverse(NodeVisitor& nv) override;

        struct DrawableSet
        {
            Drawable* drawable = 0;
            int subgeometry_index = -1;
        };

        void addDrawable(DrawableSet sg) { mDrawableSet.push_back(sg); }

        void setViewMatrix(const Eigen::Matrix4f& vm) { mViewMatrix = vm; }
        void setProjectionMatrix(const Eigen::Matrix4f& pm) { mProjectionMatrix = pm; }

    private:

        Eigen::Matrix4f mViewMatrix;
        Eigen::Matrix4f mProjectionMatrix;

        //std::vector< ref_ptr<SubGeometry> > mSubGeometryList;
        std::vector< DrawableSet > mDrawableSet;
    };

    class EGG_ENGINE_EXPORT RenderNode
        :public Node
    {
    public:
        RenderNode();
        virtual ~RenderNode();

        void traverse(NodeVisitor& nv) override;

        RenderNode* getOrCreateChild(Pass* pass);

        void setPass(egg::Pass* pass) { mPass = pass; }
        egg::Pass* getPass() { return mPass.get(); }

        RenderLeaf* getOrCreateRenderLeaf();

    private:

        ref_ptr<egg::Pass> mPass;

        ref_ptr<RenderLeaf> mRenderLeaf;

    };


}//namespace egg

#endif // _EGG_RENDERNODE_H_
