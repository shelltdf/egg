
#include <nbPlugins/Public.h>

#include <X11/Xlib.h>
#include <GL/glx.h>

#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library
//#include <gl\glaux.h>		// Header File For The Glaux Library

//#include <gl\glew.h>
//#include <gl\wglew.h>

#include <stdio.h>

class  GraphicsContext_glx
	:public nbPlugins::GraphicsContext
{
public:

	GraphicsContext_glx(){}
	virtual ~GraphicsContext_glx(){}


    virtual bool choosePixelFormat(
            nbPlugins::NativeWindow* win
			,int width, int height	//窗口WH  dx需要 gl不需要
			//,bool PixelType		//if true RBGA
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

        Display* display =  (Display*)win->m_display;
        int screen = DefaultScreen(display);


        /* attributes for a single buffered visual in RGBA format with at least  * 4 bits per color and a 16 bit depth buffer */
        // int attrListSgl[] =
        // {
            // GLX_RGBA, GLX_RED_SIZE, 4,
            // GLX_GREEN_SIZE, 4,
            // GLX_BLUE_SIZE, 4,
            // GLX_DEPTH_SIZE, 16,
            // None
        // };

        /* attributes for a double buffered visual in RGBA format with at least  * 4 bits per color and a 16 bit depth buffer */
        int attrListDbl[] =
        {
            GLX_RGBA,
            GLX_DOUBLEBUFFER,
            GLX_RED_SIZE, 8,
            GLX_GREEN_SIZE, 8,
            GLX_BLUE_SIZE, 8,
            GLX_DEPTH_SIZE, 24,
            None
        };


        /* get an appropriate visual */
        XVisualInfo* vi = glXChooseVisual(display, screen, attrListDbl);
        if (vi == NULL)
        {
            // vi = glXChooseVisual(display, screen, attrListSgl);
            // doubleBuffered = False;
            // printf("singlebuffered rendering will be used, no doublebuffering available\n");
        }
        else
        {
            // doubleBuffered = True;
            printf("doublebuffered rendering available\n");
        }

        int glxMajor, glxMinor;
        glXQueryVersion(display, &glxMajor, &glxMinor);
        printf("GLX-Version %d.%d\n", glxMajor, glxMinor);

        m_pf = (unsigned int)vi;

        return true;
    }

	virtual bool createGraphaContext(nbPlugins::NativeWindow* win)
	{
		mNativeWindow = win;

        Display* display = (Display*)win->m_display;
        Window window = (Window)win->m_winID;
        XVisualInfo* vi = (XVisualInfo*)m_pf;

        // /* create a GLX context */
        GLXContext context = glXCreateContext(display, vi, 0, GL_TRUE);

        glXMakeCurrent(display, window, context);


        // 提示当前渲染状态
        if (glXIsDirect(display, context))
        {
            printf("DRI enabled\n");
        }
        else
        {
            printf("no DRI available\n");
        }

        m_gcID = (unsigned int) context;

		mCreated = true;
		return true;
	}

	virtual bool destroyGraphaContext(nbPlugins::NativeWindow* win)
	{

		mCreated = false;
		return true;
	}

    virtual unsigned int getCurrentContext()
    {
        return (unsigned int) glXGetCurrentContext();
    }

	virtual bool isWindowsAuxFull()
	{
        //需要窗口函数辅助全屏
		return true;
	}

	virtual void frame() //辅助android更新的
	{

	}

	virtual void swapBuffers()
	{
		//SwapBuffers((HDC)m_DC);				// Swap Buffers (Double Buffering)

        Display* display = (Display*)mNativeWindow->m_display;
        Window window = (Window)mNativeWindow->m_winID;

        glXSwapBuffers(display, window);

	}

protected:
private:
};


//注册
NIUBI_REGISTER_PLUGINS_FUNCTION_BEGIN( pm ,GraphicsContext_glx)
NIUBI_REGISTER_PLUGINS(pm,GraphicsContext_glx)
NIUBI_REGISTER_PLUGINS_FUNCTION_END

