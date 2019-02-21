
#include <nbPlugins/public.h>
#include <nbBase/public.h>

/*
支持 OpenGL 1.1
*  https://www.khronos.org/opengles/sdk/1.1/docs/man/
*
* VBO PBO FBO 相关资料
*  http://www.songho.ca/opengl/gl_vbo.html
*/

/*
A
glActiveTexture
glAlphaFunc

B
glBindBuffer
glBindTexture
glBlendFunc
glBufferData
glBufferSubData

C
glClear
glClearColor
glClearDepth
glClearStencil
glClientActiveTexture
glClipPlane
glColor
glColorMask
glColorPointer
glCompressedTexImage2D
glCompressedTexSubImage2D
glCopyTexImage2D
glCopyTexSubImage2D
glCullFace

D
glDeleteBuffers
glDeleteTextures
glDepthFunc
glDepthMask
glDepthRange
glDisable
glDisableClientState
glDrawArrays
glDrawElements

E
glEnable
glEnableClientState

F
glFinish
glFlush
glFog
glFrontFace
glFrustum


G
glGenBuffers
glGenTextures
glGet
glGetBufferParameteriv
glGetClipPlane
glGetError
glGetLight
glGetMaterial
glGetPointerv
glGetString
glGetTexEnv
glGetTexParameter


H
glHint


I
glIsBuffer
glIsEnabled
glIsTexture


L
glLight
glLightModel
glLineWidth
glLoadIdentity
glLoadMatrix
glLogicOp


M
glMaterial
glMatrixMode
glMultMatrix
glMultiTexCoord

N
glNormal
glNormalPointer

O
glOrtho

P
glPixelStorei
glPointParameter
glPointSize
glPointSizePointerOES
glPolygonOffset
glPopMatrix
glPushMatrix

R
glReadPixels
glRotate

S
glSampleCoverage
glScale
glScissor
glShadeModel
glStencilFunc
glStencilMask
glStencilOp

T
glTexCoordPointer
glTexEnv
glTexImage2D
glTexParameter
glTexSubImage2D
glTranslate

V
glVertexPointer
glViewport

*/



#if _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
//#pragma comment (lib, "libGLESv1_CM.lib")   /* link Windows user lib       */
#endif

#include <GLES/gl.h>


class Renderer_gles_1_x
    :public nbPlugins::Renderer
{
public:

    Renderer_gles_1_x(){}
    ~Renderer_gles_1_x(){}

    void init()
    {
        LOGW("Renderer_gles_1_x init %d", 1);
    }

    void setViewport(int x, int y, int width, int height) override
    {
        glViewport(x, y, width, height);
    }

    void beginScene() override
    {
        //nothing
    }
    void endScene() override
    {
        //nothing
    }

    void clear(int mask
        , float r, float g, float b, float a
        , float z, float stencil) override
    {

        GLbitfield flags = 0;

        if ((mask & CM_COLOR) == CM_COLOR)
        {
            flags = flags | GL_COLOR_BUFFER_BIT;
            glClearColor(r, g, b, a);
        }
        if ((mask & CM_ZBUFFER) == CM_ZBUFFER)
        {
            flags = flags | GL_DEPTH_BUFFER_BIT;
            glClearDepthf(z);
        }
        //if ( (mask & CM_STENCIL) == CM_STENCIL)
        //{
        //	flags = flags| D3DCLEAR_STENCIL;
        //}
        glClear(flags);
    }

    void PopAttrib(int d) override
    {
        //nothing
    }
    void PushAttrib(int d) override
    {
        //nothing
    }


    void enable(ENABLE_CAP cap) override
    {
        switch (cap)
        {
        case FOG:glEnable(GL_FOG); break;
        case TEXTURE_2D:glEnable(GL_TEXTURE_2D); break;
        case CULL_FACE:glEnable(GL_CULL_FACE); break;
        case BLEND:glEnable(GL_BLEND); break;
        case COLOR_LOGIC_OP:glEnable(GL_COLOR_LOGIC_OP); break;
        case DITHER:glEnable(GL_DITHER); break;
        case POLYGON_OFFSET_FILL:glEnable(GL_POLYGON_OFFSET_FILL); break;

        case ALPHA_TEST:glEnable(GL_ALPHA_TEST); break;
        case STENCIL_TEST:glEnable(GL_STENCIL_TEST); break;
        case DEPTH_TEST:glEnable(GL_DEPTH_TEST); break;
        case SCISSOR_TEST:glEnable(GL_SCISSOR_TEST); break;

        case COLOR_MATERIAL:glEnable(GL_COLOR_MATERIAL); break;
        case LIGHTING:glEnable(GL_LIGHTING); break;
        case LIGHT0:glEnable(GL_LIGHT0); break;
        case LIGHT1:glEnable(GL_LIGHT1); break;
        case LIGHT2:glEnable(GL_LIGHT2); break;
        case LIGHT3:glEnable(GL_LIGHT3); break;
        case LIGHT4:glEnable(GL_LIGHT4); break;
        case LIGHT5:glEnable(GL_LIGHT5); break;
        case LIGHT6:glEnable(GL_LIGHT6); break;
        case LIGHT7:glEnable(GL_LIGHT7); break;

        case POINT_SMOOTH:glEnable(GL_POINT_SMOOTH); break;
        case LINE_SMOOTH:glEnable(GL_LINE_SMOOTH); break;
        case NORMALIZE:glEnable(GL_NORMALIZE); break;
        case RESCALE_NORMAL:glEnable(GL_RESCALE_NORMAL); break;

        case VERTEX_ARRAY:glEnable(GL_VERTEX_ARRAY); break;
        case NORMAL_ARRAY:glEnable(GL_NORMAL_ARRAY); break;
        case COLOR_ARRAY:glEnable(GL_COLOR_ARRAY); break;
        case TEXTURE_COORD_ARRAY:glEnable(GL_TEXTURE_COORD_ARRAY); break;

        case MULTISAMPLE:glEnable(GL_MULTISAMPLE); break;
        case SAMPLE_ALPHA_TO_COVERAGE:glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE); break;
        case SAMPLE_ALPHA_TO_ONE:glEnable(GL_SAMPLE_ALPHA_TO_ONE); break;
        case SAMPLE_COVERAGE:glEnable(GL_SAMPLE_COVERAGE); break;

        default:break;
        }
    }

    void disable(ENABLE_CAP cap) override
    {
        switch (cap)
        {
        case FOG:glDisable(GL_FOG); break;
        case TEXTURE_2D:glDisable(GL_TEXTURE_2D); break;
        case CULL_FACE:glDisable(GL_CULL_FACE); break;
        case BLEND:glDisable(GL_BLEND); break;
        case COLOR_LOGIC_OP:glDisable(GL_COLOR_LOGIC_OP); break;
        case DITHER:glDisable(GL_DITHER); break;
        case POLYGON_OFFSET_FILL:glDisable(GL_POLYGON_OFFSET_FILL); break;

        case ALPHA_TEST:glDisable(GL_ALPHA_TEST); break;
        case STENCIL_TEST:glDisable(GL_STENCIL_TEST); break;
        case DEPTH_TEST:glDisable(GL_DEPTH_TEST); break;
        case SCISSOR_TEST:glDisable(GL_SCISSOR_TEST); break;

        case COLOR_MATERIAL:glDisable(GL_COLOR_MATERIAL); break;
        case LIGHTING:glDisable(GL_LIGHTING); break;
        case LIGHT0:glDisable(GL_LIGHT0); break;
        case LIGHT1:glDisable(GL_LIGHT1); break;
        case LIGHT2:glDisable(GL_LIGHT2); break;
        case LIGHT3:glDisable(GL_LIGHT3); break;
        case LIGHT4:glDisable(GL_LIGHT4); break;
        case LIGHT5:glDisable(GL_LIGHT5); break;
        case LIGHT6:glDisable(GL_LIGHT6); break;
        case LIGHT7:glDisable(GL_LIGHT7); break;

        case POINT_SMOOTH:glDisable(GL_POINT_SMOOTH); break;
        case LINE_SMOOTH:glDisable(GL_LINE_SMOOTH); break;
        case NORMALIZE:glDisable(GL_NORMALIZE); break;
        case RESCALE_NORMAL:glDisable(GL_RESCALE_NORMAL); break;

        case VERTEX_ARRAY:glDisable(GL_VERTEX_ARRAY); break;
        case NORMAL_ARRAY:glDisable(GL_NORMAL_ARRAY); break;
        case COLOR_ARRAY:glDisable(GL_COLOR_ARRAY); break;
        case TEXTURE_COORD_ARRAY:glDisable(GL_TEXTURE_COORD_ARRAY); break;

        case MULTISAMPLE:glDisable(GL_MULTISAMPLE); break;
        case SAMPLE_ALPHA_TO_COVERAGE:glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE); break;
        case SAMPLE_ALPHA_TO_ONE:glDisable(GL_SAMPLE_ALPHA_TO_ONE); break;
        case SAMPLE_COVERAGE:glDisable(GL_SAMPLE_COVERAGE); break;

        default:
            break;
        }
    }



    void setAlphaFunc(TEST_MODE mode, float v) override
    {
        GLenum m = ALWAYS;
        if (mode == NEVER)	m = GL_NEVER;
        if (mode == LESS)	m = GL_LESS;
        if (mode == EQUAL)	m = GL_EQUAL;
        if (mode == LEQUAL)	m = GL_LEQUAL;
        if (mode == GREATER) m = GL_GREATER;
        if (mode == NOTEQUAL) m = GL_NOTEQUAL;
        if (mode == GEQUAL)	m = GL_GEQUAL;
        if (mode == ALWAYS)	m = GL_ALWAYS;
        glAlphaFunc(m, v);
    };

    void setBlendFunc(BLEND_FACTOR sfactor, BLEND_FACTOR dfactor) override
    {
        GLenum  s = GL_ONE;
        if (sfactor == ZERO)	s = GL_ZERO;
        if (sfactor == ONE)	s = GL_ONE;
        if (sfactor == DST_COLOR)	s = GL_DST_COLOR;
        if (sfactor == ONE_MINUS_DST_COLOR)	s = GL_ONE_MINUS_DST_COLOR;
        if (sfactor == SRC_ALPHA)	s = GL_SRC_ALPHA;
        if (sfactor == ONE_MINUS_SRC_ALPHA)	s = GL_ONE_MINUS_SRC_ALPHA;
        if (sfactor == DST_ALPHA)	s = GL_DST_ALPHA;
        if (sfactor == ONE_MINUS_DST_ALPHA)	s = ONE_MINUS_DST_ALPHA;
        if (sfactor == SRC_ALPHA_SATURATE)	s = GL_SRC_ALPHA_SATURATE;

        GLenum  d = GL_ZERO;
        if (dfactor == ZERO)	d = GL_ZERO;
        if (dfactor == ONE)	d = GL_ONE;
        if (dfactor == DST_COLOR)	d = GL_DST_COLOR;
        if (dfactor == ONE_MINUS_DST_COLOR)	d = GL_ONE_MINUS_DST_COLOR;
        if (dfactor == SRC_ALPHA)	d = GL_SRC_ALPHA;
        if (dfactor == ONE_MINUS_SRC_ALPHA)	d = GL_ONE_MINUS_SRC_ALPHA;
        if (dfactor == DST_ALPHA)	d = GL_DST_ALPHA;
        if (dfactor == ONE_MINUS_DST_ALPHA)	d = ONE_MINUS_DST_ALPHA;

        glBlendFunc(s, d);
    }

    void setClipPlane(int num, float p0, float p1, float p2, float p3) override
    {
        GLfloat p[4];
        p[0] = p0; p[1] = p1; p[2] = p2; p[3] = p3;
        glClipPlanef(GL_CLIP_PLANE0 + num, p);
    };

    void setCullFace(FACE face) override
    {
        GLenum f = GL_BACK;
        if (face == FRONT)	f = GL_FRONT;
        if (face == BACK)	f = GL_BACK;
        if (face == FRONT_AND_BACK)	f = GL_FRONT_AND_BACK;
        glCullFace(f);
    };

    void setDepthFunc(TEST_MODE mode) override
    {
        //glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
        GLenum m = ALWAYS;
        if (mode == NEVER)	m = GL_NEVER;
        if (mode == LESS)	m = GL_LESS;
        if (mode == EQUAL)	m = GL_EQUAL;
        if (mode == LEQUAL)	m = GL_LEQUAL;
        if (mode == GREATER) m = GL_GREATER;
        if (mode == NOTEQUAL) m = GL_NOTEQUAL;
        if (mode == GEQUAL)	m = GL_GEQUAL;
        if (mode == ALWAYS)	m = GL_ALWAYS;
        glDepthFunc(m);								// The Type Of Depth Testing To Do
    }
    void setDepthBufferWrite(bool write) override
    {
        glDepthMask(write);
    };

    void setDepthRange(float _near, float _far) override
    {
        glDepthRangef(_near, _far);
    }
    void setFrontFace(FRONT_FACE ff) override
    {
        GLenum f = GL_CCW;
        if (CW)	f = GL_CW;
        if (CCW)	f = GL_CCW;
        glFrontFace(f);
    }
    //void setLineStipple(int d){};
    void setLineWidth(float width) override
    {
        glLineWidth(width);
    }
    void setLogicOp(LOGIC_OP op) override
    {
        //glLogicOp(GLenum  opcode);
    }
    void setPointSize(float size) override
    {
        glPointSize(size);
    }
    //void setPolygonMode(int d)
    //{
    //	glPolygonMode(GLenum  face,  GLenum  mode);
    //}
    void setPolygonOffset(int d) override
    {
        //glPolygonOffset(GLfloat  factor,  GLfloat  units);
    }
    void setShadeModel(int d) override
    {
        glShadeModel(GL_SMOOTH);                        // Enable Smooth Shading
    }

    void setMatrix(MATRIX_MODEL model, float* m) override
    {
        if (model == MM_MODELVIEW)
        {
            glMatrixMode(GL_MODELVIEW);
            glLoadMatrixf(m);
        }

        else if (model == MM_PROJECTION)
        {
            glMatrixMode(GL_PROJECTION);
            glLoadMatrixf(m);
        }
    }

    void setLight(
        int num
        , float* ambient
        , float* diffuse
        , float* position
        ) override
    {
        //openLight(num);
        glLightfv(GL_LIGHT0 + num, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0 + num, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0 + num, GL_POSITION, position);
    }

    void setMaterial(
        float* ambient
        , float* diffuse
        , float* specular
        , float* shininess
        , float* emission
        ) override
    {
        //glMaterialf(GL_FRONT, GL_SHININESS, 1)
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    }

    handle_texture createTexture() override
    {
        GLuint tex = 0;
        glGenTextures(1, &tex);

        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        return tex;
    }

    void distoryTexture(handle_texture texture) override
    {
        glDeleteTextures(1, &texture);
    }

    void bindTexture(handle_texture texture, unsigned int cannel, TEXTURE_ENV env) override
    {
        glActiveTexture(GL_TEXTURE0 + cannel);
        glBindTexture(GL_TEXTURE_2D, texture);

        GLint mode = GL_MODULATE;
        if (env == ENV_ADD)			{ mode = GL_ADD; }
        else if (env == ENV_MODULATE)	{ mode = GL_MODULATE; }
        else if (env == ENV_DECAL)		{ mode = GL_DECAL; }
        //else if( env == BLEND )		{mode = GL_BLEND;}
        //else if( env == REPLACE )	{mode = GL_REPLACE;}
        //else if( env == COMBINE )	{mode = GL_COMBINE;}
        else{}

        //glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
    }

    void setTextureData(handle_texture texture, int w, int h, char* data
        , int channal_num, int channal_bits) override
    {

        GLenum format = GL_RGB;
        GLenum type = GL_UNSIGNED_BYTE;
        if (channal_num == 1) format = GL_LUMINANCE;
        if (channal_num == 3) format = GL_RGB;
        if (channal_num == 4) format = GL_RGBA;
        if (channal_bits == 8) type = GL_UNSIGNED_BYTE;


        //glActiveTexture (GL_TEXTURE0 + cannel);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D
            , 0 //mipmap
            , format
            , w, h
            , 0
            , format//GL_RGBA
            , type//GL_UNSIGNED_BYTE
            , data);
        glBindTexture(GL_TEXTURE_2D, 0);

    }

    handle_geometry createGeometry(VBO_TYPE type, unsigned int size_in_byte, char* data/*, FVF_MASK format*/) override
    {
        GLenum target = GL_ARRAY_BUFFER;
        if (type == INDEX) target = GL_ELEMENT_ARRAY_BUFFER;

        handle_geometry vbo = 0;
        glGenBuffers(1, &vbo);
        glBindBuffer(target, vbo);
        glBufferData(target, size_in_byte, data, GL_STATIC_DRAW);

        return vbo;
    }

    void distoryGeometry(handle_geometry geo) override
    {
        glDeleteBuffers(1, &geo);
    }

    void bindGeometry(handle_geometry geo, VBO_TYPE type, unsigned int size, FVF_MASK format) override
    {
        GLenum target = GL_ARRAY_BUFFER;
        if (type == INDEX) target = GL_ELEMENT_ARRAY_BUFFER;

        glBindBuffer(target, geo);

        unsigned int offset = 0;
        unsigned int stride = format.sizeInByte();

        //顶点
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(format._vertex_count, GL_FLOAT, stride, (char *)NULL); // Set The Vertex Pointer To The Vertex Buffer
        offset += format._vertex_count * 4;

        //法线
        if (format._has_normal)
        {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, stride, (char *)offset);
            offset += 3 * 4;
        }

        //色彩
        if (format._has_color)
        {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_FLOAT, stride, (char *)offset);
            offset += 4 * 4;
        }

        //UV
        if (format._uv_count > 0)
        {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, format.sizeInByte(), (char *)offset); // Set The TexCoord Pointer To The TexCoord Buffer 
        }

    }

    void unbindGeometry(handle_geometry geo, FVF_MASK format) override
    {
        //LOGW("drawGeometry=3");
        glDisableClientState(GL_VERTEX_ARRAY);
        if (format._has_normal)glDisableClientState(GL_NORMAL_ARRAY);
        if (format._has_color)glDisableClientState(GL_COLOR_ARRAY);
        if (format._uv_count > 0)glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    void bindGeometry(handle_geometry geo, unsigned int size, VBO_DATA_TYPE type) override
    {
        //glBindBuffer(GL_ARRAY_BUFFER, geo);

        //switch (type)
        //{
        //case nbPlugins::Renderer::VERTEX:
        //    //顶点
        //    glEnableClientState(GL_VERTEX_ARRAY);
        //    glVertexPointer(3, GL_FLOAT, 0, 0);
        //    break;
        //case nbPlugins::Renderer::NORMAL:
        //    glEnableClientState(GL_NORMAL_ARRAY);
        //    glNormalPointer(GL_FLOAT, 0, 0);
        //    break;
        //case nbPlugins::Renderer::COLOR:
        //    glEnableClientState(GL_COLOR_ARRAY);
        //    glColorPointer(4, GL_FLOAT, 0, 0);
        //    break;
        //case nbPlugins::Renderer::TEXTURE_COORD:
        //    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        //    glTexCoordPointer(2, GL_FLOAT, 0, 0);
        //    break;
        //default:
        //    break;
        //}
    }

    void unbindGeometry(handle_geometry geo, VBO_DATA_TYPE type) override
    {
        //switch (type)
        //{
        //case nbPlugins::Renderer::VERTEX:
        //    glDisableClientState(GL_VERTEX_ARRAY);
        //    break;
        //case nbPlugins::Renderer::NORMAL:
        //    glDisableClientState(GL_NORMAL_ARRAY);
        //    break;
        //case nbPlugins::Renderer::COLOR:
        //    glDisableClientState(GL_COLOR_ARRAY);
        //    break;
        //case nbPlugins::Renderer::TEXTURE_COORD:
        //    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        //    break;
        //default:
        //    break;
        //}
    }

    void drawGeometryArray(/*handle_geometry geo,*/ PRIMITIVE primitive, int begin, int size/*, FVF_MASK format*/) override
    {
        // LOGW("drawGeometry=1");

        GLenum mode = GL_TRIANGLES;
        if (primitive == POINTS)				{ mode = GL_POINTS; }
        else if (primitive == LINES)			{ mode = GL_LINES; }
        else if (primitive == LINESTRIP)		{ mode = GL_LINE_STRIP; }
        else if (primitive == TRIANGLES)		{ mode = GL_TRIANGLES; }
        else if (primitive == TRIANGLESTRIP)	{ mode = GL_TRIANGLE_STRIP; }
        else if (primitive == TRIANGLEFAN)		{ mode = GL_TRIANGLE_FAN; }
        else{}

        // LOGW("drawGeometry=2 %d %d %d %d",geo,mode, begin, size);
        glDrawArrays(mode, begin, size); // Draw All Of The Triangles At Once 
        //glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);


        //GLenum error = glGetError();
        //if( error !=0 )
        //{
        //    LOGW("error=%d", error);
        //}
    }

    void drawGeometryElements(/*handle_geometry geo,*/ PRIMITIVE primitive, int begin, int size/*, FVF_MASK format*/) override
    {
    }

    char* mapGeometryData(handle_geometry geo) override
    {
        return 0;
    };

    void unmapGeometryData(handle_geometry geo) override
    {
    };


    handle_shader createShader(int type) override
    {
        return 0;
    }

    void distoryShader(handle_shader shader) override
    {
    }

    void bindShader(handle_shader shader) override
    {
    }

    void setShaderString(handle_shader shader, const char* data) override
    {
    }

    bool setVSync(bool b) override
    {
        return false;
    }



protected:
private:
};


//注册
NIUBI_REGISTER_PLUGINS_FUNCTION_BEGIN(pm, Renderer_gles_1_x)
NIUBI_REGISTER_PLUGINS(pm, Renderer_gles_1_x)
NIUBI_REGISTER_PLUGINS_FUNCTION_END