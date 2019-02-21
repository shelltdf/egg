
#include <thread>
#include <chrono>

#include <eggApplication/Application.h>

//#include <eggRuntime/Runtime.h>
//#include <eggEngine/Engine.h>
//#include <eggEngine/RenderWindow.h>

//#include <eggEngine/Components/Transfrom3D.h>
//#include <eggEngine/Components/MeshRender.h>



int main(int argc, char* argv[])
{
    std::string app_name = "test";
    if (argc > 1) { app_name = argv[1]; }
    std::string app_name2 = "Application_" + app_name;

    egg::ref_ptr<egg::Runtime> runtime = egg::Runtime::getInstance(true);
    egg::ref_ptr<egg::Module> module = runtime->requestModule("Application", app_name);
    egg::ref_ptr<egg::Application> app = egg::ApplicationFactory::getInstance(true)->create(app_name2);

    egg::ApplicationOption option;
    app->init(option);


    while (!app->isDone())
    {
        app->step(1.0 / 60.0);
    }

    app->shutdown();

    app = 0;
    module = 0;
    runtime = 0;

    return 0;
}

