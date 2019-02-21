#ifndef _EGG_RUNTIME_H_
#define _EGG_RUNTIME_H_

#include <egg/Export.h>

#include <map>
#include <string>

#include <egg/Singleton.h>
#include <egg/Module.h>
//#include <eggRuntime/ref_ptr.h>


namespace egg
{

    class EGG_RUNTIME_EXPORT Runtime
        :public Object
    {
    public:
        Runtime();
        ~Runtime();

        DECLARE_SINGLETON(Runtime)

        /*
        * 请求加载模块
        * 不加载name重复的模块。
        * postfix作为重复模块的提示信息，但是任然不重复。
        */
        Module* requestModule(std::string name, std::string postfix);

        /*
        * 获取模块
        * 一个name只可能存在一个被加载的模块。
        */
        Module* getModule(std::string name);

        /*
        * 根据类型查询模块
        * 一个类型只可能被加载一次。
        */
        template<class T>
        Module* getModule()
        {
            return 0;
        }

    private:

        std::map<std::string, egg::ref_ptr<Module> > mModuleMap;

        Module* loadLibrary(std::string name, std::string postfix);

    };

}//namespace egg

#endif // !_EGG_RUNTIME_H_
