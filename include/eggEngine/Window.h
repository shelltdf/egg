#ifndef _EGG_LAUNCHER_H_
#define _EGG_LAUNCHER_H_

#include <eggEngine/Export.h>

#include <string>
#include <queue>
#include <queue>

#include <egg/Object.h>
#include <egg/Singleton.h>
#include <egg/Factory.h>
#include <egg/DataType.h>


namespace egg
{

	class GraphicsContext;
    class Renderer;
#if 0
    class Window;
    

    class EGG_ENGINE_EXPORT WindowEventCallback
        :public Object
    {
    public:

        WindowEventCallback() {}
        virtual ~WindowEventCallback() {}

        virtual void onKey(int keycode, int keystate, int x, int y) = 0;

        //输入鼠标消息
        //  type        0 move 1 click
        //  moustbutton 1 左键 2 右键 3 中键
        //  moustbuttonstate 0 up 1 down
        //  x y 像素值 左上角原点
        virtual void onMouse(int type, int moustbutton, int moustbuttonstate, int x, int y) = 0;

        //输入多点消息
        //  message     0 null 1 add cursor 2 move cursor 3 delete cursor 
        //  cursor_id   从0开始 
        //  x y         左上角百分比
        virtual void onTouch(unsigned int message, unsigned int cursor_id, float x, float y) = 0;

        virtual void onSize(int w, int h) = 0;

        //virtual void onFrame(float dt) = 0;
        //virtual void onDraw(float dt) = 0;

        Window* mWindow;

    protected:
    private:
    };
#endif

    enum WindowEventType
    {
        WET_NULL
        , WET_MOUSE
        , WET_KEY
        , WET_TOUCH
        , WET_SIZE
    };

    enum KeyState
    {
        KS_NULL
        , KS_UP
        , KS_DOWN
    };

    enum MouseType
    {
        MT_Null
        , MT_Move
        , MT_Click
    };

    enum MouseButton
    {
        MB_Null
        , MB_Left
        , MB_Middle
        , MB_Right
    };

    enum KeyCode //虚拟code
    {
        KC_Null
        ,KC_1
    };

    enum TouchType
    {
        TT_Null
        , TT_AddCursor
        , TT_MoveCursor
        , TT_DeleteCursor
    };


    //窗口消息事件
    class EGG_ENGINE_EXPORT WindowEvent
        //:public Object
    {
    public:

        WindowEvent() {}
        virtual ~WindowEvent() {}

        WindowEventType mWindowEventType = WET_NULL;

        KeyCode mKeyCode = KC_Null;
        KeyState mKeyState = KS_NULL; //key and mouse
        
        MouseType mMouseType = MT_Null;
        MouseButton mMouseButton = MB_Null;

        TouchType mTouchType = TT_Null;
        unsigned int mCursorID = 0;

        float x = 0;
        float y = 0;
        float z = 0;
        float w = 0;
    };

#if 0
    class EGG_ENGINE_EXPORT WindowEventQueue
        :public Object
    {
    public:

        WindowEventQueue() {}
        virtual ~WindowEventQueue() {}

        //键盘输入
        virtual void onKey(KeyCode keycode, KeyState keystate, int x, int y)
        {
            WindowEvent we;
            we.mWindowEventType = WET_KEY;
            we.x = x;
            we.y = y;
            we.mKeyCode = keycode;
            we.mKeyState = keystate;
            mWindowEventQueue.push(we);
        }

        //输入鼠标消息
        //  type        0 move 1 click
        //  moustbutton 1 左键 2 右键 3 中键
        //  moustbuttonstate 0 up 1 down
        //  x y 像素值 左上角原点
        virtual void onMouse(MouseType type
            , MouseButton moustbutton, KeyState moustbuttonstate, int x, int y)
        {
            WindowEvent we;
            we.mWindowEventType = WET_MOUSE;
            we.x = x;
            we.y = y;
            we.mMouseType = type;
            we.mMouseButton = moustbutton;
            we.mKeyState = moustbuttonstate;
            mWindowEventQueue.push(we);
        }

        //输入多点消息
        //  message     0 null 1 add cursor 2 move cursor 3 delete cursor 
        //  cursor_id   从0开始 
        //  x y         左上角百分比
        virtual void onTouch(TouchType message, unsigned int cursor_id, float x, float y)
        {
            WindowEvent we;
            we.mWindowEventType = WET_TOUCH;
            we.x = x;
            we.y = y;
            we.mTouchType = message;
            we.mCursorID = cursor_id;
            mWindowEventQueue.push(we);
        }


        //窗口位置调整
        //   左上角原点 像素单位
        virtual void onSize(int x, int y, int width, int height)
        {
            WindowEvent we;
            we.mWindowEventType = WET_SIZE;
            we.x = x;
            we.y = y;
            we.z = width;
            we.w = height;
            mWindowEventQueue.push(we);
        }

        std::queue<WindowEvent>& getWindowEventQueue()
        {
            return mWindowEventQueue;
        }

    private:

        std::queue<WindowEvent> mWindowEventQueue;
    };
#endif


    class EGG_ENGINE_EXPORT Window
		:public Object
	{

	public:

        Window()
            :mDone(false), mGraphicsContext(0)
            //, mWindowEventCallback(0)
            //, mGloble(0)
            ,mDisplay(0){}
		virtual ~Window(){}

        //DECLARE_FACTORY(Window);

		//void setEventCallback(WindowEventCallback* cb)
		//{
  //          mWindowEventCallback = cb;
  //          mWindowEventCallback->mWindow = this;
		//}

	public:

		void frame(double dt);

		virtual bool isShow() = 0;

		//virtual void sleep(unsigned int ms) = 0;
		//virtual std::string getDataPath() = 0;

	public:

        //建立窗口
        // 指定Windowed 指定fullscreen 两个参数
        virtual bool createWindow(const char* title, int width, int height, int bits, bool fullscreen
            , GraphicsContext* gc = 0, Renderer* renderer = 0) = 0;
        virtual bool destroyWindow() = 0;

    protected:

		//void setGraphicsContext(GraphicsContext* gc){mGraphicsContext = gc;}
		GraphicsContext* mGraphicsContext;//gc对象弱关联

		//void setRenderer(Renderer* renderer){ mRenderer = renderer; }
		//Renderer* mRenderer; //renderer对象弱关联

		//void setGloble(void* g){ mGloble = g; }
		//void* mGloble; //全局对象弱关联

	protected:

		//操作系统级别的更新接口
		virtual bool updateWindow() = 0;

	public:

		//主循环标记
		bool mDone;

		//主程序ID 貌似只有windows有这个定义
        HANDLE_TYPE mInstance;
        
        //显示服务器连接 x11专用 记录Display*类型
        HANDLE_TYPE mDisplay;

		//主窗口标识符 根据系统不同需要强制转换成不同类型
        // windows窗口是一个句柄 类似uint
        // x11窗口是一个 ulong
        HANDLE_TYPE mWinID;

		//独占全屏标记
		bool mFullscreen;

		//消息回调
        //WindowEventCallback* mWindowEventCallback;

	protected:


        //输入相关
    public:
        //键盘输入
        virtual void onKey(KeyCode keycode, KeyState keystate, int x, int y)
        {
            WindowEvent we;
            we.mWindowEventType = WET_KEY;
            we.x = x;
            we.y = y;
            we.mKeyCode = keycode;
            we.mKeyState = keystate;
            mWindowEventQueue.push(we);
        }

        //输入鼠标消息
        //  type        0 move 1 click
        //  moustbutton 1 左键 2 右键 3 中键
        //  moustbuttonstate 0 up 1 down
        //  x y 像素值 左上角原点
        virtual void onMouse(MouseType type
            , MouseButton moustbutton, KeyState moustbuttonstate, int x, int y)
        {
            WindowEvent we;
            we.mWindowEventType = WET_MOUSE;
            we.x = x;
            we.y = y;
            we.mMouseType = type;
            we.mMouseButton = moustbutton;
            we.mKeyState = moustbuttonstate;
            mWindowEventQueue.push(we);
        }

        //输入多点消息
        //  message     0 null 1 add cursor 2 move cursor 3 delete cursor 
        //  cursor_id   从0开始 
        //  x y         左上角百分比
        virtual void onTouch(TouchType message, unsigned int cursor_id, float x, float y)
        {
            WindowEvent we;
            we.mWindowEventType = WET_TOUCH;
            we.x = x;
            we.y = y;
            we.mTouchType = message;
            we.mCursorID = cursor_id;
            mWindowEventQueue.push(we);
        }


        //窗口位置调整
        //   左上角原点 像素单位
        virtual void onSize(int x, int y, int width, int height)
        {
            WindowEvent we;
            we.mWindowEventType = WET_SIZE;
            we.x = x;
            we.y = y;
            we.z = width;
            we.w = height;
            mWindowEventQueue.push(we);
        }

        std::queue<WindowEvent>& getWindowEventQueue()
        {
            return mWindowEventQueue;
        }

        void cleanWindowEventQueue()
        {
            std::queue<WindowEvent> n;
            return mWindowEventQueue.swap(n);
        }

	private:

        std::queue<WindowEvent> mWindowEventQueue;
	};


    class EGG_ENGINE_EXPORT WindowFactory
        :public Factory<Window>
    {
    public:
        WindowFactory() {}
        ~WindowFactory() {}
        DECLARE_SINGLETON(WindowFactory)
    };


//#include <egg/GraphicsContext.h>

    //void Window::frame(float dt)
    //{
    //    //更新窗口
    //    this->updateWindow();

    //    if (isShow())
    //    {
    //        //发送消息
    //        if (m_gc_plug)m_gc_plug->frame();
    //        m_event_callback->onFrame(dt);
    //        m_event_callback->onDraw(dt);
    //    }
    //}



}//namespace egg


#endif // _EGG_LAUNCHER_H_


