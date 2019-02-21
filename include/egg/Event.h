#ifndef _egg_Event_H
#define _egg_Event_H_

#include <egg/Export.h>
#include <egg/ref_ptr.h>

namespace egg
{
    class EGG_RUNTIME_EXPORT Event
        :public Referenced
    {
    public:
        Event(){}
        ~Event(){}
    };
}
#endif //_egg_Event_H_

