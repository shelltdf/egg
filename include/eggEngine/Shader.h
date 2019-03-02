#ifndef NIUBI_SG_SHADER_H
#define NIUBI_SG_SHADER_H

#include <eggEngine/Export.h>

#include <map>

//#include <eggBase/Public.h>

#include <eggEngine/DataDefine.h>
#include <eggEngine/Resource.h>
//#include <eggEngine/RenderState.h>
#include <egg/DataType.h>
#include <eggEngine/Renderer.h>

namespace egg
{

    //class Renderer;
    //class GraphicsContext;
    class RenderState;
    class Program;

    /*
    * Shader对象
    */
    class EGG_ENGINE_EXPORT Shader
        :public Resource
    {
    public:

        enum ShaderType
        {
            NB_NULL_SHADER
            , NB_VERTEX_SHADER
            , NB_TESSELLATION_CONTROL_SHADER
            , NB_TESSELLATION_EVALUATION_SHADER
            , NB_GEOMETRY_SHADER
            , NB_FRAGMENT_SHADER
            , NB_COMPUTE_SHADER
            , NB_COUNT //最后一个
        };

#if 0
        enum ShaderProfileType
        {
            SPT_BULL
            , SPT_GL_1            //不支持
            , SPT_GL_2            //支持    当作兼容模式
            , SPT_GL_CORE_330     //支持    GL3.3 #version 330
            , SPT_GLES_1          //不支持
            , SPT_GLES_2          //支持 
            , SPT_GLES_310        //支持
            , SPT_GLES_320        //支持
            , SPT_DX_11           //移植性保证
            , SPT_COUNT           //最后一个
        };
#endif

        Shader():mShaderType(NB_NULL_SHADER){}
        virtual ~Shader(){}
        
        //virtual std::string getExt(){return "shader";}
        virtual std::string ObjectClassName() override {return "shader";}

#if 1
        void bind(HANDLE_TYPE contextID, Renderer* renderer, Program* program);
        void unBind();
#endif

        virtual void save( Path filename , std::ostream& os , Serializer* ser ) const override
        {
            //直接写入字符串到文件
            os << mString.c_str();

#if 0
            Resource::save(filename ,os,ser);

            //shader类型
            ser->writeNumber( os , (int)mShaderType );

            //mString
            ser->writeNumber( os , (unsigned int)mString.size() ); //通道数
            for (auto it = mString.begin(); it != mString.end(); it ++)
            {
                ser->writeNumber( os , it->first );  //ShaderProfileType
                ser->writeString( os , it->second ); //shader文本信息
            }
#endif
        }

        virtual void load( Path filename , std::istream& is , Serializer* ser , ObjectManager* om
            , Referenced* rmg, bool load_ext) override
        {
            //从文件读取字符串
            std::ostringstream oss;
            oss << is.rdbuf();
            mString = oss.str();
            //if (mString[mString.size() - 1] != '\0')mString.push_back('\0');

#if 0
            Resource::load(filename ,is,ser,om,rmg,load_ext);

            //shader类型
            ser->readNumber( is , (int&)mShaderType );

            //mString
            unsigned int shader_count = 0;
            ser->readNumber( is , shader_count ); //通道数
            for (unsigned int i = 0; i < shader_count; i++)
            {
                int shader_profile = 0;
                ser->readNumber( is , shader_profile ); //ShaderProfileType

                std::string shader_string;
                ser->readString( is , shader_string );  //shader文本信息

                mString[(ShaderProfileType)shader_profile] = shader_string;
            }
#endif
        }

        ShaderType getShaderType(){return mShaderType;}
        void setShaderType(ShaderType st){mShaderType = st;}

#if 0
        std::string getString(ShaderProfileType proflie)
        {
            auto it = mString.find(proflie);
            if( it != mString.end() )
            {
                return it->second;
            }
            return "";
        }
        void setString(ShaderProfileType proflie,std::string str)
        {
            mString[proflie] = str;
        }
#endif

        std::string getString()
        {
            return mString;
        }
        void setString(std::string str)
        {
            mString = str;
        }

    //private:

        ShaderType mShaderType;

        std::string mString;
        //std::map< ShaderProfileType ,std::string > mString;

        RendererResourceSet<handle_shader> mShader;
    };
    
}//namespace nbSG


#endif//NIUBI_SG_SHADER_H
