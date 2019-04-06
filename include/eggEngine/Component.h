#ifndef NIUBI_SG_COMPONENTBASE_H
#define NIUBI_SG_COMPONENTBASE_H

//#include <nbBase/Public.h>
#include <string>

#include <egg/Math.h>
#include <egg/Object.h>
#include <egg/Node.h>
//#include <eggEngine/View.h>

#include <eggEngine/Resource.h>
#include <eggEngine/RenderVisitor.h>
#include <eggEngine/InputVisitor.h>
#include <eggEngine/Matrix.h>

namespace egg
{

    /*
    * 功能组分节点 不仅仅是行为 还可以控制处理流程
    */

    class EGG_ENGINE_EXPORT ComponentBase
        :public Node
    {
        friend class SceneNode;

    public:
        ComponentBase(Node* node =0,const std::string& name = "component")
            :Node(name),mAttachNode(node){}
        virtual ~ComponentBase(){}

        //类型说明
        //virtual std::string getTypeName(){return "component";}
        virtual std::string ObjectClassName() override {return "component";}

        //
        virtual void onAttach() {}
        virtual void onDetach() {}

        //分拣后的消息接口
        virtual void onInput(nv_input* nv) {} //输入消息处理
        virtual void onUpdate(nv_update* nv) {} //计算更新
        virtual void onCull(nv_cull* nv, Eigen::Matrix4f& vm) {} //筛选场景图生成渲染图
        virtual void onRender(nv_render* nv, Eigen::Matrix4f& mv) {} //这个接口除了Camera有用以外并没有实际通途
        virtual void onGUI(nv_gui* nv) {} //渲染完成后专门处理GUI的绘制过程


        //消息分拣
        //   因为组件不一定遵循完整的遍历逻辑，而且有些消息需要进行预处理，所以必须分拣处理。
        virtual void traverse(NodeVisitor& nv);

        Node* getNode(){return mAttachNode.get().get();}

    private:
        egg::observer_ptr<Node> mAttachNode;

        Eigen::Matrix4f calculateModelView(NodeVisitor& nv);
    };



    //这个类型应该是个资源 暂时占一个类型
    //class NIUBI_SG_EXPORT Line
    //    :public ComponentBase
    //{
    //public:
    //    Line(){}
    //    virtual ~Line(){}

    //    virtual std::string ObjectClassName() override {return "line";}

    //    virtual void onUpdate(View::nv_update* nv) override {}
    //    virtual void onCull(View::nv_cull* nv) override {}
    //    virtual void onRender(View::nv_render* nv ,vmml::mat4f& mv) override {}

    //};


}//namespace egg


#endif//NIUBI_SG_COMPONENTBASE_H
