#ifndef NIUBI_SG_SCENENODE_H
#define NIUBI_SG_SCENENODE_H

//#include <nbBase/Referenced>
#include <eggRuntime/Node.h>
#include <eggEngine/Component.h>

namespace egg
{

    /*
    * 场景节点
    */

    class EGG_ENGINE_EXPORT SceneNode
        :public Node
    {
    public:
        SceneNode(const std::string& name = "scenenode")
            :Node(name) {}
        virtual ~SceneNode() {}

        virtual std::string ObjectClassName() { return "scenenode"; }

        //阻止Component被添加
        virtual void addChild(Node* child) override
        {
            if (!dynamic_cast<ComponentBase*>(child))
            {
                Node::addChild(child);
            }
        }

        //消息分拣
        virtual void traverse(NodeVisitor& nv);

        //添加行为组件
        //template<class T>
        bool addComponent(ComponentBase* component);

        //查询行为组件
        //  必须是ComponentBase继承类
        //  如果包含多个 只返回第一个 并不做错误提示
        template<class T>
        T* getComponent()
        {
            //搜索全部子节点 找到第一个符合要求的就返回
            for (auto it = mComponents.begin(); it != mComponents.end(); it++)
            {
                T* v = dynamic_cast<T*>((*it).get());
                if (v) return v;
            }
            return nullptr;
        }

        //删除相同类型的行为组件
        template<class T>
        void removeComponent(T* component)
        {
            for (auto it = mComponents.begin(); it != mComponents.end(); it++)
            {
                T* v = dynamic_cast<T*>((*it).get());
                if (v)
                {
                    (*it)->onDetach();
                    mComponents.erase(it);
                    return;
                }
            }
        }

        //查询行为组件的数量
        unsigned int getComponentCount() { return mComponents.size(); }

        virtual void save(Path filename, std::ostream& os, Serializer* ser) const override;

        virtual void load(Path filename, std::istream& is, Serializer* ser, ObjectManager* om, Referenced* rmg, bool load_ext)override;

        //临时获取组件列表  以后再修改的优雅点
        std::vector< ref_ptr< ComponentBase > > getComponentList();

    private:

        //功能组分表
        std::set< ref_ptr< ComponentBase > > mComponents;
    };

}//namespace egg

#endif//NIUBI_SG_SCENENODE_H
