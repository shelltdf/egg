
#include <eggEngine/Component.h>

#include <eggEngine/SceneNode.h>
#include <eggEngine/Components/Transfrom3D.h>
#include <eggEngine/RenderWindow.h>


using namespace egg;


Eigen::Matrix4f ComponentBase::calculateModelView(NodeVisitor& nv)
{
    Eigen::Matrix4f mv;
    mv.setIdentity();

    //计算当前mv
    if (nv.np.path.size() >= 2)
    {
        Node* n = *(nv.np.path.rbegin() + 1);  //得到自己依附的node
        SceneNode* sn = dynamic_cast<SceneNode*>(n);
        if (sn)
        {
            Transfrom3D* t = sn->getComponent<Transfrom3D>();
            if (t)
            {
                mv = t->getWorldViewMatrix();
            }
        }
    }

    return mv;
}

void ComponentBase::traverse(NodeVisitor& nv)
{
    if (nv.as<nv_update>())
    {
        onUpdate(nv.as<nv_update>());
        return;//处理后打断
    }
    else if (nv.as<nv_cull>())
    {
        Eigen::Matrix4f mv = calculateModelView(nv);
        //nv_cull* cull = nv.as<nv_cull>();

        onCull(nv.as<nv_cull>(), mv);
        return;//处理后打断
    }
    else if (nv.as<nv_render>())
    {
        Eigen::Matrix4f mv = calculateModelView(nv);
        //nv_render* render = nv.as<nv_render>();

        onRender(nv.as<nv_render>(), mv);
        return;//处理后打断
    }
    else if (nv.as<nv_input>())
    {
        onInput(nv.as<nv_input>());
        return;//处理后打断
    }
    else if (nv.as<nv_gui>())
    {
        onGUI(nv.as<nv_gui>());
        return;//处理后打断
    }

    //没有被拦截的消息 继续遍历
    Node::traverse(nv);
}
