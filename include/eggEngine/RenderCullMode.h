#ifndef _EGG_RenderCullMode_H_
#define _EGG_RenderCullMode_H_

#include <eggEngine/Export.h>
#include <egg/Object.h>

namespace egg
{
    class EGG_ENGINE_EXPORT RenderCullData
        :public Object
    {
    public:
        RenderCullData() {}
        ~RenderCullData() {}

    private:

    };

    class EGG_ENGINE_EXPORT RenderCullMode
        :public Object
    {
    public:
        RenderCullMode();
        ~RenderCullMode();

    private:

    };

}//namespace egg

#endif //_EGG_RenderCullMode_H_
