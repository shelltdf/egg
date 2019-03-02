#ifndef _NSG_DefaultSetting_H_
#define _NSG_DefaultSetting_H_

#include <egg/DataType.h>
#include <egg/Object.h>
#include <egg/Singleton.h>

namespace egg
{
    class DefaultSetting
        :public Object
    {
    public:
        DefaultSetting();
        virtual ~DefaultSetting();

        DECLARE_SINGLETON(DefaultSetting)

        //error material

        //default material

        //default color texture

        //default nromal texture

        //icon object

        //IMGUI

        //FONT

    private:

    };

}//namespace egg

#endif //_NSG_DefaultSetting_H_
