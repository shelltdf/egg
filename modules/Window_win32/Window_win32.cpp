
#include <eggRuntime/Platform.h>
#include <eggRuntime/Module.h>
#include <eggRuntime/Runtime.h>
#include <eggEngine/Window.h>
#include <eggEngine/GraphicsContext.h>
#include <eggEngine/Renderer.h>
#include <eggEngine/Pass.h>

#include <windows.h>		// Header File For Windows
#include <WinUser.h>


#ifdef _WIN64

//#undef GWL_WNDPROC
//#undef GWL_HINSTANCE
//#undef GWL_HWNDPARENT
#define GWL_USERDATA GWLP_USERDATA

#endif /* _WIN64 */



//bool NBUT::m_keys[256] = {0};
LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
    UINT	uMsg,			// Message For This Window
    WPARAM	wParam,			// Additional Message Information
    LPARAM	lParam)			// Additional Message Information
    ;

class Window_win32
    :public egg::Window
{
public:

    Window_win32()
        :egg::Window()
        ,mIsSupportMultiTouch(false)
    {}
    virtual ~Window_win32(){}

    bool mIsSupportMultiTouch;

    //建立窗口
    // 指定Windowed 指定fullscreen 两个参数
    virtual bool createWindow(const char* title, int width, int height, int bits, bool fullscreen
        , egg::GraphicsContext* gc = 0, egg::Renderer* renderer = 0 ) override
    {
        mGraphicsContext = gc;

        //同步成员变量
        //this->setGraphicsContext(gc_object);
        mFullscreen = fullscreen;

        //初始化定义
        //GLuint		PixelFormat;			// Holds The Results After Searching For A Match
        WNDCLASS	wc;						// Windows Class Structure
        DWORD		dwExStyle;				// Window Extended Style
        DWORD		dwStyle;				// Window Style
        RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
        WindowRect.left = (long)0;			// Set Left Value To 0
        WindowRect.right = (long)width;		// Set Right Value To Requested Width
        WindowRect.top = (long)0;				// Set Top Value To 0
        WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

        //fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

        mInstance = (HANDLE_TYPE)GetModuleHandle(NULL);				// Grab An Instance For Our Window
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
        wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
        wc.cbClsExtra = 0;									// No Extra Window Data
        wc.cbWndExtra = 0;									// No Extra Window Data
        wc.hInstance = (HMODULE)mInstance;							// Set The Instance
        wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
        wc.hbrBackground = NULL;									// No Background Required For GL
        wc.lpszMenuName = NULL;									// We Don't Want A Menu
        wc.lpszClassName = "OpenGL";								// Set The Class Name

        if (!RegisterClass(&wc))									// Attempt To Register The Window Class
        {
            MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
            return FALSE;											// Return FALSE
        }

        if (mFullscreen && mGraphicsContext->isWindowsAuxFull())												// Attempt Fullscreen Mode?
        {
            DEVMODE dmScreenSettings;								// Device Mode
            memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
            dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
            dmScreenSettings.dmPelsWidth = width;				// Selected Screen Width
            dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
            dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
            dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
            if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
            {
                // If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
                if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
                {
                    mFullscreen = FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
                }
                else
                {
                    // Pop Up A Message Box Letting User Know The Program Is Closing.
                    MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
                    return FALSE;									// Return FALSE
                }
            }
        }

        if (mFullscreen && mGraphicsContext->isWindowsAuxFull())												// Are We Still In Fullscreen Mode?
        {
            dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
            dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	// Windows Style
            //ShowCursor(FALSE);										// Hide Mouse Pointer
        }
        else if (mFullscreen)
        {
            dwExStyle = NULL;			// Window Extended Style
            dwStyle = WS_EX_TOPMOST | WS_POPUP;							// Windows Style
        }
        else
        {
            dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
            dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;// Windows Style
        }

        AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

        // Create The Window
        if (!(mWinID = (HANDLE_TYPE)CreateWindowEx(dwExStyle,							// Extended Style For The Window
            "OpenGL",							// Class Name
            title,								// Window Title
            dwStyle,							// Defined Window Style
            //WS_CLIPSIBLINGS |					// Required Window Style
            //WS_CLIPCHILDREN,					// Required Window Style
            CW_USEDEFAULT, CW_USEDEFAULT,								// Window Position
            WindowRect.right - WindowRect.left,	// Calculate Window Width
            WindowRect.bottom - WindowRect.top,	// Calculate Window Height
            NULL,								// No Parent Window
            NULL,								// No Menu
            (HINSTANCE)mInstance,							// Instance
            this)))								// Dont Pass Anything To WM_CREATE
        {
            destroyWindow();								// Reset The Display
            MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
            return FALSE;								// Return FALSE
        }

        //初始化设备
        mGraphicsContext->choosePixelFormat(this
            , width, height
            //,bits,0
            , 24, 0			//色彩 alpha 一直RGB 很费解
            , 0				//AccumBits 积累缓冲 无法打开
            , 24		    //DepthBits怎么设置都是开启 都是float很奇怪
            , 0				//StencilBits大于0就是有效
            , 0				//AuxBuffers永远是开启的
            , 0				//采样有效
            , true			//双缓冲有效
            );
        mGraphicsContext->createGraphaContext(this);
        renderer->init();


        BYTE digitizerStatus = (BYTE)GetSystemMetrics(SM_DIGITIZER);
        if ((digitizerStatus & (0x80 + 0x40)) == 0) //Stack　Ready　+　MultiTouch
        {
        }
        else
        {
            //　Register　the　application　window　for　 receiving　multi-touch　input.
            if (RegisterTouchWindow((HWND)mWinID, 0))
            {
                //MessageBox(hWnd, L"Cannot　 register　application　window　for　touch　input", L"Error", MB_OK);
                //return　FALSE;
                mIsSupportMultiTouch = true;
            }
        }

        //
        SetWindowLongPtr((HWND)mWinID, GWL_USERDATA, (LONG_PTR)this);

        //显示窗口
        ShowWindow((HWND)mWinID, SW_SHOW);						// Show The Window
        SetForegroundWindow((HWND)mWinID);						// Slightly Higher Priority
        SetFocus((HWND)mWinID);									// Sets Keyboard Focus To The Window
        //ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

        //强制resize
        //m_event_callback->onSize(width, height);

        return true;
    }

    virtual bool destroyWindow()
    {
        if (mFullscreen)										// Are We In Fullscreen Mode?
        {
            ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
            ShowCursor(TRUE);								// Show Mouse Pointer
        }

        //销毁设备
        mGraphicsContext->destroyGraphaContext(this);

        if (mWinID && !DestroyWindow((HWND)mWinID))					// Are We Able To Destroy The Window?
        {
            MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
            mWinID = 0;										// Set hWnd To NULL
        }

        if (!UnregisterClass("OpenGL", (HINSTANCE)mInstance))			// Are We Able To Unregister Class
        {
            MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
            mInstance = NULL;									// Set hInstance To NULL
        }

        return true;
    }


    virtual bool isShow()
    {
        return ::IsWindowVisible((HWND)mWinID);
    }

#if 0
    virtual void sleep(unsigned int ms)
    {
        Sleep(ms);
    }

    std::string getDataPath()
    {
        return "../data/";
    }
#endif

    virtual bool isSupportMultiTouch()
    {
        //这里应该测试窗口是否支持多触点输入
        return mIsSupportMultiTouch;
    }

    virtual bool updateWindow()
    {
        MSG		msg;									// Windows Message Structure
        //BOOL	done=FALSE;								// Bool Variable To Exit Loop
        if (!mDone)									// Loop That Runs While done=FALSE
        {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
            {
                if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
                {
                    mDone = TRUE;							// If So done=TRUE
                    destroyWindow();
                }
                else									// If Not, Deal With Window Messages
                {
                    TranslateMessage(&msg);				// Translate The Message
                    DispatchMessage(&msg);				// Dispatch The Message
                    //myWndProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);

                }
            }
            //else										// If There Are No Messages
            //{
            //// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
            //if (active)								// Program Active?
            //{
            //if (m_keys[VK_ESCAPE])				// Was ESC Pressed?
            //{
            //	m_done=TRUE;						// ESC Signalled A Quit
            //}
            //	else								// Not Time To Quit, Update Screen
            //	{
            //		DrawGLScene();					// Draw The Scene
            //		SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
            //	}
            //}

            //if (keys[VK_F1])						// Is F1 Being Pressed?
            //{
            //	keys[VK_F1]=FALSE;					// If So Make Key FALSE
            //	KillGLWindow();						// Kill Our Current Window
            //	fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
            //	// Recreate Our OpenGL Window
            //	if (!CreateGLWindow("NeHe's OpenGL Framework",640,480,16,fullscreen))
            //	{
            //		return 0;						// Quit If Window Was Not Created
            //	}
            //}
            //}
        }

        return true;
    }

protected:
private:
};


void GetTouchPoint(HWND hWnd, const TOUCHINPUT& ti, float& out_x, float& out_y)
{
    //计算像素坐标
    POINT pt;
    pt.x = TOUCH_COORD_TO_PIXEL(ti.x);
    pt.y = TOUCH_COORD_TO_PIXEL(ti.y);
    ScreenToClient(hWnd, &pt);

    //查询窗口size
    RECT rect;
    BOOL b = GetClientRect(hWnd, &rect);

    //计算xy相对百分比
    out_x = float(pt.x) / (rect.right - rect.left);
    out_y = float(pt.y) / (rect.bottom - rect.top);

    //return pt;
}

egg::KeyCode convertKeyCode(WPARAM wParam, LPARAM lParam)
{

    return egg::KeyCode::KC_Null;
}

LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
    UINT	uMsg,			// Message For This Window
    WPARAM	wParam,			// Additional Message Information
    LPARAM	lParam)			// Additional Message Information
{
    switch (uMsg)									// Check For Windows Messages
    {
    case WM_CREATE:
    {
        //SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG)((LPCREATESTRUCT)lParam)->lpCreateParams);
        break;
    }

    case WM_ACTIVATE:							// Watch For Window Activate Message
    {
        if (!HIWORD(wParam))					// Check Minimization State
        {
            //active=TRUE;						// Program Is Active
        }
        else
        {
            //active=FALSE;						// Program Is No Longer Active
        }

        return 0;								// Return To The Message Loop
    }

    case WM_SYSCOMMAND:							// Intercept System Commands
    {
        switch (wParam)							// Check System Calls
        {
        case SC_SCREENSAVE:					// Screensaver Trying To Start?
        case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
            return 0;							// Prevent From Happening
        }
        break;									// Exit
    }

    case WM_CLOSE:								// Did We Receive A Close Message?
    {
        PostQuitMessage(0);						// Send A Quit Message
        
        //Window_win32 *win = (Window_win32*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        //if (win)
        //{
        //    win->mDone = true;
        //}

        return 0;								// Jump Back
    }

    case WM_KEYDOWN:							// Is A Key Being Held Down?
    {
        //NBUT::m_keys[wParam] = TRUE;					// If So, Mark It As TRUE

        Window_win32 *win = (Window_win32*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (win)
        {
            //win->mWindowEventCallback->onKey(wParam, 1, 0, 0);
            win->onKey(convertKeyCode(wParam, lParam), egg::KeyState::KS_DOWN, 0, 0);
        }

        return 0;								// Jump Back
    }

    case WM_KEYUP:								// Has A Key Been Released?
    {
        //NBUT::m_keys[wParam] = FALSE;					// If So, Mark It As FALSE
        Window_win32 *win = (Window_win32*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (win)
        {
            //win->mWindowEventCallback->onKey(wParam, 0, 0, 0);
            win->onKey(convertKeyCode(wParam, lParam), egg::KeyState::KS_UP, 0, 0);
        }

        return 0;								// Jump Back
    }


    case WM_LBUTTONUP:
    {
        Window_win32 *win = (Window_win32*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (win)
        {
            //win->mWindowEventCallback->onMouse(1, 1, 0, LOWORD(lParam), HIWORD(lParam));
            win->onMouse(egg::MouseType::MT_Click, egg::MouseButton::MB_Left, egg::KS_UP, LOWORD(lParam), HIWORD(lParam));
        }
        return 0;
    }

    case WM_LBUTTONDOWN:
    {
        Window_win32 *win = (Window_win32*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (win)
        {
            //win->mWindowEventCallback->onMouse(1, 1, 1, LOWORD(lParam), HIWORD(lParam));
            win->onMouse(egg::MouseType::MT_Click, egg::MouseButton::MB_Left, egg::KS_DOWN, LOWORD(lParam), HIWORD(lParam));
        }
        return 0;
    }

    case WM_RBUTTONUP:
    {
        Window_win32 *win = (Window_win32*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (win)
        {
            //win->mWindowEventCallback->onMouse(1, 2, 0, LOWORD(lParam), HIWORD(lParam));
            win->onMouse(egg::MouseType::MT_Click, egg::MouseButton::MB_Right, egg::KS_UP, LOWORD(lParam), HIWORD(lParam));
        }
        return 0;
    }

    case WM_RBUTTONDOWN:
    {
        Window_win32 *win = (Window_win32*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (win)
        {
            //win->mWindowEventCallback->onMouse(1, 2, 1, LOWORD(lParam), HIWORD(lParam));
            win->onMouse(egg::MouseType::MT_Click, egg::MouseButton::MB_Right, egg::KS_DOWN, LOWORD(lParam), HIWORD(lParam));
        }
        return 0;
    }

    case WM_MBUTTONUP:
    {
        Window_win32 *win = (Window_win32*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (win)
        {
            //win->mWindowEventCallback->onMouse(1, 3, 0, LOWORD(lParam), HIWORD(lParam));
            win->onMouse(egg::MouseType::MT_Click, egg::MouseButton::MB_Middle, egg::KS_UP, LOWORD(lParam), HIWORD(lParam));
        }
        return 0;
    }

    case WM_MBUTTONDOWN:
    {
        Window_win32 *win = (Window_win32*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (win)
        {
            //win->mWindowEventCallback->onMouse(1, 3, 1, LOWORD(lParam), HIWORD(lParam));
            win->onMouse(egg::MouseType::MT_Click, egg::MouseButton::MB_Middle, egg::KS_DOWN, LOWORD(lParam), HIWORD(lParam));
        }
        return 0;
    }

    case WM_MOUSEMOVE:
    {
        Window_win32 *win = (Window_win32*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (win)
        {
            egg::MouseButton b = egg::MouseButton::MB_Null;
            if (wParam & MK_LBUTTON){ b = egg::MouseButton::MB_Left;}
            if (wParam & MK_MBUTTON){ b = egg::MouseButton::MB_Right; }
            if (wParam & MK_RBUTTON) {b = egg::MouseButton::MB_Right; }

            //win->mWindowEventCallback->onMouse(0, b, b ? 1 : 0, LOWORD(lParam), HIWORD(lParam));
            win->onMouse(egg::MouseType::MT_Move, b
                , (b>egg::MouseButton::MB_Null) ? egg::KS_DOWN : egg::KS_UP, LOWORD(lParam), HIWORD(lParam));
        }
        return 0;
    }

    case WM_SIZE:								// Resize The OpenGL Window
    {
        Window_win32 *win = (Window_win32*)GetWindowLongPtr(hWnd, GWL_USERDATA);
        if (win)
        {
            //win->mWindowEventCallback->onSize(LOWORD(lParam), HIWORD(lParam));
            win->onSize(0, 0, LOWORD(lParam), HIWORD(lParam));
        }

        //ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
        return 0;								// Jump Back
    }

    case WM_TOUCH:
    {
        Window_win32 *win = (Window_win32*)GetWindowLongPtr(hWnd, GWL_USERDATA);

        //　A　WM_TOUCH 　message　can　contain　several　messages　from　different　 contacts
        //　packed　together.
        unsigned int numInputs = (int)wParam; //Number　of　actual　contact　 messages
        TOUCHINPUT* ti = new TOUCHINPUT[numInputs]; //　Allocate　the　storage　for
        //the　 parameters　of　the　per-
        //contact　 messages

        //　Unpack　message　parameters　into　the 　array　of　TOUCHINPUT　structures,　each
        //　representing 　a　message　for　one　single　contact.
        if (GetTouchInputInfo((HTOUCHINPUT)lParam, numInputs, ti, sizeof(TOUCHINPUT)))
        {
            //　For　each　contact,　dispatch　the　message 　to　the　appropriate　message
            //　handler.
            for (unsigned int i = 0; i < numInputs; i++)
            {
                if (ti[i].dwFlags & TOUCHEVENTF_DOWN)
                {
                    //OnTouchDownHandler(hWnd, ti[i]);

                    float out_x = 0.0;
                    float out_y = 0.0;
                    GetTouchPoint(hWnd, ti[i], out_x, out_y);
                    //win->mWindowEventCallback->onTouch(1, ti[i].dwID, out_x, out_y);
                    win->onTouch(egg::TT_AddCursor, ti[i].dwID, out_x, out_y);
                }
                else if (ti[i].dwFlags & TOUCHEVENTF_MOVE)
                {
                    //OnTouchMoveHandler(hWnd, ti[i]);

                    float out_x = 0.0;
                    float out_y = 0.0;
                    GetTouchPoint(hWnd, ti[i], out_x, out_y);
                    //win->mWindowEventCallback->onTouch(2, ti[i].dwID, out_x, out_y);
                    win->onTouch(egg::TT_MoveCursor, ti[i].dwID, out_x, out_y);
                }
                else if (ti[i].dwFlags & TOUCHEVENTF_UP)
                {
                    //OnTouchUpHandler(hWnd, ti[i]);

                    float out_x = 0.0;
                    float out_y = 0.0;
                    GetTouchPoint(hWnd, ti[i], out_x, out_y);
                    //win->mWindowEventCallback->onTouch(3, ti[i].dwID, out_x, out_y);
                    win->onTouch(egg::TT_DeleteCursor, ti[i].dwID, out_x, out_y);
                }
            }
        }

        CloseTouchInputHandle((HTOUCHINPUT)lParam);
        delete[] ti;
    }
    break;

    }//

    // Pass All Unhandled Messages To DefWindowProc
    return DefWindowProc(hWnd, uMsg, wParam, lParam);

}


INSTALL_MODULE(Window_win32)
{
    //egg::Window::registerFactory("Window_win32"
    //    ,new egg::Window::Factory<Window_win32>());

    egg::WindowFactory::getInstance(true)
        ->registerCrteator("Window_win32"
            , new egg::Crteator<Window_win32>());

    return true;
}

UNINSTALL_MODULE(Window_win32)
{
    //egg::Window::unregisterFactory("Window_win32");

    egg::WindowFactory::getInstance(true)
        ->unregisterCrteator("Window_win32");

    return true;
}



