
#include <eggApplication/Export.h>

#include <eggRuntime/Runtime.h>
#include <eggRuntime/Module.h>
#include <eggEngine/Engine.h>
#include <eggEngine/RenderWindow.h>


namespace egg
{
    struct ApplicationOption
    {
        std::string window_postfix = "win32";
        std::string graphicscontext_postfix = "wgl";
        std::string renderer_postfix = "glcore";
        std::string window_title = "EGG Engine";
        int window_width = 1280;
        int window_height = 720;
        int window_bits = 8;
        std::string project_dir = "../default_project/";
    };

    class EGG_APPLICITION_EXPORT Application
        :public Object
    {
    public:
        Application();
        ~Application();

        virtual bool init(const ApplicationOption& option);

        virtual bool step(double dt_second);

        virtual void run();

        virtual bool shutdown();

        egg::RenderWindow* getRenderWindow() { return mRenderWindow.get(); }

        bool isInited() { return mIsInited; }
        bool isDone() { return mIsDone; }

    protected:

        bool mIsInited = false;
        bool mIsDone = false;

        egg::ref_ptr<egg::Runtime> mRuntime;
        egg::ref_ptr<egg::Engine> mEngine;
        egg::ref_ptr<egg::RenderWindow> mRenderWindow;

    };



    class EGG_APPLICITION_EXPORT ApplicationFactory
        :public Factory<Application>
    {
    public:
        ApplicationFactory() {}
        ~ApplicationFactory() {}
        DECLARE_SINGLETON(ApplicationFactory)
    };


}//namespace egg
