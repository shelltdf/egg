#ifndef _EGG_RENDERVISITOR_H_
#define _EGG_RENDERVISITOR_H_

#include <eggEngine/Export.h>

#include <eggEngine/Window.h>
//#include <eggEngine/GraphicsContext.h>
#include <eggEngine/Renderer.h>
#include <eggEngine/Pass.h>
//#include <eggEngine/SceneNode.h>
//#include <eggEngine/Components/Camera.h>
//#include <eggEngine/Composer.h>
#include <egg/NodeVisitor.h>
#include <egg/Math.h>
#include <eggEngine/Matrix.h>

namespace egg
{
    class RenderNode;
    class SceneNode;

    class nv_update
        :public NodeVisitor
    {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        nv_update() { currentViewMatrix.setIdentity(); }
        virtual ~nv_update() { }
        virtual void apply(Node& node) override;

        double dt;
        Eigen::Matrix4f currentViewMatrix; //µ±«∞VMæÿ’Û
    };

    class nv_cull
        :public NodeVisitor
    {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        virtual ~nv_cull() {}
        virtual void apply(Node& node) override { traverse(node); }

        float dt;

        Eigen::Matrix4f projactiveMatrix;
        Eigen::Matrix4f cameraViewMatrix;

        SceneNode* mSceneNode = 0;
        RenderNode* mRenderNode = 0;
        bool ignore_pass = false;
    };

    class nv_render
        :public NodeVisitor
    {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        nv_render() {}
        virtual ~nv_render() {}
        virtual void apply(Node& node) override { traverse(node); }

        float dt = 0.0f;
        GraphicsContext* gc = 0;
        Renderer* render = 0;
        Eigen::Matrix4f projactiveMatrix;
        //unsigned int context = 0;
    };

    class nv_gui
        :public NodeVisitor
    {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        nv_gui() {}
        virtual ~nv_gui() {}
        virtual void apply(Node& node) override { traverse(node); }

        GraphicsContext* gc = 0;
        Renderer* renderer = 0;
        Window* window = 0;
        Eigen::Vector4f viewport;
    };


}//namespace egg

#endif // _EGG_RENDERVISITOR_H_
