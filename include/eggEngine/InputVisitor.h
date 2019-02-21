#ifndef _EGG_INPUTVISITOR_H_
#define _EGG_INPUTVISITOR_H_

#include <eggEngine/Export.h>

#include <eggRuntime/NodeVisitor.h>
#include <eggEngine/Window.h>

namespace egg
{

    class nv_input
        :public NodeVisitor
    {
    public:
        nv_input() {}
        virtual ~nv_input() { }
        virtual void apply(Node& node) override { traverse(node); }

        WindowEvent mWindowEvent;
    };


}//namespace egg

#endif // _EGG_INPUTVISITOR_H_
