
#ifndef _NSG_CORE_NODE_H_
#define _NSG_CORE_NODE_H_

/*
* 根据visitor设计模式 加入跟节点路径有关的设计方案
*       支持多child 多parent
*       支持node path
*       支持node自定义自己的遍历流程方向和形式 traverse
*
*
*             Node                     visitor
*   push path  |                          |
*   accept->   |                          |
*              |      apply(node type)    |
*              |      ----------->        | ---|
*              |                          | <--| traverse 根据类型选择上升下降
*              |      <-------------      |
*              |       traverse ascend    |
*   pop path   |        子类决定行为       |
*
*   以上是OSG中的模式 因为OSG考虑到node是不断被继承的 所以apply和traverse 是可以重载的
*   但是我们现在设计中Node是没有继承类的 所有能力差异是has关系 所以在visitor只需要一个apply接口去通知对应的capability
*/

//#include <vector>
#include <string>
#include <set>

#include <egg/Object.h>

namespace egg
{

    class NodeVisitor;

    class EGG_RUNTIME_EXPORT Node
        :public Object
    {
    public:
        Node(const std::string& name = "node")
            :Object(name){ /*mName = name;*/ }
        virtual ~Node(){}

        virtual std::string ObjectClassName(){return "node";}

        //void setName(const std::string& name){ mName = name; }
        //std::string getName(){ return mName; }

        virtual void addChild(Node* child)
        {
            mChildern.insert(child);
            child->addParent(this);
        }
        void removeChild(Node* child)
        {
            auto it = mChildern.find(child);
            if (it != mChildern.end())
            {
                (*it)->removeParent(this);
                mChildern.erase(it);
            }
        }
        Node* getChild(unsigned int i)
        {
            unsigned int t = 0;
            for (auto it = mChildern.begin(); it != mChildern.end(); ++it)
            {
                if (t == i) { return (*it).get(); }
                t++;
            }
            return 0;
        }
        unsigned int getNumChild() const { return static_cast<unsigned int>(mChildern.size()); }

        Node* getParent(unsigned int i)
        {
            unsigned int t = 0;
            for (auto it = mParents.begin(); it != mParents.end(); ++it)
            {
                if (t == i) { return (*it).get().get(); }
                t++;
            }
            return 0;
        }
        unsigned int getNumParents() const { return static_cast<unsigned int>(mParents.size()); }


        void accept(NodeVisitor& nv);   //从自己开始
        void ascend(NodeVisitor& nv);   //向上
        virtual void traverse(NodeVisitor& nv); //向下


        //类型操作
        template<class T>
        T* as()
        {
            T* v = dynamic_cast<T*>(this);
            return v;
        }

        virtual void save( Path filename , std::ostream& os , Serializer* ser ) const override
        {
            Object::save(filename,os,ser);

            //ser->writeString(os,mName); //名字

            ser->writeNumber( os ,(unsigned int)mChildern.size() ); //数量
            for (auto it = mChildern.begin(); it != mChildern.end(); it++)
            {
                ser->writeString( os , (*it)->ObjectClassName()); //类型

                (*it)->save( filename , os , ser );  //递归子节点
            }
        }

        virtual void load( Path filename , std::istream& is , Serializer* ser , ObjectManager* om 
            , Referenced* rmg, bool load_ext) override
        {
            Object::load(filename,is,ser,om,rmg,load_ext);

            //ser->readString(is,mName); //名字

            unsigned int count = 0;
            ser->readNumber(is,count);  //数量

            for (unsigned int i = 0; i < count; i++)
            {
                std::string classname;
                ser->readString( is , classname); //类型

                Object* obj = om->newObject(classname);
                Node* node = dynamic_cast<Node*>(obj);
                node->load(filename,is,ser,om,rmg,load_ext);   //读子节点

                mChildern.insert(node);
            }
        }

    private:

        void addParent(Node* parent)
        {
            observer_ptr<Node> wp(parent);
            mParents.insert(wp);
        }
        void removeParent(Node* parent)
        {
            auto it = mParents.find(parent);
            if (it != mParents.end())
            {
                mParents.erase(it);
            }
        }

        //std::string mName;
        std::set< observer_ptr<Node> > mParents;
        std::set< ref_ptr<Node> > mChildern;
        //std::vector< Capability* > mCapability;

    };

}//namespace nbSG

#endif //_NSG_CORE_NODE_H_
