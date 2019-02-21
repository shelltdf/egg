#ifndef _NSG_DATA_DEFINE_H_
#define _NSG_DATA_DEFINE_H_

#include <eggRuntime/DataType.h>

namespace egg
{

    //enum PixelFormat
    //{
    //    NB_COLOR_INDEX,
    //    NB_RED,
    //    NB_GREEN,
    //    NB_BLUE,
    //    NB_ALPHA,
    //    NB_RGB,
    //    NB_BGR,
    //    NB_RGBA,
    //    NB_BGRA,
    //    NB_LUMINANCE,
    //    NB_LUMINANCE_ALPHA
    //};

    //enum DataType
    //{
    //    NB_UNSIGNED_BYTE,
    //    NB_BYTE,
    //    NB_BITMAP,
    //    NB_UNSIGNED_SHORT,
    //    NB_SHORT,
    //    NB_UNSIGNED_INT,
    //    NB_INT,
    //    NB_FLOAT,
    //    NB_UNSIGNED_BYTE_3_3_2,
    //    NB_UNSIGNED_BYTE_2_3_3_REV,
    //    NB_UNSIGNED_SHORT_5_6_5,
    //    NB_UNSIGNED_SHORT_5_6_5_REV,
    //    NB_UNSIGNED_SHORT_4_4_4_4,
    //    NB_UNSIGNED_SHORT_4_4_4_4_REV,
    //    NB_UNSIGNED_SHORT_5_5_5_1,
    //    NB_UNSIGNED_SHORT_1_5_5_5_REV,
    //    NB_UNSIGNED_INT_8_8_8_8,
    //    NB_UNSIGNED_INT_8_8_8_8_REV,
    //    NB_UNSIGNED_INT_10_10_10_2,
    //    NB_UNSIGNED_INT_2_10_10_10_REV
    //};


    enum DataType
    {
        DT_BYTE
        , DT_UNSIGNED_BYTE
        , DT_SHORT
        , DT_UNSIGNED_SHORT
        , DT_INT
        , DT_UNSIGNED_INT
        , DT_FLOAT
        , DT_FIXED
    };

    enum PixelFormat
    {
        PF_DEPTH_COMPONENT
        , PF_ALPHA
        , PF_RGB
        , PF_RGBA
        , PF_LUMINANCE
        , PF_LUMINANCE_ALPHA
    };

    enum InternalFormat
    {
        IF_ALPHA
        , IF_LUMINANCE
        , IF_LUMINANCE_ALPHA
        , IF_RGB
        , IF_RGBA
    };


    enum SHADER_TYPE
    {
        //ST_NULL = 0
        ST_VS
        , ST_FS
        , ST_COUNT
    };

    //enum PRIMITIVE
    //{
    //    POINTS
    //    , LINES
    //    , LINESTRIP
    //    , TRIANGLES
    //    , TRIANGLESTRIP
    //    , TRIANGLEFAN
    //};

    enum VBO_TYPE
    {
        RAW         //平直数列
        , INDEX     //索引数列
    };

    //内存对象会建立一个context的映射表
    //   使用这个id来作为存储标签
    typedef unsigned int handle_geometry; //VBO id
    typedef unsigned int handle_texture;  //texture id
    typedef unsigned int handle_shader;   //shader id
    //typedef unsigned int handle_program;  //program id
    typedef unsigned int handle_framebuffer; //FBO

    struct handle_program
    {
        handle_program(int i = 0) {}

        unsigned int program_id = 0;
        handle_shader shader_id[ST_COUNT] = {0};
        bool dirty = true;
    };


    enum TEXTURE_ENV
    {
        ENV_ADD
        , ENV_MODULATE
        , ENV_DECAL
        , ENV_BLEND
        , ENV_REPLACE
        , ENV_COMBINE
    };

    enum ATTACHMENT
    {
        COLOR_ATTACHMENT0
        , DEPTH_ATTACHMENT
        , STENCIL_ATTACHMENT
    };



    //typedef unsigned int RendererResourceHandle;

    template<class RendererResourceHandle>
    struct RendererResourceSet
    {
        //RendererResourceHandle context = 0;
        //RendererResourceHandle handle = 0;

        std::map< HANDLE_TYPE, RendererResourceHandle > data;

        RendererResourceHandle& getID(HANDLE_TYPE context)
        {
            auto it = data.find(context);
            if(it == data.end())
            {
                //return it->second;
            //}
            //else
            //{
                data[context] = RendererResourceHandle(0);
            }
            return data[context];
        }
        void setID(HANDLE_TYPE context, RendererResourceHandle id)
        {
            data[context] = id;
        }
    };


}//namespace egg

#endif //_NSG_DATA_DEFINE_H_
