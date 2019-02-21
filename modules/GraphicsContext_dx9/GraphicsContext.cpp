
#include <nbPlugins/public.h>

#include <d3d9.h>
#include <strsafe.h>

#pragma comment (lib, "d3d9.lib")


//D3DFVF_XYZRHW

class GraphicsContext_dx9
	:public nbPlugins::GraphicsContext
{
public:

	GraphicsContext_dx9(){}
	virtual ~GraphicsContext_dx9(){}

	D3DPRESENT_PARAMETERS d3dpp;

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
		ZeroMemory( &d3dpp, sizeof( d3dpp ) );
		d3dpp.Windowed				= !win->m_fullscreen;
		d3dpp.hDeviceWindow			= (HWND)win->m_winID;    // set the window to be used by Direct3D
		d3dpp.SwapEffect			= D3DSWAPEFFECT_DISCARD;  
		d3dpp.BackBufferWidth		= width;  
		d3dpp.BackBufferHeight		= height;  
		d3dpp.BackBufferFormat		= D3DFMT_X8R8G8B8;  

		return true;
	}

    virtual bool createGraphaContext(nbPlugins::NativeWindow* win)
	{
		mNativeWindow = win;

		if(  ( m_GI = (unsigned int)Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL )
		{
			return false;
		}

		if( FAILED( 
			((LPDIRECT3D9)m_GI)->CreateDevice(
			D3DADAPTER_DEFAULT
			, D3DDEVTYPE_HAL
			, (HWND)win->m_winID
			, D3DCREATE_SOFTWARE_VERTEXPROCESSING
			, &d3dpp
			, (LPDIRECT3DDEVICE9*)&m_gcID
			) 
			) )
		{
			return false;
		}

		mCreated = true;
		return true;
	}

    virtual bool destroyGraphaContext(nbPlugins::NativeWindow* win)
	{
		if( ((LPDIRECT3DDEVICE9)m_gcID) != NULL )
			((LPDIRECT3DDEVICE9)m_gcID)->Release();

		if( ((LPDIRECT3D9)m_GI) != NULL )
			((LPDIRECT3D9)m_GI)->Release();

		mCreated = false;
		return true;
	}

    unsigned int getCurrentContext()
    {
        return (unsigned int)m_gcID;
    }

	virtual bool isWindowsAuxFull()
	{
		return false;
	}
	virtual void frame() //辅助android更新的	
	{

	}
	virtual void swapBuffers()
	{
		// Present the backbuffer contents to the display
		((LPDIRECT3DDEVICE9)m_gcID)->Present( NULL, NULL, NULL, NULL );
	}

protected:
private:

};




//注册
//REGISTER_NIUBI_PLUG( /*"GraphicsContext_dx9" ,*/ GraphicsContext_dx9 )
NIUBI_REGISTER_PLUGINS_FUNCTION_BEGIN( pm ,GraphicsContext_dx9)
NIUBI_REGISTER_PLUGINS(pm,GraphicsContext_dx9)
NIUBI_REGISTER_PLUGINS_FUNCTION_END