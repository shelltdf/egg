#ifndef _egg_EventQueue_H
#define _egg_EventQueue_H_

#include <egg/Export.h>
#include <egg/ref_ptr.h>

namespace egg
{
    class EGG_RUNTIME_EXPORT EventQueue
        :public Referenced
    {
    public:
        EventQueue(){}
        ~EventQueue(){}
    };
}
#endif //_egg_EventQueue_H_

