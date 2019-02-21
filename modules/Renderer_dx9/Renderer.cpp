
#include <nbPlugins/public.h>

#include <d3d9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

//#include <d3dx9.h>
//#pragma comment (lib, "d3dx9.lib")


class Renderer_dx_9
    :public nbPlugins::Renderer
{
public:

	Renderer_dx_9(){}
	~Renderer_dx_9(){}


	void init()
	{
	}

	void setViewport(int x,int y,int width,int height)
	{
		D3DVIEWPORT9 vp;
		vp.X      = x;
		vp.Y      = y;
		vp.Width  = width;
		vp.Height = height;
		vp.MinZ   = 0.0f;
		vp.MaxZ   = 1.0f;

		((LPDIRECT3DDEVICE9)m_gc_plug->m_gcID)->SetViewport(&vp);
	}

	void clear(int mask
		,float r,float g,float b,float a
		,float z,float stencil)
	{
		DWORD flags = 0; 
		if ( (mask & CM_COLOR) == CM_COLOR)
		{
			flags = flags | D3DCLEAR_TARGET;
		}
		//if ( (mask & CM_ZBUFFER) == CM_ZBUFFER)
		//{
		//	flags = flags | D3DCLEAR_ZBUFFER;
		//}
		//if ( (mask & CM_STENCIL) == CM_STENCIL)
		//{
		//	flags = flags| D3DCLEAR_STENCIL;
		//}

		//D3DRECT rect;
		//rect.x1 = 0;
		//rect.x2 = 100;
		//rect.y1 = 0;
		//rect.y2 = 100;

		//D3DCOLOR co = D3DCOLOR_ARGB( int(a*255), int(r*255),int(g*255),int(b*255));
		D3DCOLOR co = D3DCOLOR_COLORVALUE( r,g,b,a );
		((LPDIRECT3DDEVICE9)m_gc_plug->m_gcID)->Clear(
			0,0,flags
			,co,z,stencil //类型有问题
			);
	}


	void PopAttrib(int d){}
	void PushAttrib(int d){}

	void enable(ENABLE_CAP cap)
	{
	}

	void disable(ENABLE_CAP cap)
	{
	}


	void setAlphaFunc(TEST_MODE mode,float v)
	{

	}
	void setBlendFunc(BLEND_FACTOR sfactor ,BLEND_FACTOR dfactor)
	{
	}

	void setClipPlane(int num,float p0,float p1,float p2,float p3)
	{
	}

	void setCullFace( FACE face )
	{
	}
	void setDepthFunc(TEST_MODE mode)
	{
		//glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		//glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do

		((LPDIRECT3DDEVICE9)m_gc_plug->m_gcID)->SetRenderState(D3DRS_ZENABLE ,D3DZB_TRUE );
		((LPDIRECT3DDEVICE9)m_gc_plug->m_gcID)->SetRenderState(D3DRS_ZFUNC  ,D3DCMP_LESSEQUAL );
	}
	void setDepthBufferWrite(bool write)
	{
	}
	void setDepthRange(float _near,float _far)
	{
	}
	void setFrontFace(FRONT_FACE ff)
	{
	}
	//void setLineStipple(int d){}
	void setLineWidth(float width)
	{
	}
	void setLogicOp( LOGIC_OP op )
	{
	}
	void setPointSize(float size)
	{
	}
	//void setPolygonMode(int d){}
	void setPolygonOffset(int d)
	{
	}
	void setShadeModel(int d)
	{
		//glShadeModel(GL_SMOOTH);                        // Enable Smooth Shading
	}

	void beginScene()
	{
		((LPDIRECT3DDEVICE9)m_gc_plug->m_gcID)->BeginScene();
	}
	void endScene()
	{
		((LPDIRECT3DDEVICE9)m_gc_plug->m_gcID)->EndScene();
	}

	void setMatrix(MATRIX_MODEL model,float* m)
	{
		if (model == MM_MODELVIEW)
		{
			((LPDIRECT3DDEVICE9)m_gc_plug->m_gcID)->SetTransform(D3DTS_VIEW   ,(D3DMATRIX *)m );
		}
		else if (model == MM_PROJECTION)
		{
			((LPDIRECT3DDEVICE9)m_gc_plug->m_gcID)->SetTransform(D3DTS_PROJECTION   ,(D3DMATRIX *)m );
		}
	}

	//void openLight(int num)
	//{
	//	if (num >= 0)
	//	{
	//		((LPDIRECT3DDEVICE9)m_gc_plug->m_gcID)
	//			->LightEnable(num,true);
	//	}
	//}

	//void closeLight(int num)
	//{
	//	if (num >= 0)
	//	{
	//		((LPDIRECT3DDEVICE9)m_gc_plug->m_gcID)->LightEnable(num,false);
	//	}
	//	if (num < 0)
	//	{
	//		//glDisable(GL_LIGHTING);
	//	}
	//}

	void setLight(
		int num
		,float* ambient
		,float* diffuse
		,float* position
		)
	{
		//openLight(num);

		D3DLIGHT9 l;
		l.Ambient.r = ambient[0];
		l.Ambient.g = ambient[1];
		l.Ambient.b = ambient[2];
		l.Ambient.a = ambient[3];

		l.Diffuse.r = diffuse[0];
		l.Diffuse.g = diffuse[1];
		l.Diffuse.b = diffuse[2];
		l.Diffuse.a = diffuse[3];

		l.Position.x = position[0];
		l.Position.y = position[1];
		l.Position.z = position[2];

		((LPDIRECT3DDEVICE9)m_gc_plug->m_gcID)
			->SetLight(num,&l);
	}

	void setMaterial(
		float* ambient
		,float* diffuse
		,float* specular
		,float* shininess
		,float* emission
		)
	{
		D3DMATERIAL9 m;

		m.Ambient.r = ambient[0];
		m.Ambient.g = ambient[1];
		m.Ambient.b = ambient[2];
		m.Ambient.a = ambient[3];

		m.Diffuse.r = diffuse[0];
		m.Diffuse.g = diffuse[1];
		m.Diffuse.b = diffuse[2];
		m.Diffuse.a = diffuse[3];

		m.Specular.r = specular[0];
		m.Specular.g = specular[1];
		m.Specular.b = specular[2];
		m.Specular.a = specular[3];

		//m.Power = (shininess[0]+shininess[1]+shininess[2]+shininess[3])/4.0;
		m.Power = shininess[0];
		//m.Power.g = shininess[1];
		//m.Power.b = shininess[2];
		//m.Power.a = shininess[3];

		m.Emissive.r = emission[0];
		m.Emissive.g = emission[1];
		m.Emissive.b = emission[2];
		m.Emissive.a = emission[3];

		((LPDIRECT3DDEVICE9)m_gc_plug->m_gcID)
			->SetMaterial(&m);
	}


	handle_texture createTexture()
	{
		//GLuint tex;
		//glGenTextures(1,&tex);

		//glBindTexture(GL_TEXTURE_2D , tex);
		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		//glBindTexture(GL_TEXTURE_2D , 0);

		return 0;
	}

	void distoryTexture(handle_texture texture)
	{
		//glDeleteTextures(1,&texture);
	}

	void bindTexture(handle_texture texture,unsigned int cannel,TEXTURE_ENV env)
	{
		//glActiveTexture (GL_TEXTURE0 + cannel);
		//glBindTexture(GL_TEXTURE_2D , texture);

		//GLint mode = GL_MODULATE;
		//if( env == ADD )			{mode = GL_ADD;}
		//else if( env == MODULATE )	{mode = GL_MODULATE;}
		//else if( env == DECAL )		{mode = GL_DECAL;}
		//else if( env == BLEND )		{mode = GL_BLEND;}
		//else if( env == REPLACE )	{mode = GL_REPLACE;}
		//else if( env == COMBINE )	{mode = GL_COMBINE;}
		//else{}

		////glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
	}

	void setTextureData(handle_texture texture , int w,int h,char* data ,int channal_num ,int channal_bits)
	{
		//glBindTexture(GL_TEXTURE_2D , texture);
		//glTexImage2D( GL_TEXTURE_2D
		//	, 0 //mipmap
		//	, 4
		//	, w, h
		//	, 0
		//	, GL_RGBA
		//	, GL_UNSIGNED_BYTE
		//	, data );
		//glBindTexture(GL_TEXTURE_2D ,0);
	}


	handle_geometry createGeometry(unsigned int size_in_byte,char* data,FVF_MASK format)
	{
		LPDIRECT3DDEVICE9 d3d9_device = (LPDIRECT3DDEVICE9)m_gc_plug->m_gcID;

		//FVF
		DWORD D3DFVF_CUSTOMVERTEX = getFVF( format );

		//VBO
		IDirect3DVertexBuffer9* vb = 0;
		if( FAILED( d3d9_device->CreateVertexBuffer( 
			size_in_byte
			, 0 /*Usage*/
			, D3DFVF_CUSTOMVERTEX
			, D3DPOOL_DEFAULT
			, &vb
			, NULL ) ) )
		{
			return E_FAIL;
		}

		//map
		VOID* pVertices;
		if( FAILED( vb->Lock( 
			0
			, size_in_byte
			, (void**)&pVertices, 0 ) ) )
		{
			return E_FAIL;
		}
		
		//copy
		memcpy( pVertices, data, size_in_byte );

		//unmap
		vb->Unlock();

		return (handle_geometry)vb;
	}

	void distoryGeometry(handle_geometry geo)
	{

	}

	void bindGeometry(handle_geometry geo, unsigned int /*size*/ ,FVF_MASK format)
	{
		LPDIRECT3DDEVICE9 d3d9_device = (LPDIRECT3DDEVICE9)m_gc_plug->m_gcID;

		//FVF
		DWORD D3DFVF_CUSTOMVERTEX = getFVF( format );

		IDirect3DVertexBuffer9* vb = (IDirect3DVertexBuffer9*)geo;

		d3d9_device->SetStreamSource( 0, vb, 0, format.sizeInByte() );
		d3d9_device->SetFVF( D3DFVF_CUSTOMVERTEX );
	}

	void drawGeometry(handle_geometry /*geo*/,PRIMITIVE primitive ,int begin ,int size,FVF_MASK /*format*/)
	{
		LPDIRECT3DDEVICE9 d3d9_device = (LPDIRECT3DDEVICE9)m_gc_plug->m_gcID;
		//IDirect3DVertexBuffer9* vb = (IDirect3DVertexBuffer9*)geo;

		D3DPRIMITIVETYPE mode = D3DPT_LINELIST;
		if( primitive == POINTS )				{mode = D3DPT_POINTLIST;}
		else if( primitive == LINES )			{mode = D3DPT_LINELIST;}
		else if( primitive == LINESTRIP )		{mode = D3DPT_LINESTRIP;}
		else if( primitive == TRIANGLES )		{mode = D3DPT_TRIANGLELIST;}
		else if( primitive == TRIANGLESTRIP )	{mode = D3DPT_TRIANGLESTRIP;}
		else if( primitive == TRIANGLEFAN )		{mode = D3DPT_TRIANGLEFAN;}
		else{}

		d3d9_device->DrawPrimitive( mode, begin, size );
	}

	char* mapGeometryData(handle_geometry geo)
	{
		return 0;
	}

	void unmapGeometryData(handle_geometry geo)
	{
	}


	handle_shader createShader(int type){return 0;}

	void distoryShader(handle_shader shader){}

	void bindShader(handle_shader shader){}

	void setShaderString(handle_shader shader,const char* data){}


	bool setVSync(bool b)
	{
		return false;
	}


	//protected:
	private:

		DWORD getFVF( FVF_MASK format )
		{
			DWORD D3DFVF_CUSTOMVERTEX = 0;

			if(format._vertex_count == 3) D3DFVF_CUSTOMVERTEX = D3DFVF_CUSTOMVERTEX | D3DFVF_XYZ;
			else if(format._vertex_count == 4) D3DFVF_CUSTOMVERTEX = D3DFVF_CUSTOMVERTEX | D3DFVF_XYZW;
			else{}

			if(format._has_normal) D3DFVF_CUSTOMVERTEX |= D3DFVF_NORMAL;

			if(format._has_color)  D3DFVF_CUSTOMVERTEX |= D3DFVF_DIFFUSE;

			if( format._uv_count > 0 )
			{
				if( format._uv_count == 1 )D3DFVF_CUSTOMVERTEX |= D3DFVF_TEX1;
				else if( format._uv_count == 2 )D3DFVF_CUSTOMVERTEX |= D3DFVF_TEX2;
				else if( format._uv_count == 3 )D3DFVF_CUSTOMVERTEX |= D3DFVF_TEX3;
				else if( format._uv_count == 4 )D3DFVF_CUSTOMVERTEX |= D3DFVF_TEX4;
				else if( format._uv_count == 5 )D3DFVF_CUSTOMVERTEX |= D3DFVF_TEX5;
				else if( format._uv_count == 6 )D3DFVF_CUSTOMVERTEX |= D3DFVF_TEX6;
				else if( format._uv_count == 7 )D3DFVF_CUSTOMVERTEX |= D3DFVF_TEX7;
				else if( format._uv_count == 8 )D3DFVF_CUSTOMVERTEX |= D3DFVF_TEX8;
			}

			return D3DFVF_CUSTOMVERTEX;
		}

};


//注册
//REGISTER_NIUBI_PLUG(/* "Renderer_dx_9" ,*/ Renderer_dx_9 )
NIUBI_REGISTER_PLUGINS_FUNCTION_BEGIN( pm ,Renderer_dx_9)
	NIUBI_REGISTER_PLUGINS(pm,Renderer_dx_9)
	NIUBI_REGISTER_PLUGINS_FUNCTION_END
