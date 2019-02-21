
#include <eggEngine/Window.h>
#include <eggEngine/GraphicsContext.h>

using namespace egg;

//IMPLEMENT_FACTORY(Window);
IMPLEMENT_SINGLETON(WindowFactory)

void Window::frame(double dt)
{
    //如果当前队列中还存在消息就认为是没有被处理的垃圾消息直接清空
    //this->cleanWindowEventQueue();

    //更新窗口收集消息
    this->updateWindow();

    //辅助更新
    if (isShow())
    {
        //发送消息
        if (mGraphicsContext)mGraphicsContext->frame();
        //mWindowEventCallback->onFrame(dt);
        //mWindowEventCallback->onDraw(dt);
    }

}

