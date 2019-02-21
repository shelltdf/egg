#ifndef _EGG_RENDERWINDOW_H_
#define _EGG_RENDERWINDOW_H_

#include <eggEngine/Export.h>

#include <eggEngine/Window.h>
#include <eggEngine/GraphicsContext.h>
#include <eggEngine/Renderer.h>
#include <eggEngine/SceneNode.h>
#include <eggEngine/Components/Camera.h>
#include <eggEngine/Composer.h>


namespace egg
{

    class EGG_ENGINE_EXPORT RenderWindow
        :public Object
    {
    public:

        RenderWindow(Window* win, GraphicsContext* gc, Renderer* rd);
        virtual ~RenderWindow();

        //
        bool runStep(double dt_second);

        // get node
        egg::SceneNode* getRoot() { return mRoot.get(); }

        //查询默认摄像机
        //egg::SceneNode* getDefaultCamera() { return mDefaultCamera.get(); }

        // composer
        egg::Composer* getComposer() { return mComposer.get(); }
        void setComposer(egg::Composer* composer) { mComposer = composer; }

        //get context id
        unsigned int getContextID() { return mGraphicsContext->m_gcID; }

    private:

        // create render window
        bool init();
        bool shutdown();

        //线程模式
        int mThreadMode = 0;

        //多线程入口
        bool startInputThread();
        bool startRenderThread();

        //单线程入口
        void processInput(double dt_second);
        void processRender(double dt_second);

        //渲染环境对象
        ref_ptr<Window> mWindow = 0;
        ref_ptr<GraphicsContext> mGraphicsContext = 0;
        ref_ptr<Renderer> mRenderer = 0;

        //下面的属性可以抽象到一个View类型中 实现多View

        //内置场景图根节点
        ref_ptr<SceneNode> mRoot = 0;
        ref_ptr<SceneNode> mAuxRoot = 0; //辅助显示对象跟节点

        //默认摄像机节点
        //  作为场景中不可见的一个节点
        //ref_ptr<SceneNode> mDefaultCamera = 0; //废弃了 使用ROOT作为摄像机的SN

        //渲染流程控制对象
        ref_ptr<Composer> mComposer = 0;
    };


#if 0
    class EGG_ENGINE_EXPORT RenderWindowEventCallback
        :public WindowEventCallback
    {
    public:
        RenderWindowEventCallback() {}
        virtual ~RenderWindowEventCallback() {}

        virtual void onKey(int keycode, int keystate, int x, int y);
        virtual void onMouse(int type, int moustbutton, int moustbuttonstate, int x, int y);
        virtual void onTouch(unsigned int message, unsigned int cursor_id, float x, float y);
        virtual void onSize(int w, int h);
        //virtual void onFrame(float dt);
        //virtual void onDraw(float dt);

    private:

    };
#endif




}//namespace egg

#endif // _EGG_RENDERWINDOW_H_
