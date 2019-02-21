
#include <egg/Node.h>

#include <egg/NodeVisitor.h>

using namespace egg;

void Node::accept(NodeVisitor& nv)
{
    //push node path
    nv.np.path.push_back(this);

    //apply visitor
    nv.apply(*this);

    //这里还是让用户自己决定
    //nv.traverse(*this);

    //pop node path
    nv.np.path.pop_back();
}

void Node::ascend(NodeVisitor& nv)
{
    for (auto it = mParents.begin(); it != mParents.end(); ++it)
    {
        (*it).get()->accept(nv);
    }
}

void Node::traverse(NodeVisitor& nv)
{
    //foreach child
    for (auto it = mChildern.begin(); it != mChildern.end(); ++it)
    {
        (*it)->accept(nv);
    }
}
