
#include <egg/Platform.h>
#include <egg/Module.h>
#include <egg/Runtime.h>
#include <eggEngine/Window.h>
#include <eggEngine/GraphicsContext.h>
#include <eggEngine/Renderer.h>
#include <eggEngine/Pass.h>

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
//#include <Wingdi.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "gdi32.lib")    /* link Windows GDI lib        */
#pragma comment (lib, "winmm.lib")    /* link Windows MultiMedia lib */
#pragma comment (lib, "user32.lib")   /* link Windows user lib       */


class GraphicsContext_wgl
    :public egg::GraphicsContext
{
public:

	GraphicsContext_wgl(){}
	virtual ~GraphicsContext_wgl(){}

	virtual bool choosePixelFormat(
        egg::Window* win
		,int width, int height
		,int ColorBits			//24 32
		,int AlphaBits			//0
		,int AccumBits			//0
		,int DepthBits			//16Bit Z-Buffer (Depth Buffer)  
		,int StencilBits		//0
		,int AuxBuffers			//0
		,int samples			//多采样定义  0关闭 2 4 8 16
		,bool doublebuffer		//双缓冲	
		)
	{

		if (!(m_DC=(HANDLE_TYPE)GetDC((HWND)win->mWinID)))							// Did We Get A Device Context?
		{
			win->destroyWindow();								// Reset The Display
			MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

#if 1
		DWORD dwFlags =  PFD_DRAW_TO_WINDOW |		// Format Must Support Window
			PFD_SUPPORT_OPENGL;						// Format Must Support OpenGL;
		if(doublebuffer)
		{
			dwFlags |= PFD_DOUBLEBUFFER;			// Must Support Double Buffering
		}

		static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			dwFlags,
			PFD_TYPE_RGBA,								// Request An RGBA Format
			ColorBits,									// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			AlphaBits,									// No Alpha Buffer
			0,											// Shift Bit Ignored
			AccumBits,									// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			DepthBits,									// 16Bit Z-Buffer (Depth Buffer)  
			StencilBits,								// No Stencil Buffer
			AuxBuffers,									// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};

		GLuint		PixelFormat;			// Holds The Results After Searching For A Match


		//第二次初始化的时候才可能有效
		//if( arbMultisampleSupported )
		//{
		//	PixelFormat = arbMultisampleFormat;
		//}
		//else
		//{
			if (!(PixelFormat=ChoosePixelFormat((HDC)m_DC,&pfd)))	// Did Windows Find A Matching Pixel Format?
			{
				win->destroyWindow();								// Reset The Display
				MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
				return FALSE;								// Return FALSE
			}
		//}


#endif

		if(!SetPixelFormat((HDC)m_DC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
		{
			win->destroyWindow();								// Reset The Display
			MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		return true;
	}

    virtual bool createGraphaContext(egg::Window* win)
	{		
		mNativeWindow = win;

		if (!(m_gcID=(HANDLE_TYPE)wglCreateContext((HDC)m_DC)))				// Are We Able To Get A Rendering Context?
		{
			win->destroyWindow();								// Reset The Display
			MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if(!wglMakeCurrent((HDC)m_DC,(HGLRC)m_gcID))					// Try To Activate The Rendering Context
		{
			win->destroyWindow();								// Reset The Display
			MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

#if 0

		if( 0 ) //samples > 0
		{
			//测试WGL支持  如果支持就重新建立context
			if(InitMultisample( 0 , (HDC)m_DC ,pfd 
				, ColorBits
				, AlphaBits
				, AccumBits
				, DepthBits
				, StencilBits
				, AuxBuffers
				, samples
				, doublebuffer
				))
			{
				destroyGraphaContext(win);
				return 
					createGraphaContext(
					win, width, height
					, ColorBits	
					, AlphaBits
					, AccumBits
					, DepthBits
					, StencilBits
					, AuxBuffers
					, samples
					, doublebuffer
					);

			}
		}

#endif 


		//		
//#if 1
//
//
//		//int pfd = 0;
//
//		// See If The String Exists In WGL!
//		if (!WGLisExtensionSupported("WGL_ARB_multisample"))
//		{
//			arbMultisampleSupported=false;
//			return false;
//		}
//
//		// Get Our Pixel Format
//		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");	
//		if (!wglChoosePixelFormatARB) 
//		{
//			arbMultisampleSupported=false;
//			return false;
//		}
//
//		//int PixelFormat;
//		bool valid;
//		UINT numFormats;
//		float fAttributes[] = {0,0};
//
//		// These Attributes Are The Bits We Want To Test For In Our Sample
//		// Everything Is Pretty Standard, The Only One We Want To
//		// Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
//		// These Two Are Going To Do The Main Testing For Whether Or Not
//		// We Support Multisampling On This Hardware
//		int iAttributes[] = { 
//			WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
//			WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
//			WGL_ACCELERATION_ARB ,WGL_FULL_ACCELERATION_ARB,
//			WGL_COLOR_BITS_ARB,24,
//			WGL_ALPHA_BITS_ARB,8,
//			WGL_DEPTH_BITS_ARB,16,
//			WGL_STENCIL_BITS_ARB,0,
//			WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
//			WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
//			WGL_SAMPLES_ARB, 4 ,                        // Check For 4x Multisampling
//			0,0};
//
//		// First We Check To See If We Can Get A Pixel Format For 4 Samples
//		valid = wglChoosePixelFormatARB((HDC)m_DC,iAttributes,fAttributes,1,(int*)&PixelFormat,&numFormats);
//
//		// if We Returned True, And Our Format Count Is Greater Than 1
//		if (valid && numFormats >= 1)
//		{
//			arbMultisampleSupported = true;
//			arbMultisampleFormat    = PixelFormat; 
//			return arbMultisampleSupported;
//		}
//
//		// Our Pixel Format With 4 Samples Failed, Test For 2 Samples
//		iAttributes[19] = 2;
//		valid = wglChoosePixelFormatARB((HDC)m_DC,iAttributes,fAttributes,1,(int*)&PixelFormat,&numFormats);
//		if (valid && numFormats >= 1)
//		{
//			arbMultisampleSupported = true;
//			arbMultisampleFormat    = PixelFormat;  
//			return arbMultisampleSupported;
//		}
//
//#endif
//


		mCreated = true;
		return true;

	}

    virtual bool destroyGraphaContext(egg::Window* win)
	{
		if (m_gcID)											// Do We Have A Rendering Context?
		{
			if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
			{
				MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			}

			if (!wglDeleteContext((HGLRC)m_gcID))						// Are We Able To Delete The RC?
			{
				MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			}
			m_gcID=NULL;										// Set RC To NULL
		}

		if (m_DC && !ReleaseDC((HWND)win->mWinID,(HDC)m_DC))					// Are We Able To Release The DC
		{
			MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			m_DC=NULL;										// Set DC To NULL
		}

		mCreated = false;
		return true;
	}

    unsigned int getCurrentContext()
    {
        return (unsigned int)wglGetCurrentContext();
    }

	virtual bool isWindowsAuxFull()
	{
		return true;
	}

	virtual void frame() //辅助android更新的
	{

	}

	virtual void swapBuffers()
	{
		SwapBuffers((HDC)m_DC);	// Swap Buffers (Double Buffering)
	}

    virtual bool makeCurrent()
    {
        return wglMakeCurrent((HDC)m_DC, (HGLRC)m_gcID);
    }

    virtual bool setVSync(bool b)
    {

        //WGL_EXT_swap_control
        typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
        typedef int (WINAPI * PFNWGLGETSWAPINTERVALEXTPROC) (void);
        PFNWGLSWAPINTERVALEXTPROC       wglSwapIntervalEXT = NULL;
        PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapIntervalEXT = NULL;
        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");

        //WGL_EXT_swap_control
        if (wglSwapIntervalEXT && wglGetSwapIntervalEXT)
        {
            wglSwapIntervalEXT(b ? 1 : 0);
            return true;
        }

        return false;
    }

protected:
private:

    //bool WGLExtensionSupported(const char *extension_name)
    //{
    //    // this is pointer to function which returns pointer to string with list of all wgl extensions
    //    PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;

    //    // determine pointer to wglGetExtensionsStringEXT function
    //    _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

    //    if (strstr(wglGetExtensionsString(), extension_name) == NULL)
    //    {
    //        // string was not found
    //        return false;
    //    }

    //    // extension is supported
    //    return true;
    //}
};




//注册
//REGISTER_NIUBI_PLUG( /*"GraphicsContext_wgl" ,*/ GraphicsContext_wgl )
//NIUBI_REGISTER_PLUGINS_FILE(GraphicsContext_wgl)
//void nbRegisterPlugins( NiuBi::PluginsManager* pm )
//{
//	NIUBI_REGISTER_PLUGINS(pm,GraphicsContext_wgl)
//}

//NIUBI_REGISTER_PLUGINS_FUNCTION_BEGIN( pm ,GraphicsContext_wgl)
//NIUBI_REGISTER_PLUGINS(pm,GraphicsContext_wgl)
//NIUBI_REGISTER_PLUGINS_FUNCTION_END
//


INSTALL_MODULE(GraphicsContext_wgl)
{
    egg::GraphicsContextFactory::getInstance(true)
        ->registerCrteator("GraphicsContext_wgl"
        , new egg::Crteator<GraphicsContext_wgl>());

    return true;
}

UNINSTALL_MODULE(GraphicsContext_wgl)
{
    egg::GraphicsContextFactory::getInstance(true)
        ->unregisterCrteator("GraphicsContext_wgl");

    return true;
}

