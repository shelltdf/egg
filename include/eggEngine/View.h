#ifndef NIUBI_SG_VIEW_H
#define NIUBI_SG_VIEW_H
#if 0
#include <eggEngine/Renderer.h>

//#include <nbBase/Referenced>
#include <eggRuntime/Node.h>
#include <eggRuntime/NodeVisitor.h>
//#include <nbSG/SceneNode.h>
#include <eggRuntime/Math.h>


namespace egg
{

    class SceneNode;

    /*
    * 渲染容器
    *     以后改成 RenderView
    */

    class EGG_ENGINE_EXPORT View
        :public Node
    {
    public:
        View()
        {
            viewMatrix = vmml::mat4f::IDENTITY;
            projactiveMatrix = vmml::mat4f::IDENTITY;
        }
        virtual ~View(){}

        //viewMatrix
        vmml::mat4f viewMatrix;
        //projactiveMatrix
        vmml::mat4f projactiveMatrix;
        //viewPort
        vmml::vec4f viewPort;

        //bg color
        //bg clean mask

        void update(float dt);
        void cull(float dt);
        void render(Renderer* render,unsigned int context,float dt/*, vmml::mat4f& projactiveMatrix, vmml::mat4f& viewMatrix*/);


        class nv_update
            :public NodeVisitor
        {
        public:
            virtual ~nv_update(){}
            virtual void apply(Node& node) override;

            float dt;
            vmml::mat4f currentViewMatrix; //当前VM矩阵
        };

        class nv_cull
            :public NodeVisitor
        {
        public:
            virtual ~nv_cull(){}
            virtual void apply(Node& node) override{ traverse(node); }

            float dt;
        };

        class nv_render
            :public NodeVisitor
        {
        public:
            nv_render():render(0),context(0){}
            virtual ~nv_render(){}
            virtual void apply(Node& node) override{ traverse(node); }

            float dt;
            Renderer* render;
            vmml::mat4f projactiveMatrix;
            unsigned int context;
        };

    };

}//namespace nbSG
#endif
#endif//NIUBI_SG_VIEW_H
