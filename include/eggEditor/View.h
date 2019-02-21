#ifndef _EGG_View_H_
#define _EGG_View_H_

#include <eggEditor/Export.h>
#include <eggRuntime/Object.h>

namespace egg
{
    class EGG_EDITOR_EXPORT View
        :public Object
    {

    public:
        View();
        virtual ~View();

    };
}

#endif // !_EGG_View_H_

