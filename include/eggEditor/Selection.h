#ifndef _EGG_Selection_H_
#define _EGG_Selection_H_

#include <eggEditor/Export.h>
#include <eggRuntime/Object.h>

namespace egg
{
    class EGG_EDITOR_EXPORT Selection
        :public Object
    {

    public:
        Selection();
        virtual ~Selection();

    };
}

#endif // !_EGG_Selection_H_

