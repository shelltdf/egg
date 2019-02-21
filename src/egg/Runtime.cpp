#include <egg/Runtime.h>

#if _WIN32
#include <windows.h>
#endif //_WIN32


using namespace egg;

IMPLEMENT_SINGLETON(Runtime)

Runtime::Runtime()
{
}

Runtime::~Runtime()
{
}

Module* Runtime::requestModule(std::string name, std::string postfix)
{
    auto it = mModuleMap.find(name);
    if (it != mModuleMap.end()) return it->second.get();

    Module* module = loadLibrary(name, postfix);
    if (!module)
    {
        printf("load module error!\n");
    }

    std::string reg_name = name;
    if (postfix.size() > 0)
    {
        reg_name = reg_name + "_" + postfix;
    }

    mModuleMap[reg_name] = module;

    return module;
}

Module* Runtime::getModule(std::string name)
{
    return 0;
}

#if _WIN32
Module* Runtime::loadLibrary(std::string name, std::string postfix)
{
#if _DEBUG
    std::string filepath = /*"Module_" +*/ name + "_" + postfix + CMAKE_DEBUG_POSTFIX + ".dll";
#else
    std::string filepath = /*"Module_" +*/ name + "_" + postfix + ".dll";
#endif
    std::string install_name = "eggModuleInstall_" + name + "_" + postfix;
    std::string uninstall_name = "eggModuleUninstall_" + name + "_" + postfix;

    HMODULE dll = ::LoadLibraryA(filepath.c_str());
    if (dll == NULL) return 0;

    Module::pfunType pf_install = (Module::pfunType)GetProcAddress(dll, install_name.c_str());
    Module::pfunType pf_uninstall = (Module::pfunType)GetProcAddress(dll, uninstall_name.c_str());
    if (pf_install == NULL) return 0;
    if (pf_uninstall == NULL) return 0;
    

    Module* module = new Module(pf_install, pf_uninstall);

    return module;
}
#endif //_WIN32

