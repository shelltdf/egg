#ifndef _EGG_RENDERER_H_
#define _EGG_RENDERER_H_

#include <eggEngine/Export.h>

#include <vector>
#include <string>
#include <map>
#include <stack>

//#include <eggEngine/Material.h>
#include <eggEngine/Geometry.h>
//#include <eggEngine/Texture.h>
#include <eggRuntime/Singleton.h>
#include <eggRuntime/Factory.h>

#include <eggEngine/RenderState.h>
#include <eggEngine/DataDefine.h>


namespace egg
{

    class EGG_ENGINE_EXPORT Renderer
        :public Object
    {
    public:

        Renderer() :mGraphicsContext(0) {}
        virtual ~Renderer() {}

        //DECLARE_FACTORY(Renderer);

        //init
        virtual void init() = 0;

        //主要是兼容dx9
        void setGraphicsContext(GraphicsContext* gc) { mGraphicsContext = gc; }
        GraphicsContext* mGraphicsContext; //gc weak pointer

        // render state
        //   返回唯一的渲染状态对象
        virtual RenderState* getRenderState() = 0;

        // program
        //   每个名字代表一个类型
        //   根据名字从默认的asset路径读取定义文件
        //   命名规则是 |shaders|glcore|name 其中glcore是renderer的类型
        //virtual Program* getProgram(const char* name) = 0;


        // write/read buffer
        virtual void setWriteBuffer() = 0;
        virtual void setReadBuffer() = 0;

        // geometry
        virtual void bindGeometry(egg::GraphicsContext* gc, Geometry* geometry) = 0;
        //virtual void cleanGeometry(egg::Geometry* geometry) = 0;
        virtual void unbindGeometry(egg::GraphicsContext* gc, Geometry* geometry) = 0;
        virtual void drawGeometry(egg::GraphicsContext* gc, Geometry* geometry, int i = -1) = 0;


        // Texture
        virtual handle_texture createTexture() = 0;
        virtual void distoryTexture(handle_texture texture) = 0;
        virtual void bindTexture(handle_texture texture, unsigned int cannel
            , TEXTURE_ENV env = ENV_MODULATE) = 0;
        virtual void setTextureData(handle_texture texture, int w, int h, char* data
            , PixelFormat pixel_format = PF_RGBA
            , DataType number_data_type = DT_UNSIGNED_BYTE) = 0;
        //virtual void setTextureEnv(handle_texture texture,TEXTURE_ENV env){}

        // FBO
        virtual handle_framebuffer createFrameBuffer() = 0;
        virtual void distoryFrameBuffer(handle_framebuffer framebuffer) = 0;
        virtual void bindFrameBuffer(handle_framebuffer framebuffer) = 0;
        virtual void attactFrameBuffer(handle_framebuffer framebuffer
            , handle_texture texture, ATTACHMENT attach) = 0;
        virtual void drawBuffer(unsigned int size, ATTACHMENT* attach) = 0;

        //VBO
        virtual egg::handle_geometry createVBO(egg::VBO_TYPE type, unsigned int size_in_byte, char* data) = 0;
        virtual void distoryVBO(egg::handle_geometry geo) = 0;
        virtual void bindVBO(egg::handle_geometry geo, unsigned int id, unsigned int count, egg::DataType type
            , bool is_index, int stride = 0, int offset = 0, bool normalize = false) = 0;
        virtual void unbindVBO(egg::handle_geometry geo, unsigned int id, bool is_index) = 0;
        virtual void drawArray(egg::PrimitiveSet::Mode primitive, int begin, int size) = 0;
        virtual void drawElements(egg::PrimitiveSet::Mode primitive,/* int begin,*/ int size, egg::DataType index_data_type, char* data = 0) = 0;


        ///
        /// program操作
        ///

        virtual handle_program createProgram() = 0;
        virtual void distoryProgram(handle_program& program) = 0;
        virtual void bindProgram(handle_program& program) = 0;
        virtual void unbindProgram() = 0;
        virtual void setAttribLocation(handle_program program, std::string name, int i0) = 0;


        ///
        /// shader操作
        ///
        virtual handle_shader createShader(handle_program& program, SHADER_TYPE type, const char* data) = 0;
        virtual void distoryShader(handle_shader& shader) = 0;
        virtual void attachShader(handle_program& program, SHADER_TYPE type, handle_shader shader) = 0;

        
        ///
        /// uniform
        ///

        //设置float类型uniform
        virtual void setUniform(handle_program program, std::string name, float f0) = 0;
        virtual void setUniform(handle_program program, std::string name, float f0, float f1) = 0;
        virtual void setUniform(handle_program program, std::string name, float f0, float f1, float f2) = 0;
        virtual void setUniform(handle_program program, std::string name, float f0, float f1, float f2, float f3) = 0;

        //设置int类型uniform
        virtual void setUniform(handle_program program, std::string name, int i0) = 0;
        virtual void setUniform(handle_program program, std::string name, int i0, int i1) = 0;
        virtual void setUniform(handle_program program, std::string name, int i0, int i1, int i2) = 0;
        virtual void setUniform(handle_program program, std::string name, int i0, int i1, int i2, int i3) = 0;

        //设置数组类型uniform
        virtual void setUniform(handle_program program, std::string name, unsigned int vec, unsigned int count, float* fv) = 0;
        virtual void setUniform(handle_program program, std::string name, unsigned int vec, unsigned int count, int* iv) = 0;

        //设置matrix类型uniform
        virtual void setUniformMatrix(handle_program program, std::string name, unsigned int count, const float* fv) = 0;


        //设置视口  左下角为原点
        virtual void setViewport(int x, int y, int width, int height) = 0;

        //启动场景绘制  给dx准备的
        virtual void beginRender() = 0;
        virtual void endRender() = 0;

        enum CLEAR_MASK
        {
            CM_COLOR = 0x1
            , CM_ZBUFFER = 0x2
            , CM_STENCIL = 0x4
        };

        virtual void clear(int mask, float r, float g, float b, float a, float z, float stencil) = 0;


        //
        //固定流水线状态
        //

        virtual void setScissor(int x, int y, int width, int height) = 0;
        virtual void setScissor() = 0;

        enum BLEND_FACTOR
        {
            BF_ZERO
            , BF_ONE
            , BF_SRC_COLOR
            , BF_ONE_MINUS_SRC_COLOR
            , BF_DST_COLOR
            , BF_ONE_MINUS_DST_COLOR
            , BF_SRC_ALPHA
            , BF_ONE_MINUS_SRC_ALPHA
            , BF_DST_ALPHA
            , BF_ONE_MINUS_DST_ALPHA
            , BF_CONSTANT_COLOR
            , BF_ONE_MINUS_CONSTANT_COLOR
            , BF_CONSTANT_ALPHA
            , BF_ONE_MINUS_CONSTANT_ALPHA
        };

        virtual void setBlendFunc(BLEND_FACTOR sfactor, BLEND_FACTOR dfactor) = 0;
        virtual void setBlendFunc() = 0;

        enum DEPTH_FUNC
        {
            DF_NEVER
            , DF_LESS
            , DF_EQUAL
            , DF_LEQUAL
            , DF_GREATER
            , DF_NOTEQUAL
            , DF_GEQUAL
            , DF_ALWAYS
        };

        virtual void setDepthFunc(DEPTH_FUNC func) = 0;
        virtual void setDepthFunc() = 0;

    };

    class EGG_ENGINE_EXPORT RendererFactory
        :public Factory<Renderer>
    {
    public:
        RendererFactory() {}
        ~RendererFactory() {}
        DECLARE_SINGLETON(RendererFactory)
    };

}//namespace egg


#endif // _EGG_RENDERER_H_
