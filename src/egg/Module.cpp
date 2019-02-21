#include <egg/Module.h>

using namespace egg;

Module::Module(pfunType install_func, pfunType uninstall_func)
    :eggModuleInstall_func(install_func)
    , eggModuleUninstall_func(uninstall_func)
{
    if (eggModuleInstall_func)
    {
        eggModuleInstall_func();
    }
}

Module::~Module()
{
    if (eggModuleUninstall_func)
    {
        eggModuleUninstall_func();
    }
}


