
#include <nbPlugins/public.h>

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

//#include <GLES/gl.h>
#include <EGL/egl.h>

//#pragma comment (lib, "libEGL.lib")
//#pragma comment (lib, "libGLESv1_CM.lib")   
//#pragma comment (lib, "libGLESv2.lib")

/*
一个HGLRC被分为了三个对象
EGLDisplay m_eglDisplay;
EGLContext m_eglContext;
EGLSurface m_eglSurface;
*/

class GraphicsContext_egl_mali
    :public nbPlugins::GraphicsContext
{
public:

    GraphicsContext_egl_mali(){}
    virtual ~GraphicsContext_egl_mali(){}

    EGLDisplay m_eglDisplay;
    EGLContext m_eglContext;
    EGLSurface m_eglSurface;

    EGLConfig m_eglConfig[1];

    virtual bool choosePixelFormat(
        nbPlugins::NativeWindow* win
        , int width, int height
        , int ColorBits			//24 32
        , int AlphaBits			//0
        , int AccumBits			//0
        , int DepthBits			//16Bit Z-Buffer (Depth Buffer)  
        , int StencilBits		//0
        , int AuxBuffers			//0
        , int samples			//多采样定义  0关闭 2 4 8 16
        , bool doublebuffer		//双缓冲	
        )
    {
        EGLint nConfigs;

        //EGLint attributeList[] = { EGL_RED_SIZE, 1, EGL_DEPTH_SIZE, 1, EGL_NONE };
        EGLint aEGLAttributes[] = {
            //EGL_BUFFER_SIZE, bpp,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
#if NIUBI_USE_GLES1
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
#endif
#if NIUBI_USE_GLES2
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
#endif
#if NIUBI_USE_GLES3
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
#endif
            EGL_SAMPLE_BUFFERS, 0,
            EGL_SAMPLES, 0,
            EGL_NONE
        };

        // EGL init.
        m_DC = (unsigned int)GetDC((HWND)(win->m_winID));
        m_eglDisplay = eglGetDisplay(
            //(NativeDisplayType)m_DC
            EGL_DEFAULT_DISPLAY
            );

        if (m_eglDisplay == EGL_NO_DISPLAY || eglGetError() != 0x3000)
        {
            printf("m_eglDisplay error\n");
        }

        if (!eglInitialize(m_eglDisplay, NULL, NULL))
        {
            printf("eglInitialize error\n");
        }

        if (!eglChooseConfig(m_eglDisplay, aEGLAttributes, m_eglConfig, 1, &nConfigs))
        {
            printf("eglChooseConfig error\n");
        }
        printf("EGLConfig = %p\n", m_eglConfig[0]);


        m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig[0], (NativeWindowType)((HWND)win->m_winID), 0);

        return true;
    }

    virtual bool createGraphaContext(nbPlugins::NativeWindow* win)
    {
        mNativeWindow = win;

        EGLint aEGLContextAttributes[] = {
#if NIUBI_USE_GLES1
            EGL_CONTEXT_CLIENT_VERSION, 1,
#endif
#if NIUBI_USE_GLES2
            EGL_CONTEXT_CLIENT_VERSION, 2,
#endif
#if NIUBI_USE_GLES3
            EGL_CONTEXT_CLIENT_VERSION, 3,
#endif
            EGL_NONE
        };

        m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig[0], EGL_NO_CONTEXT, aEGLContextAttributes);
        printf("EGLSurface = %p\n", m_eglSurface);
        printf("EGLContext = %p\n", m_eglContext);

        eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

        mCreated = true;
        return true;

    }
    virtual bool destroyGraphaContext(nbPlugins::NativeWindow* win)
    {
        // Exit.
        eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(m_eglDisplay, m_eglContext);
        eglDestroySurface(m_eglDisplay, m_eglSurface);
        eglTerminate(m_eglDisplay);

        ReleaseDC(((HWND)win->m_winID), (HDC)m_DC);
        mCreated = false;

        return true;
    }

    unsigned int getCurrentContext()
    {
        return (unsigned int)eglGetCurrentContext();
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
        // Swap Buffers (Double Buffering)
        eglSwapBuffers(m_eglDisplay, m_eglSurface);
    }

protected:
private:

};




//注册
//REGISTER_NIUBI_PLUG( /*"GraphicsContext_egl_mali" , */GraphicsContext_egl_mali )
NIUBI_REGISTER_PLUGINS_FUNCTION_BEGIN(pm, GraphicsContext_egl_mali)
NIUBI_REGISTER_PLUGINS(pm, GraphicsContext_egl_mali)
NIUBI_REGISTER_PLUGINS_FUNCTION_END
