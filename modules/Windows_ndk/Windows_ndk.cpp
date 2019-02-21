
#include <nbBase/Public.h>
#include <nbPlugins/Public.h>

#include <jni.h>
#include <errno.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
      
                         
class NativeWindow_ndk
	:public nbPlugins::NativeWindow
{
public:

    //engine globle object
    // struct engine 
    // {
        // struct android_app* app;
        // void* native_window;
        // bool isinit;
        
        // EGLDisplay display;
        // EGLSurface surface;
        // EGLContext context;
        // int32_t width;
        // int32_t height;
        
        // int animating;
        // struct saved_state state;
    // };
    typedef nbBase::niubi_entry_arg engine;
    
    //bool init;

	NativeWindow_ndk(){LOGI("NativeWindow_ndk()");}
	virtual ~NativeWindow_ndk(){LOGI("~NativeWindow_ndk()");}

	virtual bool createWindow(const char* title, int width, int height, int bits, bool fullscreen )
	{
        LOGI("createWindow");
        
        //struct engine engine;
        engine* e = (engine*)m_globle;
        //android_app* app
     
        //memset(&engine, 0, sizeof(engine));
        e->state->userData     = e;
        e->state->onAppCmd     = engine_handle_cmd;
        e->state->onInputEvent = engine_handle_input;
        e->native_window = (void*)this;
        //e->app = state;


#if 0  //启动时自动解压apk的assets目录到files目录
	
	//assets管理器
	ANativeActivity* nativeActivity = e->state->activity;
	AAssetManager*   assetManager = nativeActivity->assetManager;
	const char* internalPath = nativeActivity->internalDataPath;
	
	std::string datalist_file = "datalist.txt";
    std::string files_path(internalPath); 
	std::string assets_path("data");
	files_path = files_path+"/"+assets_path;

	//打开datalist_file
	AAsset* configFileAsset = AAssetManager_open(assetManager
												, datalist_file.c_str()
												, AASSET_MODE_BUFFER);
	if(configFileAsset == 0)
	{
		LOGW("configFileAsset %d %d %s"
		,(unsigned int)assetManager,(unsigned int)configFileAsset,datalist_file.c_str());
	}
	else
	{
		const void* configData = AAsset_getBuffer(configFileAsset);
		const off_t configLen  = AAsset_getLength(configFileAsset);
		
		//读取所有文件列表
		std::vector<std::string> sl = nbBase::split(std::string((const char*)configData)," ",false);
		
		//释放
		AAsset_close(configFileAsset);
			
		//循环处理
		for(int i = 0;i<sl.size();i++)
		{
			//虑掉无效的地址
			if(sl[i] == "." || sl[i].size() == 0 || sl[i][0] < 32)
			{
				continue;
			}
			
			//输入输出路径
			std::string assets_filename = nbBase::Path((assets_path+"/"+sl[i]).c_str()).getPathUnix();
			std::string files_filename  = nbBase::Path((files_path +"/"+sl[i]).c_str()).getPathUnix();
			LOGW( "%s -> %s\n",assets_filename.c_str(),files_filename.c_str() );


			//打开asset
			AAsset* fileAsset = AAssetManager_open(assetManager, assets_filename.c_str()+2, AASSET_MODE_BUFFER);
			if(fileAsset == 0)
			{
				LOGW("fileAsset %d %d %s"
				,(unsigned int)assetManager,(unsigned int)fileAsset,assets_filename.c_str());
				continue;//无效文件跳过 比如目录
			}
			else
			{
				//获取文件到内存
				const void* fileData = AAsset_getBuffer(fileAsset);
				const off_t fileLen  = AAsset_getLength(fileAsset);
				
				//根据需要建立路径
				nbBase::Path p(files_filename.c_str());
				nbBase::Path::createDirectory(p.getPathUnix(true).c_str());
				
				LOGW( "need file %s\n",p.getPathUnix().c_str());
				LOGW( "need dir  %s\n",p.getPathUnix(true).c_str());
				

				//复制文件
				FILE* f = fopen(  files_filename.c_str(), "wb+");
				if ( f == NULL )
				{
					LOGE("Could not create file %s\n",files_filename.c_str());
				}
				else
				{
					int32_t res = fwrite(fileData, sizeof(char), fileLen, f);
					if (fileLen == res)
					{
						LOGI("copy file %s\n",files_filename.c_str());
					}
					else
					{
						LOGE("Error copy file %s\n",files_filename.c_str());
					}
					fclose(f);
				}//file
				
			}//assets

			
		}
		
	}
#endif

		return true;
	}
	


	virtual bool destroyWindow()
	{
		return true;
	}
    
	virtual bool isShow()
	{
        engine* e = (engine*)m_globle;
		return e->isinit;
	}

	virtual void sleep(unsigned int ms)
	{

	}
	
	std::string getDataPath()
	{
		engine* e = (engine*)m_globle;
		ANativeActivity* nativeActivity = e->state->activity;
		AAssetManager*   assetManager = nativeActivity->assetManager;
		
		const char* internalPath = nativeActivity->internalDataPath;
		std::string dataPath(internalPath);
		dataPath+="/data/";	
		
		return dataPath;
	}

    virtual bool isSupportMultiTouch()
    {
        //这里应该测试窗口是否支持多触点输入
        return true;
    }
    
	virtual bool updateWindow()
	{
        engine* e = (engine*)m_globle;
     
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident = ALooper_pollAll(
            //e->animating ? 0 : -1
            0
            , NULL
            , &events,(void**)&source)) >= 0) 
        {
            // Process this event.
            if (source != NULL) 
            {
                source->process(e->state, source);
            }

            // Check if we are exiting.
            if (e->state->destroyRequested != 0) 
            {
                engine_term_display( e );
                return false;
            }
        }

        // if (engine.animating)
        // {
            // engine_draw_frame(&engine);
        // }
        
		return true;
	}


protected:
private:

    static int engine_init_display(engine* e) 
    {
        //engine* e = (engine*)m_globle;
        NativeWindow* nw = (NativeWindow*)e->native_window;
        
        //初始化设备
        LOGI("createGraphaContext()");
		nw->m_gc_plug->choosePixelFormat(
			nw
			,0,0//,width,height
			//,bits,0
			,32,8			//色彩 alpha 一直RGB 很费解
			,1				//积累缓冲 无法打开
			,16				//怎么设置都是开启 都是float很奇怪
			,1				//大于0就是有效
			,1				//永远是开启的
			,4				//采样有效
			,true			//双缓冲有效
			);
		nw->m_gc_plug->createGraphaContext(nw);
            
        LOGI("m_renderer_plug->init()");
		nw->m_renderer_plug->init();
        
        //engine* e = (engine*)m_globle;
        e->isinit = true;
        
        LOGI("engine_init_display() return");
        return 0;
    }

    /**
     * Just the current frame in the display.
     */
    // static void engine_draw_frame(struct engine* engine) 
    // {
        // if (engine->display == NULL) 
        // {
            // return;// No display.
        // }

        // Just fill the screen with a color.
        // glClearColor(((float)engine->state.x)/engine->width, engine->state.angle,
                // ((float)engine->state.y)/engine->height, 1);
        // glClear(GL_COLOR_BUFFER_BIT);

        // eglSwapBuffers(engine->display, engine->surface);
    // }

    /**
     * Tear down the EGL context currently associated with the display.
     */
    static void engine_term_display(engine* e) 
    {
        NativeWindow* nw = (NativeWindow*)e->native_window;
    
        //销毁设备
		nw->m_gc_plug->destroyGraphaContext(nw);
       
    }

    /**
     * Process the next input event.
     */
    static int32_t engine_handle_input(android_app* app, AInputEvent* event) 
    {
        engine* e = (engine*)app->userData;
        // if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) 
        // {
            // engine->animating = 1;
            // engine->state.x = AMotionEvent_getX(event, 0);
            // engine->state.y = AMotionEvent_getY(event, 0);
            // return 1;
        // }
        
        if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) 
        {
            NativeWindow* win = (NativeWindow*)e->native_window;
            
            int32_t action = AKeyEvent_getAction(event);
            switch (action & AMOTION_EVENT_ACTION_MASK) 
            {
                case AMOTION_EVENT_ACTION_DOWN: 
                    {
                        size_t count = AMotionEvent_getPointerCount(event);
                        for( size_t i = 0 ; i < count ; i++)
                        {
                            float x = AMotionEvent_getX(event, i);
                            float y = AMotionEvent_getY(event, i);
                            int32_t id = AMotionEvent_getPointerId(event, i);
                            
                            LOGI("add %d %f %f",id,x,y);
                            win->m_event_callback->onTouch(1,id,x/e->width,y/e->height);
                        }
                    }
                    break;
                case AMOTION_EVENT_ACTION_MOVE: 
                    {
                        size_t count = AMotionEvent_getPointerCount(event);
                        for( size_t i = 0 ; i < count ; i++)
                        {
                            float x = AMotionEvent_getX(event, i);
                            float y = AMotionEvent_getY(event, i);
                            int32_t id = AMotionEvent_getPointerId(event, i);
                            
                            LOGI("move %d %f %f",id,x,y);
                            win->m_event_callback->onTouch(2,id,x/e->width,y/e->height);
                        }
                    }
                    break;
                    
                case AMOTION_EVENT_ACTION_UP: 
                    {
                        size_t count = AMotionEvent_getPointerCount(event);
                        for( size_t i = 0 ; i < count ; i++)
                        {
                            float x = AMotionEvent_getX(event, i);
                            float y = AMotionEvent_getY(event, i);
                            int32_t id = AMotionEvent_getPointerId(event, i);
                            
                            LOGI("del %d %f %f",id,x,y);
                            win->m_event_callback->onTouch(3,id,x/e->width,y/e->height);
                        }
                    }
                    break;
            }
            
        }
        return 0;
    }

    /**
     * Process the next main command.
     */
    static void engine_handle_cmd(android_app* app, int32_t cmd) 
    {
        engine* e = (engine*)app->userData;
		NativeWindow* nw = (NativeWindow*)e->native_window;
		
        switch (cmd) 
        {
            case APP_CMD_SAVE_STATE:
                // The system has asked us to save our current state.  Do so.
                // engine->app->savedState = malloc(sizeof(struct saved_state));
                // *((struct saved_state*)engine->app->savedState) = engine->state;
                // engine->app->savedStateSize = sizeof(struct saved_state);
                break;
            case APP_CMD_INIT_WINDOW:
                // The window is being shown, get it ready.
                if (e->state->window != NULL) 
                {
                    engine_init_display(e);
                    //engine_draw_frame(engine);
                }
                break;
            case APP_CMD_TERM_WINDOW:
                // The window is being hidden or closed, clean it up.
                engine_term_display(e);
                break;
            case APP_CMD_GAINED_FOCUS:
                // When our app gains focus, we start monitoring the accelerometer.
                // if (engine->accelerometerSensor != NULL) 
                // {
                    // ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                            // engine->accelerometerSensor);
                    // We'd like to get 60 events per second (in us).
                    // ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                            // engine->accelerometerSensor, (1000L/60)*1000);
                // }
                break;
            case APP_CMD_LOST_FOCUS:
                // When our app loses focus, we stop monitoring the accelerometer.
                // This is to avoid consuming battery while not being used.
                // if (engine->accelerometerSensor != NULL) 
                // {
                    // ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                            // engine->accelerometerSensor);
                // }
                // Also stop animating.
                // engine->animating = 0;
                //engine_draw_frame(engine);
                break;
				
			case APP_CMD_WINDOW_RESIZED:
			    // EGLint w, h;
				// eglQuerySurface((EGLDisplay)e->display, (EGLSurface)e->surface, EGL_WIDTH,  &w);
				// eglQuerySurface((EGLDisplay)e->display, (EGLSurface)e->surface, EGL_HEIGHT, &h);
				//nw->m_event_callback->onSize(w,h);
				
				//提示gc有一次resize
				if(nw->m_gc_plug)nw->m_gc_plug->needResize();
				
				break;
        }
    }

};


//注册
NIUBI_REGISTER_PLUGINS_FUNCTION_BEGIN( pm ,NativeWindow_ndk)
NIUBI_REGISTER_PLUGINS(pm,NativeWindow_ndk)
NIUBI_REGISTER_PLUGINS_FUNCTION_END
