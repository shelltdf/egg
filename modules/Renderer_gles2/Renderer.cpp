
#include <nbPlugins/public.h>
#include <nbBase/public.h>

/*
支持 OpenGL ES 2.0
*  https://www.khronos.org/opengles/sdk/docs/man/
*
* VBO PBO FBO 相关资料
*  http://www.songho.ca/opengl/gl_vbo.html
*/


#if _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

/*
    对于一些被放弃的固定流水线 以后考虑用shader来模拟
    */

bool checkGLError(int l)
{
    GLenum error = glGetError();
    if (error == GL_NO_ERROR)
    {
        return true;
    }
    if (error == GL_INVALID_ENUM)
    {
        LOGW("error GL_INVALID_ENUM = 0x%x line %d", error, l);
    }
    if (error == GL_INVALID_VALUE)
    {
        LOGW("error GL_INVALID_VALUE = 0x%x line %d", error, l);
    }
    if (error == GL_INVALID_OPERATION)
    {
        LOGW("error GL_INVALID_OPERATION = 0x%x line %d", error, l);
    }
    if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
    {
        LOGW("error GL_INVALID_FRAMEBUFFER_OPERATION = 0x%x line %d", error, l);
    }
    if (error == GL_OUT_OF_MEMORY)
    {
        LOGW("error GL_OUT_OF_MEMORY = 0x%x line %d", error, l);
    }

    return false;
}

//输出着色器LOG
void printShaderInfoLog(GLuint shader)
{
    GLint bufSize = 0;
    char *infoLog   = 0;
    GLsizei length  = 0;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);
    if (bufSize > 0) 
    {
        infoLog = new char[bufSize];
        if (infoLog != NULL) 
        {
            glGetShaderInfoLog(shader, bufSize, &length, infoLog);
            LOGE("%s", infoLog);
            delete infoLog;
        }
        else
        {
            LOGE("Could not allocate InfoLog buffer.");
        }
    }
}



//输出程序LOG
void printProgramInfoLog(GLuint program)
{
    GLsizei bufSize = 0;
    char *infoLog   = 0;
    GLsizei length  = 0;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH , &bufSize);

    if (bufSize > 0) 
    {
        infoLog = new char[bufSize];
        if (infoLog != NULL) 
        {
            glGetProgramInfoLog(program, bufSize, &length, infoLog);
            LOGE("%s", infoLog);
            delete infoLog;
        }
        else
        {
            LOGE("Could not allocate InfoLog buffer.");
        }
    }
}


class Renderer_gles_2_x
    :public nbPlugins::Renderer
{
public:

    Renderer_gles_2_x(){}
    ~Renderer_gles_2_x(){}

    void init()
    {
        LOGW("Renderer_gles_2_x init %d", 1);
    }

    void setViewport(int x, int y, int width, int height) override
    {
        glViewport(x, y, width, height);
        checkGLError(__LINE__);
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
        if ((mask & CM_STENCIL) == CM_STENCIL)
        {
            flags = flags | GL_STENCIL_BUFFER_BIT;
            glClearStencil((GLint)stencil);
        }
        glClear(flags);

        checkGLError(__LINE__);
    }

    void PopAttrib(int d) override
    {
        //no support
    }
    void PushAttrib(int d) override
    {
        //no support
    }


    void enable(ENABLE_CAP cap) override
    {
        checkGLError(__LINE__);

        switch (cap)
        {
        case TEXTURE_2D:glEnable(GL_TEXTURE_2D); break;
        case CULL_FACE:glEnable(GL_CULL_FACE); break;
        case BLEND:glEnable(GL_BLEND); break;
        case DITHER:glEnable(GL_DITHER); break;
        case POLYGON_OFFSET_FILL:glEnable(GL_POLYGON_OFFSET_FILL); break;

        case STENCIL_TEST:glEnable(GL_STENCIL_TEST); break;
        case DEPTH_TEST:glEnable(GL_DEPTH_TEST); break;
        case SCISSOR_TEST:glEnable(GL_SCISSOR_TEST); break;

        case SAMPLE_ALPHA_TO_COVERAGE:glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE); break;
        case SAMPLE_COVERAGE:glEnable(GL_SAMPLE_COVERAGE); break;

        default:break;
        }

        checkGLError(__LINE__);
    }

    void disable(ENABLE_CAP cap) override
    {
        switch (cap)
        {

        case TEXTURE_2D:glDisable(GL_TEXTURE_2D); break;
        case CULL_FACE:glDisable(GL_CULL_FACE); break;
        case BLEND:glDisable(GL_BLEND); break;
        case DITHER:glDisable(GL_DITHER); break;
        case POLYGON_OFFSET_FILL:glDisable(GL_POLYGON_OFFSET_FILL); break;

        case STENCIL_TEST:glDisable(GL_STENCIL_TEST); break;
        case DEPTH_TEST:glDisable(GL_DEPTH_TEST); break;
        case SCISSOR_TEST:glDisable(GL_SCISSOR_TEST); break;

        case SAMPLE_ALPHA_TO_COVERAGE:glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE); break;
        case SAMPLE_COVERAGE:glDisable(GL_SAMPLE_COVERAGE); break;

        default:
            break;
        }

        checkGLError(__LINE__);
    }

    void setAlphaFunc(TEST_MODE mode, float v) override
    {
        //no support
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

        checkGLError(__LINE__);
    }

    void setClipPlane(int num, float p0, float p1, float p2, float p3) override
    {
        //no support
    };

    void setCullFace(FACE face) override
    {
        GLenum f = GL_BACK;
        if (face == FRONT)	f = GL_FRONT;
        if (face == BACK)	f = GL_BACK;
        if (face == FRONT_AND_BACK)	f = GL_FRONT_AND_BACK;
        glCullFace(f);

        checkGLError(__LINE__);
    };

    void setDepthFunc(TEST_MODE mode) override
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
        glDepthFunc(m);

        checkGLError(__LINE__);
    }
    void setDepthBufferWrite(bool write) override
    {
        glDepthMask(write);

        checkGLError(__LINE__);
    };

    void setDepthRange(float _near, float _far) override
    {
        glDepthRangef(_near, _far);

        checkGLError(__LINE__);
    }
    void setFrontFace(FRONT_FACE ff) override
    {
        GLenum f = GL_CCW;
        if (CW)	f = GL_CW;
        if (CCW)	f = GL_CCW;
        glFrontFace(f);

        checkGLError(__LINE__);
    }
    //void setLineStipple(int d){};
    void setLineWidth(float width) override
    {
        glLineWidth(width);

        checkGLError(__LINE__);
    }
    void setLogicOp(LOGIC_OP op) override
    {
        //no support
    }
    void setPointSize(float size) override
    {
        //no support
    }
    //void setPolygonMode(int d)
    //{
    //	glPolygonMode(GLenum  face,  GLenum  mode);
    //}
    void setPolygonOffset(int d) override
    {
        //no support
    }
    void setShadeModel(int d) override
    {
        //no support
    }

    void setMatrix(MATRIX_MODEL model, float* m) override
    {
        //no support
    }

    void setLight(
        int num
        , float* ambient
        , float* diffuse
        , float* position
        ) override
    {
        //no support
    }

    void setMaterial(
        float* ambient
        , float* diffuse
        , float* specular
        , float* shininess
        , float* emission
        ) override
    {
        //no support
    }

    handle_texture createTexture() override
    {
        GLuint tex = 0;
        glGenTextures(1, &tex);

        glBindTexture(GL_TEXTURE_2D, tex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

        glBindTexture(GL_TEXTURE_2D, 0);

        checkGLError(__LINE__);

        return tex;
    }

    void distoryTexture(handle_texture texture) override
    {
        glDeleteTextures(1, &texture);

        checkGLError(__LINE__);
    }

    void bindTexture(handle_texture texture, unsigned int cannel, TEXTURE_ENV env) override
    {
        glActiveTexture(GL_TEXTURE0 + cannel);
        glBindTexture(GL_TEXTURE_2D, texture);

        checkGLError(__LINE__);
    }

    void setTextureData(handle_texture texture, int w, int h, char* data
        //, int channal_num, int channal_bits
        , PixelFormat pixel_format , DataType number_data_type
        ) override
    {

        GLenum format = GL_RGB;
        switch (pixel_format)
        {
        case nbPlugins::Renderer::DEPTH_COMPONENT:
            format = GL_DEPTH_COMPONENT;
            break;
        case nbPlugins::Renderer::ALPHA:
            format = GL_ALPHA;
            break;
        case nbPlugins::Renderer::RGB:
            format = GL_RGB;
            break;
        case nbPlugins::Renderer::RGBA:
            format = GL_RGBA;
            break;
        case nbPlugins::Renderer::LUMINANCE:
            format = GL_LUMINANCE;
            break;
        case nbPlugins::Renderer::LUMINANCE_ALPHA:
            format = GL_LUMINANCE_ALPHA;
            break;
        default:
            break;
        }

        GLenum internal_format = format;
        if( format == GL_DEPTH_COMPONENT)
        {
            internal_format = GL_DEPTH_COMPONENT16;
            //internal_format = GL_DEPTH_COMPONENT24_OES;
        }


        GLenum type = GL_BYTE;
        switch (number_data_type)
        {
        case nbPlugins::Renderer::BYTE:
            type = GL_BYTE;
            break;
        case nbPlugins::Renderer::UNSIGNED_BYTE:
            type = GL_UNSIGNED_BYTE;
            break;
        case nbPlugins::Renderer::SHORT:
            type = GL_SHORT;
            break;
        case nbPlugins::Renderer::UNSIGNED_SHORT:
            type = GL_UNSIGNED_SHORT;
            break;
        case nbPlugins::Renderer::INT:
            type = GL_INT;
            break;
        case nbPlugins::Renderer::UNSIGNED_INT:
            type = GL_UNSIGNED_INT;
            break;
        case nbPlugins::Renderer::FLOAT:
            type = GL_FLOAT;
            break;
        case nbPlugins::Renderer::FIXED:
            type = GL_FIXED;
            break;
        default:
            break;
        }


        //glActiveTexture (GL_TEXTURE0 + cannel);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D
            , 0 //mipmap
            , internal_format
            , w, h
            , 0
            , format//GL_RGBA
            , type//GL_UNSIGNED_BYTE
            , data);
        glBindTexture(GL_TEXTURE_2D, 0);

        checkGLError(__LINE__);
    }


    virtual handle_framebuffer createFrameBuffer()
    {
        GLuint fbo;
        glGenFramebuffers(1, &fbo);

        //glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, 512);
        //glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, 512);
        //glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_SAMPLES, 4);

        return fbo;
    }

    virtual void distoryFrameBuffer(handle_framebuffer framebuffer)
    {
        glDeleteFramebuffers(1, &framebuffer);
    }

    virtual void bindFrameBuffer(handle_framebuffer framebuffer)
    {
        glBindFramebuffer( GL_FRAMEBUFFER , framebuffer);
    }

    virtual void attactFrameBuffer( handle_framebuffer framebuffer , handle_texture texture , ATTACHMENT attach )
    {
        GLenum a = GL_COLOR_ATTACHMENT0;
        if (attach == COLOR_ATTACHMENT0) a = GL_COLOR_ATTACHMENT0;
        if (attach == DEPTH_ATTACHMENT) a = GL_DEPTH_ATTACHMENT;
        if (attach == STENCIL_ATTACHMENT) a = GL_STENCIL_ATTACHMENT;

        glFramebufferTexture2D( GL_FRAMEBUFFER , a ,GL_TEXTURE_2D , texture, 0);
    }

    virtual void drawBuffer( unsigned int size , ATTACHMENT* attach )
    {

        //GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT2 };
        //glDrawBuffers(2,  attachments);

    }


    handle_geometry createGeometry(VBO_TYPE type, unsigned int size_in_byte, char* data/*, FVF_MASK format*/) override
    {
        GLenum target = GL_ARRAY_BUFFER;
        if (type == INDEX) target = GL_ELEMENT_ARRAY_BUFFER;

        handle_geometry vbo = 0;
        glGenBuffers(1, &vbo);
        glBindBuffer(target, vbo);
        glBufferData(target, size_in_byte, data, GL_STATIC_DRAW);

        checkGLError(__LINE__);
        return vbo;
    }

    void distoryGeometry(handle_geometry geo) override
    {
        glDeleteBuffers(1, &geo);
        checkGLError(__LINE__);
    }

    void bindGeometry(handle_geometry geo, VBO_TYPE type, unsigned int size, FVF_MASK format) override
    {
        checkGLError(__LINE__);

        GLenum target = GL_ARRAY_BUFFER;
        if (type == INDEX) target = GL_ELEMENT_ARRAY_BUFFER;

        glBindBuffer(target, geo);

        unsigned int offset = 0;
        unsigned int stride = format.sizeInByte();

        //默认先分配几个固定的属性通道
        //   顶点 0
        //   法线 1
        //   色彩 2
        //   UV0  3
        //   UV1  4

        //顶点 0
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, format._vertex_count, GL_FLOAT, GL_FALSE, stride, (void*)0);
        offset += format._vertex_count * 4;

        //法线 1
        if (format._has_normal)
        {
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
            offset += 3 * 4;
        }

        //色彩 2
        if (format._has_color)
        {
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride, (void*)offset);
            offset += 4 * 4;
        }

        //UV0 3
        if (format._uv_count > 0)
        {
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
            offset += 2 * 4;
        }

        //UV1 4
        if (format._uv_count > 1)
        {
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        }

        checkGLError(__LINE__);
    }

    void unbindGeometry(handle_geometry geo, VBO_TYPE type , FVF_MASK format) override
    {
        //LOGW("drawGeometry=3");
        //glDisableClientState(GL_VERTEX_ARRAY);
        //if (format._has_normal)glDisableClientState(GL_NORMAL_ARRAY);
        //if (format._has_color)glDisableClientState(GL_COLOR_ARRAY);
        //if (format._uv_count > 0)glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        GLenum target = GL_ARRAY_BUFFER;
        if (type == INDEX) target = GL_ELEMENT_ARRAY_BUFFER;

        glBindBuffer(target, 0);

        glDisableVertexAttribArray(0);
        if (format._has_normal)glDisableVertexAttribArray(1);
        if (format._has_color)glDisableVertexAttribArray(2);
        if (format._uv_count > 0)glDisableVertexAttribArray(3);
        if (format._uv_count > 1)glDisableVertexAttribArray(4);
    }

    void bindGeometry(handle_geometry geo, VBO_TYPE type,VBO_DATA_TYPE data_type) override
    {
        checkGLError(__LINE__);

        GLenum target = GL_ARRAY_BUFFER;
        if (type == INDEX) target = GL_ELEMENT_ARRAY_BUFFER;

        glBindBuffer(target, geo);

        switch (data_type)
        {
        case nbPlugins::Renderer::VERTEX:
            //顶点
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
            break;
        case nbPlugins::Renderer::NORMAL:
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
            break;
        case nbPlugins::Renderer::COLOR:
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
            break;
        case nbPlugins::Renderer::TEXTURE_COORD:
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
            break;
        default:
            break;
        }

        checkGLError(__LINE__);
    }

    void bindGeometry(handle_geometry geo, unsigned int id, unsigned int count, DataType type, bool is_index ) override
    {
        //checkGLError(__LINE__);

        GLenum target = GL_ARRAY_BUFFER;
        if (is_index) 
        {
            target = GL_ELEMENT_ARRAY_BUFFER;
        }

        GLenum nt = GL_BYTE;
        switch (type)
        {
        case nbPlugins::Renderer::BYTE:
            nt = GL_BYTE;
            break;
        case nbPlugins::Renderer::UNSIGNED_BYTE:
            nt = GL_UNSIGNED_BYTE;
            break;
        case nbPlugins::Renderer::SHORT:
            nt = GL_SHORT;
            break;
        case nbPlugins::Renderer::UNSIGNED_SHORT:
            nt = GL_UNSIGNED_SHORT;
            break;
        case nbPlugins::Renderer::INT:
            nt = GL_INT;
            break;
        case nbPlugins::Renderer::UNSIGNED_INT:
            nt = GL_UNSIGNED_INT;
            break;
        case nbPlugins::Renderer::FLOAT:
            nt = GL_FLOAT;
            break;
        case nbPlugins::Renderer::FIXED:
            nt = GL_FIXED;
            break;
        default:
            break;
        }

        glBindBuffer(target, geo);

        if(!is_index)
        {
            glEnableVertexAttribArray(id);
            glVertexAttribPointer(id, count, nt, GL_FALSE, 0, (void*)0);
        }

        checkGLError(__LINE__);
    }

    void unbindGeometry(handle_geometry geo, VBO_TYPE type) override
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

    void unbindGeometry(handle_geometry geo ,unsigned int id, bool is_index) override
    {
        if(is_index)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
        {
            glDisableVertexAttribArray(id);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    void drawGeometryArray( PRIMITIVE primitive, int begin, int size ) override
    {
        GLenum mode = GL_TRIANGLES;
        if (primitive == POINTS)				{ mode = GL_POINTS; }
        else if (primitive == LINES)			{ mode = GL_LINES; }
        else if (primitive == LINE_LOOP)	    { mode = GL_LINE_LOOP; }
        else if (primitive == LINESTRIP)		{ mode = GL_LINE_STRIP; }
        else if (primitive == TRIANGLES)		{ mode = GL_TRIANGLES; }
        else if (primitive == TRIANGLESTRIP)	{ mode = GL_TRIANGLE_STRIP; }
        else if (primitive == TRIANGLEFAN)		{ mode = GL_TRIANGLE_FAN; }
        else{}

        glDrawArrays(mode, begin, size); // Draw All Of The Triangles At Once 

        checkGLError(__LINE__);
    }

    void drawGeometryElements( PRIMITIVE primitive,/* int begin,*/ int size ,DataType number_type) override
    {

        GLenum mode = GL_TRIANGLES;
        if (primitive == POINTS)				{ mode = GL_POINTS; }
        else if (primitive == LINES)			{ mode = GL_LINES; }
        else if (primitive == LINE_LOOP)	    { mode = GL_LINE_LOOP; }
        else if (primitive == LINESTRIP)		{ mode = GL_LINE_STRIP; }
        else if (primitive == TRIANGLES)		{ mode = GL_TRIANGLES; }
        else if (primitive == TRIANGLESTRIP)	{ mode = GL_TRIANGLE_STRIP; }
        else if (primitive == TRIANGLEFAN)		{ mode = GL_TRIANGLE_FAN; }
        else{}

        GLenum nt = GL_BYTE;
        switch (number_type)
        {
        case nbPlugins::Renderer::BYTE:
            nt = GL_BYTE;
            break;
        case nbPlugins::Renderer::UNSIGNED_BYTE:
            nt = GL_UNSIGNED_BYTE;
            break;
        case nbPlugins::Renderer::SHORT:
            nt = GL_SHORT;
            break;
        case nbPlugins::Renderer::UNSIGNED_SHORT:
            nt = GL_UNSIGNED_SHORT;
            break;
        case nbPlugins::Renderer::INT:
            nt = GL_INT;
            break;
        case nbPlugins::Renderer::UNSIGNED_INT:
            nt = GL_UNSIGNED_INT;
            break;
        case nbPlugins::Renderer::FLOAT:
            nt = GL_FLOAT;
            break;
        case nbPlugins::Renderer::FIXED:
            nt = GL_FIXED;
            break;
        default:
            break;
        }


        glDrawElements(mode, size, nt, 0);
    }

    char* mapGeometryData(handle_geometry geo) override
    {
        return 0;
    };

    void unmapGeometryData(handle_geometry geo) override
    {
    };


    handle_program createProgram() override
    {
        handle_program prog;
        prog.id = glCreateProgram();
        return prog;

        checkGLError(__LINE__);
    }

    void distoryProgram(handle_program& program) override
    {
        if (program.id > 0)
        {
            glDeleteProgram(program.id);
            program.id = 0;
            program.clear();
        }

        checkGLError(__LINE__);
    }

    void bindProgram(handle_program& shader) override
    {
        if (shader.dirty)
        {
            glLinkProgram(shader.id);
            if(!checkGLError(__LINE__))
            {
                printProgramInfoLog(shader.id);
            }
            shader.dirty = false;
        }
        glUseProgram(shader.id);

        if(!checkGLError(__LINE__))
        {
            printProgramInfoLog(shader.id);
        }
    }

    virtual void unbindProgram()
    {
        glUseProgram(0);
    }

    handle_shader createShader(handle_program& program, SHADER_TYPE type, const char* data) override
    {
        GLuint t = GL_VERTEX_SHADER;
        if (type == SHADER_TYPE::ST_FS) t = GL_FRAGMENT_SHADER;

        GLuint s = glCreateShader(t);

        glShaderSource(s, 1, &data, NULL);
        glCompileShader(s);

        //glAttachShader(program.id, s);
        //program.shader_id[type] = s;
        //program.dirty = true;

        if( !checkGLError(__LINE__) )
        {
            printShaderInfoLog(s);
        }
        return s;
    }

    void distoryShader( handle_shader& shader )
    {
        if (shader > 0)
        {
            glDeleteShader(shader);
            shader = 0;
        }

        checkGLError(__LINE__);
    }

    void attachShader( handle_program& program , SHADER_TYPE type , handle_shader shader )
    {
        if(program.shader_id[type] == 0)
        {
            glAttachShader(program.id, shader);
            //printShaderInfoLog(shader);
            //printProgramInfoLog(program.id);
            program.shader_id[type] = shader;
            program.dirty = true;
        }

        checkGLError(__LINE__);
    }

    //设置属性通道
    void setAttribLocation(handle_program program, std::string name, int i0) override
    {
        //checkGLError(__LINE__);
        //GLint a = glGetAttribLocation(shader.id, name.c_str());
        glBindAttribLocation(program.id, i0, name.c_str());

        checkGLError(__LINE__);
    }

    //设置float类型uniform
    void setUniform(handle_program program, std::string name, float f0) override
    {
        glUniform1f(glGetUniformLocation(program.id, name.c_str()), f0);
        checkGLError(__LINE__);
    }
    void setUniform(handle_program program, std::string name, float f0, float f1) override
    {
        glUniform2f(glGetUniformLocation(program.id, name.c_str()), f0, f1);
        checkGLError(__LINE__);
    }
    void setUniform(handle_program program, std::string name, float f0, float f1, float f2) override
    {
        glUniform3f(glGetUniformLocation(program.id, name.c_str()), f0, f1, f2);
        checkGLError(__LINE__);
    }
    void setUniform(handle_program program, std::string name, float f0, float f1, float f2, float f3) override
    {
        glUniform4f(glGetUniformLocation(program.id, name.c_str()), f0, f1, f2, f3);
        checkGLError(__LINE__);
    }


    //设置int类型uniform
    void setUniform(handle_program program, std::string name, int i0) override
    {
        glUniform1i(glGetUniformLocation(program.id, name.c_str()), i0);
        checkGLError(__LINE__);
    }
    void setUniform(handle_program program, std::string name, int i0, int i1) override
    {
        glUniform2i(glGetUniformLocation(program.id, name.c_str()), i0, i1);
        checkGLError(__LINE__);
    }
    void setUniform(handle_program program, std::string name, int i0, int i1, int i2) override
    {
        glUniform3i(glGetUniformLocation(program.id, name.c_str()), i0, i1, i2);
        checkGLError(__LINE__);
    }
    void setUniform(handle_program program, std::string name, int i0, int i1, int i2, int i3) override
    {
        glUniform4i(glGetUniformLocation(program.id, name.c_str()), i0, i1, i2, i3);
        checkGLError(__LINE__);
    }

    //设置数组类型uniform
    void setUniform(handle_program program, std::string name, unsigned int vec, unsigned int count, float* fv) override
    {
        if(program.id <= 0) return;
        GLint loc = glGetUniformLocation(program.id, name.c_str());
        if(loc >= 0)
        {
            if(vec == 1)
            {
                glUniform1fv(loc, count, fv);
            }
            else if(vec == 2)
            {
                glUniform2fv(loc, count, fv);
            }
            else if(vec == 3)
            {
                glUniform3fv(loc, count, fv);
            }
            else if(vec == 4)
            {
                glUniform4fv(loc, count, fv);
            }
        }
        checkGLError(__LINE__);
    }
    void setUniform(handle_program program, std::string name, unsigned int vec, unsigned int count, int* iv) override
    {
        if(program.id <= 0) return;
        GLint loc = glGetUniformLocation(program.id, name.c_str());
        if(loc >= 0)
        {
            if(vec == 1)
            {
                glUniform1iv(loc, count, iv);
            }
            else if(vec == 2)
            {
                glUniform2iv(loc, count, iv);
            }
            else if(vec == 3)
            {
                glUniform3iv(loc, count, iv);
            }
            else if(vec == 4)
            {
                glUniform4iv(loc, count, iv);
            }
        }
        checkGLError(__LINE__);
    }

    //设置matrix类型uniform
    void setUniformMatrix(handle_program program, std::string name, unsigned int count, const float* fv) override
    {
        if(program.id <= 0) return;
        //checkGLError(__LINE__);
        //void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); 
        //void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); 

        GLint loc = glGetUniformLocation(program.id, name.c_str());
        glUniformMatrix4fv(loc, count, GL_FALSE, fv);

        checkGLError(__LINE__);
    }

    bool setVSync( bool b ) override
    {
        return false;
    }


protected:
private:
};


//注册
NIUBI_REGISTER_PLUGINS_FUNCTION_BEGIN(pm, Renderer_gles_2_x)
NIUBI_REGISTER_PLUGINS(pm, Renderer_gles_2_x)
NIUBI_REGISTER_PLUGINS_FUNCTION_END

