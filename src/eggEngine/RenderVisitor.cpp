
#include <eggEngine/RenderVisitor.h>

#include <eggEngine/Components/Transfrom3D.h>
#include <eggEngine/SceneNode.h>

using namespace egg;


void nv_update::apply(Node& node)
{
    //这里单独处理Transfrom3D对象 以后尽量修改掉 不能让View处理一个个案

    //计算当前矩阵
    Eigen::Matrix4f saveCurrentViewMatrix = this->currentViewMatrix;
    SceneNode* sn = node.as<SceneNode>();
    Transfrom3D* t = 0;
    if (sn)
    {
        t = sn->getComponent<Transfrom3D>();
        if (t)
        {
            t->setWorldViewMatrix(this->currentViewMatrix * t->getLocalMatrix());
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
