#ifndef _EGG_MODULE_H_
#define _EGG_MODULE_H_

#include<egg/Export.h>
#include<egg/Object.h>

namespace egg
{

    class EGG_RUNTIME_EXPORT Module
        :public Object
    {
    public:
        typedef bool(*pfunType)();

        Module(pfunType install_func = 0, pfunType uninstall_func = 0);
        virtual ~Module();

    private:

        pfunType eggModuleInstall_func;
        pfunType eggModuleUninstall_func;

    };

#define STRINGIFIED(T) #T

#if defined(EGG_LIBRARY_STATIC)

#define USE_MODULE(NAME) \
bool eggModuleInstall_##NAME();\
class eggModule_##NAME { public: eggModule_##NAME() { eggModuleInstall_##NAME(); }};\
eggModule_##NAME eggModuleInstall_proxy_##NAME;

#define INSTALL_MODULE(NAME) \
bool eggModuleInstall_##NAME()

#define UNINSTALL_MODULE(NAME) \
bool eggModuleUninstall_##NAME()

#else

#define USE_MODULE(NAME)

#define INSTALL_MODULE(NAME) \
extern "C" EGG_MODULE_EXPORT bool eggModuleInstall_##NAME()

#define UNINSTALL_MODULE(NAME) \
extern "C" EGG_MODULE_EXPORT bool eggModuleUninstall_##NAME()

#endif

}//namespace egg

#endif // !_EGG_MODULE_H_
