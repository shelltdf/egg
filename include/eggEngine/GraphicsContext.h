#ifndef GraphicsContext_h__
#define GraphicsContext_h__

//#include <egg/Public.h>
#include <eggEngine/Export.h>

#include <eggRuntime/Object.h>
#include <eggRuntime/Singleton.h>
#include <eggRuntime/Factory.h>
#include <eggRuntime/DataType.h>

namespace egg
{
	class Window;

	class EGG_ENGINE_EXPORT GraphicsContext
        :public Object
	{

	public:

		GraphicsContext()
            :mCreated(false),m_globle(0),mNeedReszie(false),mNativeWindow(0)
            , m_GI(0)
            , m_DC(0)
            , m_gcID(0)
            , m_pf(0)
        {}
		virtual ~GraphicsContext(){}

        //DECLARE_FACTORY(GraphicsContext);

	public:
    
		//选择像素格式
		virtual bool choosePixelFormat(
            Window* win
			,int width, int height	//窗口WH  dx需要 gl不需要
			,int ColorBits			//24 32
			,int AlphaBits			//0
			,int AccumBits			//0
			,int DepthBits			//24Bit Z-Buffer (Depth Buffer)  
			,int StencilBits		//0
			,int AuxBuffers			//0
			,int samples			//多采样定义  0关闭 2 4 8 16
			,bool doublebuffer		//双缓冲
			) = 0;

		//建立设备环境
		virtual bool createGraphaContext(Window* win) = 0;
		virtual bool destroyGraphaContext(Window* win) = 0;
		virtual bool isCreated(){return mCreated;}

		void setGloble(void* g){m_globle = g; }
		void* m_globle; //全局对象弱关联

		//交换缓冲区
		virtual void swapBuffers() = 0;

		//全局设备对象
		//opengl无用  dx是LPDIRECT3D9
        HANDLE_TYPE m_GI;

		//窗口设备ID
		//opengl是dc  dx无用
        HANDLE_TYPE m_DC;

		//图形设备标识符 
		//根据接口环境不同需要强制转换成不同类型  
		//gl下是HGLRC  dx下是LPDIRECT3DDEVICE9
        HANDLE_TYPE m_gcID;
        
        //当前选择的像素格式 默认为0
        // windows为PixelFormat
        // x11为visual指针
        HANDLE_TYPE m_pf;


		//当前图形环境是否需要窗口辅助全屏  目前已知的只有dx不需要
		virtual bool isWindowsAuxFull() = 0;

		//辅助android更新的
		void needResize(){mNeedReszie = true;}
		virtual void frame() = 0;

        //
        virtual bool makeCurrent() = 0;

        //设置垂直同步
        virtual bool setVSync(bool b) = 0;

	protected:

		bool mCreated;
		bool mNeedReszie;
        Window* mNativeWindow;
	};


    class EGG_ENGINE_EXPORT GraphicsContextFactory
        :public Factory<GraphicsContext>
    {
    public:
        GraphicsContextFactory() {}
        ~GraphicsContextFactory() {}
        DECLARE_SINGLETON(GraphicsContextFactory)
    };

}

#endif // GraphicsContext_h__

