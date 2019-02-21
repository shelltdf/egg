#ifndef _CLR_FunctionHandle_H_
#define _CLR_FunctionHandle_H_

#include <egg/Export.h>

#include <egg/ref_ptr.h>

namespace egg
{
    class EGG_RUNTIME_EXPORT FunctionHandle
        :public Referenced
    {
    public:
        FunctionHandle();
        virtual ~FunctionHandle();

    private:

    };

}//namespace egg

#endif // !_CLR_FunctionHandle_H_
