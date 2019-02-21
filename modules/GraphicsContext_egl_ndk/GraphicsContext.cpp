
#include <nbPlugins/public.h>
#include <nbBase/public.h>

#include <jni.h>
#include <errno.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>


/*
一个HGLRC被分为了三个对象
EGLDisplay m_eglDisplay;
EGLContext m_eglContext;
EGLSurface m_eglSurface;
*/

class GraphicsContext_egl_ndk
	:public nbPlugins::GraphicsContext
{
public:

    //engine globle object
    typedef nbBase::niubi_entry_arg engine;

	GraphicsContext_egl_ndk(){mNeedReszie = true;}
	virtual ~GraphicsContext_egl_ndk(){}

	EGLDisplay m_eglDisplay;
	EGLContext m_eglContext;
	EGLSurface m_eglSurface;

	EGLConfig config;

	virtual bool choosePixelFormat(
		nbPlugins::NativeWindow* win
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
		engine* e = (engine*)m_globle;

		//EGLint attributeList[] = { EGL_RED_SIZE, 1, EGL_DEPTH_SIZE, 1, EGL_NONE };
		EGLint aEGLAttributes[] = {
			//EGL_BUFFER_SIZE, bpp,
#if NIUBI_USE_GLES1
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
#endif
#if NIUBI_USE_GLES2
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
#endif
#if NIUBI_USE_GLES3
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
#endif
			EGL_RED_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_BLUE_SIZE, 8,
			EGL_DEPTH_SIZE, 24,
			// EGL_SAMPLE_BUFFERS, 0,
			// EGL_SAMPLES, 0,
			EGL_NONE
		};
        

        
        EGLint format;
		//EGLConfig m_eglConfig[1];
        
		EGLint nConfigs;


		// EGL init.
		//m_DC=(unsigned int)GetDC((HWND)(win->m_winID));
		m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		if( m_eglDisplay == EGL_NO_DISPLAY || eglGetError() != 0x3000 )
		{
			LOGI("m_eglDisplay error\n");
		}

		if (!eglInitialize(m_eglDisplay, 0, 0))
		{
			LOGI("eglInitialize error\n");
		}

		if (!eglChooseConfig(m_eglDisplay, aEGLAttributes, &config, 1, &nConfigs))
		{
			LOGI("eglChooseConfig error\n");
		}
		LOGI("EGLConfig = %p\n", config);


        /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
        * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
        * As soon as we picked a EGLConfig, we can safely reconfigure the
        * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
        eglGetConfigAttrib(m_eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format);
        ANativeWindow_setBuffersGeometry(e->state->window, 0, 0, format);

    
		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, e->state->window, NULL);

		return true;
	}

	virtual bool createGraphaContext( nbPlugins::NativeWindow* win )
	{
		mNativeWindow = win;
		
        engine* e = (engine*)m_globle;

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

		m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, aEGLContextAttributes);
		LOGI("EGLSurface = %p\n", m_eglSurface);
		LOGI("EGLContext = %p\n", m_eglContext);

        LOGI("eglMakeCurrent\n");
		//eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
        if (eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext) == EGL_FALSE) 
        {
            LOGW("Unable to eglMakeCurrent");
            //return -1;
        }

        LOGI("eglQuerySurface\n");
		EGLint w, h;
        eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_WIDTH, &w);
        eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_HEIGHT, &h);

        // e->display = (unsigned int)m_eglDisplay;
        // e->context = (unsigned int)m_eglContext;
        // e->surface = (unsigned int)m_eglSurface;
        e->width   = w;
        e->height  = h;
        // e->state.angle = 0;
        
        
        LOGI("mCreated = true\n");
		mCreated = true;

		return true;
	}
    
	virtual bool destroyGraphaContext(nbPlugins::NativeWindow* win)
	{
		// Exit.
		// eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		// eglDestroyContext(m_eglDisplay, m_eglContext);
		// eglDestroySurface(m_eglDisplay, m_eglSurface);
		// eglTerminate(m_eglDisplay);

		// ReleaseDC(((HWND)win->m_winID), (HDC)m_DC);
		// mCreated = false;

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
		// LOGI("111111");
		if(mNeedReszie)
		{
			// LOGI("222222");
			EGLint w = 320;
			EGLint h = 240;
			eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_WIDTH,  &w);
			eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_HEIGHT, &h);
			if(mNativeWindow)
			{
				// LOGI("333333");
				mNativeWindow->m_event_callback->onSize(w,h);
				mNeedReszie = false;
				// LOGI("444444");
			}
			// LOGI("555555");
		}
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
NIUBI_REGISTER_PLUGINS_FUNCTION_BEGIN( pm ,GraphicsContext_egl_ndk)
NIUBI_REGISTER_PLUGINS(pm,GraphicsContext_egl_ndk)
NIUBI_REGISTER_PLUGINS_FUNCTION_END
