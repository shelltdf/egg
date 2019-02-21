
#include <eggApplication/Application.h>
//#include <eggRuntime/Runtime.h>
//#include <eggEngine/Engine.h>
//#include <eggEditor/Editor.h>


using namespace egg;

IMPLEMENT_SINGLETON(ApplicationFactory)


USE_MODULE(Window_win32)
USE_MODULE(GraphicsContext_wgl)
USE_MODULE(Renderer_glcore)

Application::Application()
{
    
}

Application::~Application()
{
}

bool Application::init(const ApplicationOption& option)
{
    //init core object
    mRuntime = egg::Runtime::getInstance(true);
    mEngine = egg::Engine::getInstance(true);
    //egg::Editor* editor = egg::Editor::getInstance();

    //init module
    //egg::Module* module_Platform = mRuntime->requestModule("Platform", "win32");
    egg::Module* module_Window = mRuntime->requestModule("Window", "win32");
    egg::Module* module_GraphicsContext = mRuntime->requestModule("GraphicsContext", "wgl");
    egg::Module* module_Renderer = mRuntime->requestModule("Renderer", "glcore");

    //create window gc and renderer
    egg::Window* win = egg::WindowFactory::getInstance(true)->create("Window_win32");
    egg::GraphicsContext* gc = egg::GraphicsContextFactory::getInstance(true)->create("GraphicsContext_wgl");
    egg::Renderer* renderer = egg::RendererFactory::getInstance(true)->create("Renderer_glcore");
    //win->setEventCallback(new egg::RenderWindowEventCallback());
    win->createWindow("EGG Engine", 1280, 720, 8, false, gc, renderer);

    //render window
    mRenderWindow = new egg::RenderWindow(win, gc, renderer);


    //Object
    egg::ObjectManager* om = egg::ObjectManager::getInstance(true);

    //VFS
    egg::FileSystem* vfs = egg::FileSystem::getInstance(true);
    egg::niubi_entry_arg arg;
    vfs->setArg(arg);

    //resource manager
    egg::ResourceManager* rm = egg::ResourceManager::getInstance(true);
    rm->init(arg, true);

    //默认资源地址
    //  在android下为 assetmanager 的根目录
    //  在IOS下为 application 目录
    //  在pc上编辑模式为任何地址的 project 目录， 发布模式为当前bin的同一级目录。编辑模式下为可写入。
    rm->registerArchive("asset", "../default_project/", false);
    rm->updateUniformAssetPath();


    mIsInited = true;
    return true;
}

bool Application::step(double dt_second)
{
    if (!mIsInited) return false;

    mIsDone = !mRenderWindow->runStep(dt_second);
    return mIsDone;
}

void Application::run()
{
    while (!mIsDone)
    {
        this->step(1.0 / 60.0);
    }
    this->shutdown();
}

bool Application::shutdown()
{
    if (!mIsInited) return false;

    mRenderWindow = 0;
    mEngine = 0;
    mRuntime = 0;

    mIsInited = false;
    return true;
}
