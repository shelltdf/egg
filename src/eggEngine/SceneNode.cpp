
#include <eggEngine/SceneNode.h>

#include <eggEngine/RenderWindow.h>

using namespace egg;

void SceneNode::traverse(NodeVisitor& nv)
{
    for (auto it = mComponents.begin(); it != mComponents.end(); it++)
    {
        (*it)->accept(nv);
    }

    //遍历
    Node::traverse(nv);

    if (nv.as<nv_update>())
    {
        //这里是SceneNode更新完成的时间点
    }
}

bool SceneNode::addComponent(ComponentBase* component)
{
    //检查避免重复类型
    for (auto it = mComponents.begin(); it != mComponents.end(); it++)
    {
        std::string a = (*it)->ObjectClassName();
        std::string b = component->ObjectClassName();

        if (a == b)
        {
            return false;
        }
    }

    //如果没有附加给别的节点 就正常添加
    if (!component->mAttachNode.get().valid())
    {
        component->mAttachNode = this;
        mComponents.insert(component);
        component->onAttach();
        return true;
    }

    return false;
}

void SceneNode::save(Path filename, std::ostream& os, Serializer* ser) const
{
    Node::save(filename, os, ser);

    //写入全部功能组件
    ser->writeNumber(os, (unsigned int)mComponents.size());   //数量
    for (auto it = mComponents.begin(); it != mComponents.end(); it++)
    {
        ser->writeString(os, (*it)->ObjectClassName());//类型

        (*it)->save(filename, os, ser);  //内容
    }
}

void SceneNode::load(Path filename, std::istream& is, Serializer* ser, ObjectManager* om, Referenced* rmg, bool load_ext)
{
    Node::load(filename, is, ser, om, rmg, load_ext);

    //读取所有功能组件
    unsigned int com_num = 0;
    ser->readNumber(is, com_num); //读数量
    for (unsigned int i = 0; i < com_num; i++)
    {
        std::string com_type_name;
        ser->readString(is, com_type_name);//类型

        Object* obj = om->newObject(com_type_name);
        obj->load(filename, is, ser, om, rmg, load_ext);      //内容

        ComponentBase* cb = dynamic_cast<ComponentBase*>(obj);
        //mComponents.insert(cb);
        addComponent(cb);
    }
}

//临时获取组件列表  以后再修改的优雅点
std::vector< ref_ptr< ComponentBase > > SceneNode::getComponentList()
{
    std::vector< ref_ptr< ComponentBase > > ret;
    for (auto it = mComponents.begin(); it != mComponents.end(); it++)
    {
        ret.push_back(*it);
    }
    return ret;
}


