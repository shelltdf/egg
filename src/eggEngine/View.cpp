
#include <eggEngine/View.h>
#if 0
#include <eggEngine/Component.h>
#include <eggEngine/SceneNode.h>
#include <eggEngine/Components/Transfrom3D.h>

using namespace egg;

void View::update(float dt)
{
    //执行子节点
    ref_ptr<nv_update> nv = new nv_update();
    nv->dt = dt;
    nv->currentViewMatrix = this->viewMatrix;
    this->accept(*nv);
}

void View::cull(float dt)
{
    //执行子节点
    ref_ptr<nv_cull> nv = new nv_cull();
    nv->dt = dt;
    this->accept(*nv);
}

void View::render(Renderer* render,unsigned int context,float dt/*, vmml::mat4f& projactiveMatrix, vmml::mat4f& viewMatrix*/)
{
    //设置当前属性
    //this->projactiveMatrix = projactiveMatrix;
    //this->viewMatrix = viewMatrix;

    //清除背景
    render->setViewport((int)viewPort.x(), (int)viewPort.y(), (int)viewPort.z(), (int)viewPort.w());
    render->clear(
        Renderer::CM_COLOR | Renderer::CM_ZBUFFER
        , 0.2f, 0.2f, 0.2f, 1.0f
        , 1.0f, 0.0f
        );

    //执行子节点
    ref_ptr<nv_render> nv = new nv_render();
    nv->dt = dt;
    nv->projactiveMatrix = this->projactiveMatrix;
    nv->render = render;
    nv->context = context;
    this->accept(*nv);
}

void View::nv_update::apply(Node& node)
{
    //这里单独处理Transfrom3D对象 以后尽量修改掉 不能让View处理一个个案

    //计算当前矩阵
    vmml::mat4f saveCurrentViewMatrix = this->currentViewMatrix;
    SceneNode* sn = node.as<SceneNode>();
    Transfrom3D* t = 0;
    if(sn)
    {
        t = sn->getComponent<Transfrom3D>();
        if (t)
        {
            t->setWorldViewMatrix( this->currentViewMatrix * t->getLocalMatrix() );
            this->currentViewMatrix = t->getWorldViewMatrix();
        }
    }

    //遍历子节点
    traverse(node);

    //还原矩阵
    if (t)
    {
        Transfrom3D* t = node.as<Transfrom3D>();
        this->currentViewMatrix = saveCurrentViewMatrix;
    }
}

#endif
