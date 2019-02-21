
#include <eggRuntime/Platform.h>
#include <eggRuntime/Module.h>
#include <eggRuntime/Runtime.h>
#include <eggEngine/Window.h>
#include <eggEngine/GraphicsContext.h>
#include <eggEngine/Renderer.h>
#include <eggEngine/Program.h>
#include <eggRuntime/Log.h>
#include <eggEngine/Pass.h>

/*
OpenGL Core 3.3
*/

#if _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#if _WIN32
#include <gl/wglew.h>
#endif


bool checkGLError(int l)
{
    GLenum error = glGetError();
    if (error == GL_NO_ERROR)
    {
        return true;
    }
    if (error == GL_INVALID_ENUM)
    {
        LOGW("egg error GL_INVALID_ENUM = 0x%x line %d", error, l);
    }
    if (error == GL_INVALID_VALUE)
    {
        LOGW("egg error GL_INVALID_VALUE = 0x%x line %d", error, l);
    }
    if (error == GL_INVALID_OPERATION)
    {
        LOGW("egg error GL_INVALID_OPERATION = 0x%x line %d", error, l);
    }
    if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
    {
        LOGW("egg error GL_INVALID_FRAMEBUFFER_OPERATION = 0x%x line %d", error, l);
    }
    if (error == GL_OUT_OF_MEMORY)
    {
        LOGW("egg error GL_OUT_OF_MEMORY = 0x%x line %d", error, l);
    }

    return false;
}

//输出着色器LOG
void printShaderInfoLog(GLuint shader)
{
    GLint bufSize = 0;
    char *infoLog = 0;
    GLsizei length = 0;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
    if (bufSize > 0)
    {
        infoLog = new char[bufSize];
        if (infoLog != NULL)
        {
            glGetShaderInfoLog(shader, bufSize, &length, infoLog);
            LOGE("%s", infoLog);
            delete[] infoLog;
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
    char *infoLog = 0;
    GLsizei length = 0;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

    if (bufSize > 0)
    {
        infoLog = new char[bufSize];
        if (infoLog != NULL)
        {
            glGetProgramInfoLog(program, bufSize, &length, infoLog);
            LOGE("%s", infoLog);
            delete[] infoLog;
        }
        else
        {
            LOGE("Could not allocate InfoLog buffer.");
        }
    }
}


//RenderState
class RenderState_glcore
    :public egg::RenderState
{
public:

    RenderState_glcore() {}
    virtual ~RenderState_glcore() {}

    virtual void applyMatrix(egg::GraphicsContext* gc, egg::Renderer* renderer) override
    {
        egg::Pass* pass = this->top();
        if (!pass) return;

        egg::Program* program = pass->getProgram();
        if (!program) return;

        //bind program
        program->bind(gc->m_gcID, renderer);
        egg::handle_program handle = program->getProgram().getID(gc->m_gcID);

        //set MVP matrix
        renderer->setUniformMatrix(handle, egg_ModelViewMatrix, 1, &modelViewMatrix[0]);
        renderer->setUniformMatrix(handle, egg_ProjectionMatrix, 1, &projectionMatrix[0]);

        //unbind
        //program->unBind();
    }

    virtual void bindRenderPass(egg::GraphicsContext* gc, egg::Renderer* renderer)override
    {
        egg::Pass* pass = this->top();
        if (!pass) return;

        egg::Program* program = pass->getProgram();
        if (!program) return;

        //bind program
        program->bind(gc->m_gcID, renderer);
        egg::handle_program handle = program->getProgram().getID(gc->m_gcID);


        //set uniform
        auto mUniforms = pass->getUniforms();
        for (auto it_uni = mUniforms.begin(); it_uni != mUniforms.end(); it_uni++)
        {
            egg::UniformBase* ub = it_uni->second.get();
            unsigned int noc = 0;

            if (it_uni->second->getType() == egg::UniformBase::UDT_INT
                || it_uni->second->getType() == egg::UniformBase::UDT_SAMPLER_2D
                || it_uni->second->getType() == egg::UniformBase::UDT_SAMPLER_CUBE
                || it_uni->second->getType() == egg::UniformBase::UDT_FLOAT
                )
            {
                noc = 1;
            }

            if (it_uni->second->getType() == egg::UniformBase::UDT_INT_VEC2
                || it_uni->second->getType() == egg::UniformBase::UDT_FLOAT_VEC2
                )
            {
                noc = 2;
            }

            if (it_uni->second->getType() == egg::UniformBase::UDT_INT_VEC3
                || it_uni->second->getType() == egg::UniformBase::UDT_FLOAT_VEC3
                )
            {
                noc = 3;
            }

            //只处理int float matrix
            switch (it_uni->second->getType())
            {
            case egg::UniformBase::UDT_SAMPLER_2D:       //强行记录为int
            case egg::UniformBase::UDT_SAMPLER_CUBE:     //强行记录为int
            case egg::UniformBase::UDT_INT:
            case egg::UniformBase::UDT_INT_VEC2:
            case egg::UniformBase::UDT_INT_VEC3:
            case egg::UniformBase::UDT_INT_VEC4:
            {
                egg::Uniform<int>* u = dynamic_cast<egg::Uniform<int>*>(ub);
                if (u)
                {
                    renderer->setUniform(handle, ub->getName().c_str(), noc, ub->getCount(), u->getDataPtr());
                }
            }
            break;

            case egg::UniformBase::UDT_FLOAT:
            case egg::UniformBase::UDT_FLOAT_VEC2:
            case egg::UniformBase::UDT_FLOAT_VEC3:
            case egg::UniformBase::UDT_FLOAT_VEC4:
            {
                egg::Uniform<float>* u = dynamic_cast<egg::Uniform<float>*>(ub);
                if (u)
                {
                    renderer->setUniform(handle, ub->getName().c_str(), noc, ub->getCount(), u->getDataPtr());
                }
            }
            break;

            case egg::UniformBase::UDT_MAT2:
            case egg::UniformBase::UDT_MAT3:
            case egg::UniformBase::UDT_MAT4:
            {
                egg::Uniform<float>* u = dynamic_cast<egg::Uniform<float>*>(ub);
                if (u)
                {
                    renderer->setUniformMatrix(handle, ub->getName().c_str(), ub->getCount() / 16, u->getDataPtr());
                }
            }
            break;
            default:
                break;
            }//switch

        }//for uniform


         //bind texture
        for (unsigned int i = 0; i < pass->getNumTexture(); i++)
        {
            egg::Texture* tex = pass->getTexture(i);
            if (!tex) continue;

            //输入不存在就建立对应的GL对象
            if (tex->mGLHandle.getID(gc->m_gcID) == 0)
            {
                egg::Image* image = tex->getImage();
                if (image == 0) continue;

                //查询外部路径作为内存和外存的区别
                unsigned char* image_data_prt = image->getData();
                if (image_data_prt)
                {
                    //内存中存在image的数据
                    tex->mGLHandle.setID(gc->m_gcID, renderer->createTexture());

                    //int channal = 0;
                    //if(image->getPixelFormat() == Image::NB_LUMINANCE)
                    //{
                    //    channal = 1;
                    //}
                    //else if(image->getPixelFormat() == Image::NB_RGB)
                    //{
                    //    channal = 3;
                    //}
                    //else if(image->getPixelFormat() == Image::NB_RGBA)
                    //{
                    //    channal = 4;
                    //}

                    renderer->setTextureData(tex->mGLHandle.getID(gc->m_gcID)
                        , image->getS(), image->getT()
                        , (char*)image_data_prt
                        , image->getPixelFormat()
                        , image->getDataType()
                    );

                }
            }//if(tex->mGLHandle == 0)


            //如果句柄存在就bind
            if (tex->mGLHandle.getID(gc->m_gcID) > 0)
            {
                //render->enable(nbPlugins::Renderer::TEXTURE_2D);//默认开启
                renderer->bindTexture(tex->mGLHandle.getID(gc->m_gcID), i, egg::TEXTURE_ENV::ENV_MODULATE);
            }


        }//for textrue
    }
    virtual void unbindRenderPass()override
    {
        egg::Pass* pass = this->top();
        if (!pass) return;

        egg::Program* program = pass->getProgram();
        if (!program) return;

        program->unBind();
    }

private:

#if 0 //暂时保留
    egg::handle_geometry createGeometry(unsigned int size_in_byte, char* data, int format)override
    {
        egg::handle_geometry vbo = 0;
        glGenBuffersARB(1, &vbo);
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo);
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, size_in_byte, data, GL_STATIC_DRAW_ARB);
        return vbo;
    }

    void distoryGeometry(egg::handle_geometry geo)override
    {
        glDeleteBuffersARB(1, &geo);
    }

    //填充数据
    //  void setGeometryData(handle_geometry geo ,unsigned int szie ,char* data){}

    //绑定数据
    void bindGeometry(egg::handle_geometry geo, unsigned int size, int format)override
    {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, geo);
    }

    //绘制图原
    void drawGeometry(egg::handle_geometry geo, egg::PRIMITIVE primitive, int begin, int size, int format)override
    {
        unsigned int offset = 0;
        unsigned int stride = format.sizeInByte();

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(format._vertex_count, GL_FLOAT, stride, (char *)NULL); // Set The Vertex Pointer To The Vertex Buffer
        offset += format._vertex_count * 4;

        if (format._has_normal)
        {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, stride, (char *)offset);
            offset += 3 * 4;
        }

        if (format._has_color)
        {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_FLOAT, stride, (char *)offset);
            offset += 4 * 4;
        }

        //if(format._uv_count>0)
        //{
        //	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        //	glTexCoordPointer( 2, GL_FLOAT, format.sizeInByte(), (char *) offset ); // Set The TexCoord Pointer To The TexCoord Buffer 
        //}

        for (int i = 0; i < format._uv_count; i++)
        {
            glClientActiveTextureARB(GL_TEXTURE0_ARB + i);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, format.sizeInByte(), (char *)offset); // Set The TexCoord Pointer To The TexCoord Buffer 
        }

        GLenum mode = GL_TRIANGLES;
        if (primitive == POINTS) { mode = GL_POINTS; }
        else if (primitive == LINES) { mode = GL_LINES; }
        else if (primitive == LINESTRIP) { mode = GL_LINE_STRIP; }
        else if (primitive == TRIANGLES) { mode = GL_TRIANGLES; }
        else if (primitive == TRIANGLESTRIP) { mode = GL_TRIANGLE_STRIP; }
        else if (primitive == TRIANGLEFAN) { mode = GL_TRIANGLE_FAN; }
        else {}

        glDrawArrays(mode, begin, size); // Draw All Of The Triangles At Once 
        //glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);

        glDisableClientState(GL_VERTEX_ARRAY);
        if (format._has_normal)glDisableClientState(GL_NORMAL_ARRAY);
        if (format._has_color)glDisableClientState(GL_COLOR_ARRAY);
        if (format._uv_count > 0)glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
#endif



};


class Renderer_glcore
    :public egg::Renderer
{
public:

    Renderer_glcore() {}
    ~Renderer_glcore() {}

    void init() override
    {
        glewInit();
    }

    void setViewport(int x, int y, int width, int height) override
    {
        glViewport(x, y, width, height);
    }

    void beginRender() override
    {
        //do nothing
    }
    void endRender() override
    {
        //do nothing
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
            glClearDepth(z);
        }
        if ( (mask & CM_STENCIL) == CM_STENCIL)
        {
        	flags = flags| GL_STENCIL_BUFFER_BIT;
        }
        glClear(flags);
    }

    void setWriteBuffer()override
    {
        //glDrawBuffer
    }
    void setReadBuffer()override
    {
        //glReadBuffer
    }

    egg::RenderState* getRenderState()override
    {
        static RenderState_glcore* rs = new RenderState_glcore();
        return rs;
    }

    //egg::Program* getProgram(const char* name)override
    //{
    //    return 0;
    //}

#if 0
    void setMatrix(MATRIX_MODEL model, float* m)
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
#endif


    GLenum getDataType(egg::DataType data_type)
    {
        GLenum nt = GL_BYTE;
        switch (data_type)
        {
        case egg::DT_BYTE:
            nt = GL_BYTE;
            break;
        case egg::DT_UNSIGNED_BYTE:
            nt = GL_UNSIGNED_BYTE;
            break;
        case egg::DT_SHORT:
            nt = GL_SHORT;
            break;
        case egg::DT_UNSIGNED_SHORT:
            nt = GL_UNSIGNED_SHORT;
            break;
        case egg::DT_INT:
            nt = GL_INT;
            break;
        case egg::DT_UNSIGNED_INT:
            nt = GL_UNSIGNED_INT;
            break;
        case egg::DT_FLOAT:
            nt = GL_FLOAT;
            break;
        case egg::DT_FIXED:
            nt = GL_FIXED;
            break;
        default:
            break;
        }

        return nt;
    }

    GLenum getPrimitive(egg::PrimitiveSet::Mode primitive)
    {
        GLenum mode = GL_TRIANGLES;
        if (primitive == egg::PrimitiveSet::POINTS) { mode = GL_POINTS; }
        else if (primitive == egg::PrimitiveSet::LINES) { mode = GL_LINES; }
        else if (primitive == egg::PrimitiveSet::LINE_LOOP) { mode = GL_LINE_LOOP; }
        //else if (primitive == egg::PrimitiveSet::LINESTRIP) { mode = GL_LINE_STRIP; }
        else if (primitive == egg::PrimitiveSet::TRIANGLES) { mode = GL_TRIANGLES; }
        //else if (primitive == egg::PrimitiveSet::TRIANGLESTRIP) { mode = GL_TRIANGLE_STRIP; }
        //else if (primitive == egg::PrimitiveSet::TRIANGLEFAN) { mode = GL_TRIANGLE_FAN; }
        else {}

        return mode;
    }

    egg::handle_geometry createVBO(egg::VBO_TYPE type, unsigned int size_in_byte, char* data)override
    {
        GLenum target = GL_ARRAY_BUFFER;
        if (type == egg::INDEX) target = GL_ELEMENT_ARRAY_BUFFER;

        egg::handle_geometry vbo = 0;
        glGenBuffers(1, &vbo);
        glBindBuffer(target, vbo);
        //glBufferData(target, size_in_byte, data, GL_STATIC_DRAW);
        glBufferData(target, size_in_byte, data, GL_STREAM_DRAW);

        checkGLError(__LINE__);
        return vbo;
    }

    void distoryVBO(egg::handle_geometry geo)override
    {
        glDeleteBuffers(1, &geo);
        checkGLError(__LINE__);
    }

    void bindVBO(egg::handle_geometry geo, unsigned int id, unsigned int count, egg::DataType type
        , bool is_index, int stride = 0, int offset = 0, bool normalize = false)override
    {
        //checkGLError(__LINE__);

        GLenum target = GL_ARRAY_BUFFER;
        if (is_index) { target = GL_ELEMENT_ARRAY_BUFFER; }

        GLenum nt = getDataType(type);

        glBindBuffer(target, geo);
        checkGLError(__LINE__);

        if (!is_index)
        {
            glEnableVertexAttribArray(id);
            glVertexAttribPointer(id, count, nt, normalize ? GL_TRUE : GL_FALSE, stride, (void*)offset);
        }

        checkGLError(__LINE__);
    }

    void unbindVBO(egg::handle_geometry geo, unsigned int id, bool is_index)override
    {
        if (is_index)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
        {
            glDisableVertexAttribArray(id);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    void drawArray(egg::PrimitiveSet::Mode primitive, int begin, int size)override
    {
        GLenum mode = getPrimitive(primitive);
        glDrawArrays(mode, begin, size); // Draw All Of The Triangles At Once 
        checkGLError(__LINE__);
    }

    void drawElements(egg::PrimitiveSet::Mode primitive,/* int begin,*/ int size, egg::DataType index_data_type,char* data=0)override
    {
        GLenum mode = getPrimitive(primitive);
        GLenum nt = getDataType(index_data_type);
        glDrawElements(mode, size, nt, data);
        checkGLError(__LINE__);
    }


    void bindGeometry(egg::GraphicsContext* gc, egg::Geometry* geometry)override
    {
        //查询当前Geometry对象是否包含正确的VBO句柄
        //如果没有就建立对应的VBO
        //bind全部VBO对象


        //如果vbo对象为空 就建立他们
        for (auto it_va = geometry->mVectexAttribute.begin(); it_va != geometry->mVectexAttribute.end(); it_va++)
        {
            auto it_buf = geometry->mBuffers.find(it_va->second->buffer_name);
            if (it_buf != geometry->mBuffers.end())
            {
                auto it_vbo = geometry->mVBOs.find(it_buf->first);
                if (it_vbo != geometry->mVBOs.end()) continue;

#if 0
                //兼容性 把int类型强制转换成float
                //       主要是因为本地模拟器并不支持 ivec4 类型
                if (it_buf->second->getTypeID() == typeid(int).name())
                {
                    egg::ArrayT<float>* a = new egg::ArrayT<float>(it_buf->second->getSize());
                    int*   b = (int*)it_buf->second->getData();
                    for (unsigned int i = 0; i < a->getSize(); i++)
                    {
                        (*a)[i] = (float)b[i];
                    }
                    geometry->mBuffers[it_buf->first] = a;
                }
#endif
                //如果vbo对象不存在就建立
                //if (it_vbo->second.getID(gc->m_gcID))
                {
                    egg::handle_geometry hg = createVBO(
                        egg::RAW
                        , it_buf->second->getSize() * it_buf->second->getSizeOf()
                        , it_buf->second->getData()
                    );
                    //it_vbo->second.setID(gc->m_gcID, hg);
                    egg::RendererResourceSet<egg::handle_geometry> rrs;
                    rrs.setID(gc->m_gcID, hg);
                    geometry->mVBOs[it_buf->first] = rrs;
                }
            }
        }

        //绑定vbo对象
        for (auto it_vbo = geometry->mVBOs.begin(); it_vbo != geometry->mVBOs.end(); it_vbo++)
        {
            auto it_va = geometry->mVectexAttribute.find(it_vbo->first);
            if (it_va == geometry->mVectexAttribute.end()) continue;

            egg::VectexAttribute* attr = it_va->second.get();
            egg::Pass* pass = this->getRenderState()->top();
            egg::Program* prog = pass->getProgram();
            if (prog)
            {
                auto it_prog = prog->mVANameMapping.find(it_vbo->first);
                if (it_prog != prog->mVANameMapping.end())
                {
                    this->bindVBO(it_vbo->second.getID(gc->m_gcID), it_prog->second, attr->noc, attr->type, false);
                }
            }//if(prog) 
        }

    }

    void unbindGeometry(egg::GraphicsContext* gc, egg::Geometry* geometry)override
    {
        //查询当前Geometry对象是否包含正确的vbo句柄
        //如果存在就删除vbo

        for (auto it_vbo = geometry->mVBOs.begin(); it_vbo != geometry->mVBOs.end(); it_vbo++)
        {
            auto it_va = geometry->mVectexAttribute.find(it_vbo->first);
            if (it_va == geometry->mVectexAttribute.end()) continue;

            egg::VectexAttribute* attr = it_va->second.get();
            egg::Pass* pass = this->getRenderState()->top();
            egg::Program* prog = pass->getProgram();
            if (prog)
            {
                auto it_prog = prog->mVANameMapping.find(it_vbo->first);
                if (it_prog != prog->mVANameMapping.end())
                {
                    this->unbindVBO(it_vbo->second.getID(gc->m_gcID), it_prog->second, false);
                }
            }//if(prog) 
        }
    }

    void drawGeometry(egg::GraphicsContext* gc, egg::Geometry* geometry, int id = -1)override
    {
        //检查ID
        if (id >= geometry->mSubGeometry.size()) return;

        //绘制指定subgeometry中的全部图元集合
        for (size_t set_id = 0; set_id < geometry->mSubGeometry[id]->PrimitiveSets.size(); set_id++)//当前primitive set的id
        {
            egg::PrimitiveSet ps = geometry->mSubGeometry[id]->PrimitiveSets[set_id];

            unsigned int s1 = geometry->mSubGeometry.size();
            unsigned int s2 = geometry->mSubGeometry[id]->PrimitiveSets.size();
            unsigned int b = ps.mBegin;
            unsigned int e = ps.mCount;

            if (geometry->isIndex())
            {
                this->drawElements(ps.mType, (e - b), geometry->getIndexDataType());
            }
            else
            {
                this->drawArray(ps.mType, b, e);
            }

        }//for set_id

    }


    egg::handle_texture createTexture() override
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

    void distoryTexture(egg::handle_texture texture) override
    {
        glDeleteTextures(1, &texture);

        checkGLError(__LINE__);
    }

    void bindTexture(egg::handle_texture texture, unsigned int cannel, egg::TEXTURE_ENV env) override
    {
        glActiveTexture(GL_TEXTURE0 + cannel);
        glBindTexture(GL_TEXTURE_2D, texture);

        //临时
        //glBindSampler(0, 0);

        checkGLError(__LINE__);
    }

    void setTextureData(egg::handle_texture texture, int w, int h, char* data
        //, int channal_num, int channal_bits
        , egg::PixelFormat pixel_format, egg::DataType number_data_type
    ) override
    {

        GLenum format = GL_RGB;
        switch (pixel_format)
        {
        case egg::PF_DEPTH_COMPONENT:
            format = GL_DEPTH_COMPONENT;
            break;
        case egg::PF_ALPHA:
            format = GL_ALPHA;
            break;
        case egg::PF_RGB:
            format = GL_RGB;
            break;
        case egg::PF_RGBA:
            format = GL_RGBA;
            break;
        case egg::PF_LUMINANCE:
            format = GL_LUMINANCE;
            break;
        case egg::PF_LUMINANCE_ALPHA:
            format = GL_LUMINANCE_ALPHA;
            break;
        default:
            break;
        }

        GLenum internal_format = format;
        if (format == GL_DEPTH_COMPONENT)
        {
            internal_format = GL_DEPTH_COMPONENT16;
            //internal_format = GL_DEPTH_COMPONENT24_OES;
        }


        GLenum type = GL_BYTE;
        switch (number_data_type)
        {
        case egg::DT_BYTE:
            type = GL_BYTE;
            break;
        case egg::DT_UNSIGNED_BYTE:
            type = GL_UNSIGNED_BYTE;
            break;
        case egg::DT_SHORT:
            type = GL_SHORT;
            break;
        case egg::DT_UNSIGNED_SHORT:
            type = GL_UNSIGNED_SHORT;
            break;
        case egg::DT_INT:
            type = GL_INT;
            break;
        case egg::DT_UNSIGNED_INT:
            type = GL_UNSIGNED_INT;
            break;
        case egg::DT_FLOAT:
            type = GL_FLOAT;
            break;
        case egg::DT_FIXED:
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


    virtual egg::handle_framebuffer createFrameBuffer() override
    {
        GLuint fbo;
        glGenFramebuffers(1, &fbo);

        //glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, 512);
        //glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, 512);
        //glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_SAMPLES, 4);

        return fbo;
    }

    virtual void distoryFrameBuffer(egg::handle_framebuffer framebuffer) override
    {
        glDeleteFramebuffers(1, &framebuffer);
    }

    virtual void bindFrameBuffer(egg::handle_framebuffer framebuffer) override
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    }

    virtual void attactFrameBuffer(egg::handle_framebuffer framebuffer, egg::handle_texture texture, egg::ATTACHMENT attach) override
    {
        GLenum a = GL_COLOR_ATTACHMENT0;
        if (attach == egg::COLOR_ATTACHMENT0) a = GL_COLOR_ATTACHMENT0;
        if (attach == egg::DEPTH_ATTACHMENT) a = GL_DEPTH_ATTACHMENT;
        if (attach == egg::STENCIL_ATTACHMENT) a = GL_STENCIL_ATTACHMENT;

        glFramebufferTexture2D(GL_FRAMEBUFFER, a, GL_TEXTURE_2D, texture, 0);
    }

    virtual void drawBuffer(unsigned int size, egg::ATTACHMENT* attach) override
    {

        //GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT2 };
        //glDrawBuffers(2,  attachments);

    }



    egg::handle_program createProgram() override
    {
        egg::handle_program prog;
        prog.program_id = glCreateProgram();
        return prog;

        checkGLError(__LINE__);
    }

    void distoryProgram(egg::handle_program& program) override
    {
        if (program.program_id > 0)
        {
            glDeleteProgram(program.program_id);
            program = 0;
        }

        checkGLError(__LINE__);
    }

    void bindProgram(egg::handle_program& program) override
    {
        if (program.dirty)
        {
            glLinkProgram(program.program_id);

            if (!checkGLError(__LINE__))
            {
                printProgramInfoLog(program.program_id);
            }
            program.dirty = false;
        }
        glUseProgram(program.program_id);

        if (!checkGLError(__LINE__))
        {
            printProgramInfoLog(program.program_id);
        }
    }

    virtual void unbindProgram()override
    {
        glUseProgram(0);
    }

    egg::handle_shader createShader(egg::handle_program& program, egg::SHADER_TYPE type, const char* data) override
    {
        GLuint t = GL_VERTEX_SHADER;
        if (type == egg::SHADER_TYPE::ST_FS) t = GL_FRAGMENT_SHADER;

        GLuint s = glCreateShader(t);

        glShaderSource(s, 1, &data, NULL);
        glCompileShader(s);

        if (!checkGLError(__LINE__))
        {
            printShaderInfoLog(s);
        }
        return s;
    }

    void distoryShader(egg::handle_shader& shader)override
    {
        if (shader > 0)
        {
            glDeleteShader(shader);
            shader = 0;
        }

        checkGLError(__LINE__);
    }

    void attachShader(egg::handle_program& program, egg::SHADER_TYPE type, egg::handle_shader shader)override
    {
        if (program.shader_id[type] == 0)
        {
            glAttachShader(program.program_id, shader);
            program.shader_id[type] = shader;
            program.dirty = true;
        }

        checkGLError(__LINE__);
    }

    //设置属性通道
    void setAttribLocation(egg::handle_program program, std::string name, int i0) override
    {
        glBindAttribLocation(program.program_id, i0, name.c_str());
        checkGLError(__LINE__);
    }

    //设置float类型uniform
    void setUniform(egg::handle_program program, std::string name, float f0) override
    {
        glUniform1f(glGetUniformLocation(program.program_id, name.c_str()), f0);
        checkGLError(__LINE__);
    }
    void setUniform(egg::handle_program program, std::string name, float f0, float f1) override
    {
        glUniform2f(glGetUniformLocation(program.program_id, name.c_str()), f0, f1);
        checkGLError(__LINE__);
    }
    void setUniform(egg::handle_program program, std::string name, float f0, float f1, float f2) override
    {
        glUniform3f(glGetUniformLocation(program.program_id, name.c_str()), f0, f1, f2);
        checkGLError(__LINE__);
    }
    void setUniform(egg::handle_program program, std::string name, float f0, float f1, float f2, float f3) override
    {
        glUniform4f(glGetUniformLocation(program.program_id, name.c_str()), f0, f1, f2, f3);
        checkGLError(__LINE__);
    }


    //设置int类型uniform
    void setUniform(egg::handle_program program, std::string name, int i0) override
    {
        GLint loc = glGetUniformLocation(program.program_id, name.c_str());
        glUniform1i(loc, i0);
        checkGLError(__LINE__);
    }
    void setUniform(egg::handle_program program, std::string name, int i0, int i1) override
    {
        glUniform2i(glGetUniformLocation(program.program_id, name.c_str()), i0, i1);
        checkGLError(__LINE__);
    }
    void setUniform(egg::handle_program program, std::string name, int i0, int i1, int i2) override
    {
        glUniform3i(glGetUniformLocation(program.program_id, name.c_str()), i0, i1, i2);
        checkGLError(__LINE__);
    }
    void setUniform(egg::handle_program program, std::string name, int i0, int i1, int i2, int i3) override
    {
        glUniform4i(glGetUniformLocation(program.program_id, name.c_str()), i0, i1, i2, i3);
        checkGLError(__LINE__);
    }

    //设置数组类型uniform
    void setUniform(egg::handle_program program, std::string name, unsigned int vec, unsigned int count, float* fv) override
    {
        if (program.program_id <= 0) return;

        GLint loc = glGetUniformLocation(program.program_id, name.c_str());
        if (loc >= 0)
        {
            if (vec == 1)
            {
                glUniform1fv(loc, count, fv);
            }
            else if (vec == 2)
            {
                glUniform2fv(loc, count, fv);
            }
            else if (vec == 3)
            {
                glUniform3fv(loc, count, fv);
            }
            else if (vec == 4)
            {
                glUniform4fv(loc, count, fv);
            }
        }
        checkGLError(__LINE__);
    }
    void setUniform(egg::handle_program program, std::string name, unsigned int vec, unsigned int count, int* iv) override
    {
        if (program.program_id <= 0) return;

        GLint loc = glGetUniformLocation(program.program_id, name.c_str());
        if (loc >= 0)
        {
            if (vec == 1)
            {
                glUniform1iv(loc, count, iv);
            }
            else if (vec == 2)
            {
                glUniform2iv(loc, count, iv);
            }
            else if (vec == 3)
            {
                glUniform3iv(loc, count, iv);
            }
            else if (vec == 4)
            {
                glUniform4iv(loc, count, iv);
            }
        }
        checkGLError(__LINE__);
    }

    //设置matrix类型uniform
    void setUniformMatrix(egg::handle_program program, std::string name, unsigned int count, const float* fv) override
    {
        if (program.program_id <= 0) return;
        //checkGLError(__LINE__);
        //void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); 
        //void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); 

        GLint loc = glGetUniformLocation(program.program_id, name.c_str());
        glUniformMatrix4fv(loc, count, GL_FALSE, fv);

        checkGLError(__LINE__);
    }


    void setScissor(int x, int y, int width, int height) override
    {
        glEnable(GL_SCISSOR_TEST);
        glScissor(x, y, width, height);
    }
    void setScissor() override
    {
        glDisable(GL_SCISSOR_TEST);
    }

    void setBlendFunc(BLEND_FACTOR sfactor, BLEND_FACTOR dfactor)  override
    {
        glEnable(GL_BLEND);
        //glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(getBlendFactor(sfactor), getBlendFactor(dfactor));
    }
    void setBlendFunc() override
    {
        glDisable(GL_BLEND);
    }

    void setDepthFunc(DEPTH_FUNC func)override
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(getDepthFunc(func));
    }
    void setDepthFunc()override
    {
        glDisable(GL_DEPTH_TEST);
    }

protected:
private:

    GLenum getBlendFactor(BLEND_FACTOR factor)
    {
        GLenum type = GL_ONE;
        switch (factor)
        {
        case egg::Renderer::BF_ZERO: type = GL_ZERO; break;
        case egg::Renderer::BF_ONE: type = GL_ONE; break;
        case egg::Renderer::BF_SRC_COLOR: type = GL_SRC_COLOR; break;
        case egg::Renderer::BF_ONE_MINUS_SRC_COLOR: type = GL_ONE_MINUS_SRC_COLOR; break;
        case egg::Renderer::BF_DST_COLOR: type = GL_DST_COLOR; break;
        case egg::Renderer::BF_ONE_MINUS_DST_COLOR: type = GL_ONE_MINUS_DST_COLOR; break;
        case egg::Renderer::BF_SRC_ALPHA: type = GL_SRC_ALPHA; break;
        case egg::Renderer::BF_ONE_MINUS_SRC_ALPHA: type = GL_ONE_MINUS_SRC_ALPHA; break;
        case egg::Renderer::BF_DST_ALPHA: type = GL_DST_ALPHA; break;
        case egg::Renderer::BF_ONE_MINUS_DST_ALPHA: type = GL_ONE_MINUS_DST_ALPHA; break;
        case egg::Renderer::BF_CONSTANT_COLOR: type = GL_CONSTANT_COLOR; break;
        case egg::Renderer::BF_ONE_MINUS_CONSTANT_COLOR: type = GL_ONE_MINUS_CONSTANT_COLOR; break;
        case egg::Renderer::BF_CONSTANT_ALPHA: type = GL_CONSTANT_ALPHA; break;
        case egg::Renderer::BF_ONE_MINUS_CONSTANT_ALPHA: type = GL_ONE_MINUS_CONSTANT_ALPHA; break;

        default: break;
        }
        return type;
    }

    GLenum getDepthFunc(DEPTH_FUNC func)
    {
        GLenum type = GL_LESS;
        switch (func)
        {
        case egg::Renderer::DF_NEVER: type = GL_NEVER; break;
        case egg::Renderer::DF_LESS: type = GL_LESS; break;
        case egg::Renderer::DF_EQUAL: type = GL_EQUAL; break;
        case egg::Renderer::DF_LEQUAL: type = GL_LEQUAL; break;
        case egg::Renderer::DF_GREATER: type = GL_GREATER; break;
        case egg::Renderer::DF_NOTEQUAL: type = GL_NOTEQUAL; break;
        case egg::Renderer::DF_GEQUAL: type = GL_GEQUAL; break;
        case egg::Renderer::DF_ALWAYS: type = GL_ALWAYS; break;

        default: break;
        }
        return type;
    }

};



INSTALL_MODULE(Renderer_glcore)
{
    //egg::Renderer::registerFactory("Renderer_glcore"
    //    , new egg::Renderer::Factory<Renderer_glcore>());

    egg::RendererFactory::getInstance(true)
        ->registerCrteator("Renderer_glcore"
            , new egg::Crteator<Renderer_glcore>());

    return true;
}

UNINSTALL_MODULE(Renderer_glcore)
{
    //egg::Renderer::unregisterFactory("Renderer_glcore");

    egg::RendererFactory::getInstance(true)
        ->unregisterCrteator("Renderer_glcore");

    return true;
}



