
#ifndef _NSG_CORE_NODE_VISITOR_H_
#define _NSG_CORE_NODE_VISITOR_H_

#include <egg/Node.h>
#include <egg/NodePath.h>

namespace egg
{

    class EGG_RUNTIME_EXPORT NodeVisitor
        :public Object
    {
    public:
        NodeVisitor() :isUP(false){}
        virtual ~NodeVisitor(){}

        //子类型继承接口
        virtual void apply(Node& node)
        {
            traverse(node);
        }

        //默认遍历过程
        inline void traverse(Node& node)
        {
            if (isUP)
            {
                node.ascend(*this);
            }
            else
            {
                node.traverse(*this);
            }
        }

        //类型操作
        template<class T>
        T* as()
        {
            T* v = dynamic_cast<T*>(this);
            return v;
        }


        bool isUP; //便利方向 true向上 false向下

        NodePath np;
    };

}//namespace nbSG

#endif //_NSG_CORE_NODE_VISITOR_H_
