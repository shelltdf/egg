#ifndef NIUBI_SG_GEOMETRY_H
#define NIUBI_SG_GEOMETRY_H

#include <eggEngine/Export.h>

#include <map>

//#include <eggBase/Public.h>

#include <eggEngine/DataDefine.h>
#include <eggEngine/Resource.h>
#include <eggEngine/Matrix.h>


/*
* Geometry
*   表达一个资源对象
*   包含多个subGeometry 每个subGeometry包含多个渲染段（也就是图元段）
*   
*   关于属性定义的问题
*       理论上shader只能在link之前才能修改attribute的id 一旦link就无法修改了
*       而geometry的属性装在前就需要确定id
*       所以这个问题的核心是到底谁为主
*           如果以geometry为主 建立shader的时候必须用geometry做参考 这样跟material的关联就很乱
*           如果把规则限定在材质里 会更清晰一些 
*               而且这样可以避免以为geometry的定义不同产生不同的shader
*               更重要的是这样可以让后面的shader可视化设计更简单 不依赖geometry也可以定义
*       按照第二种方式可以简单总结
*           material决定属性的id
*           以material做参考link shader
*           以material做参考加载geometry的vbo
*/

/*
* 基于material主导的方案 geometry建立vbo的时候 就必须存在一个material的关联了
*     其实这不是负担 因为没有shader 什么都无法渲染 
*     系统只是提供了几个默认的material 在没有指定的时候 自动完成这个工作
*     这样的做法对geometry用紧凑还是分离的VBO都没有影响
*/

namespace egg
{

    //图元集合
    struct PrimitiveSet
    {

        enum Mode
        {
            POINTS,
            LINES,
            LINE_STRIP,
            LINE_LOOP,
            TRIANGLES,
            TRIANGLE_STRIP,
            TRIANGLE_FAN,
            QUADS,
            QUAD_STRIP,
            POLYGON,
            LINES_ADJACENCY,
            LINE_STRIP_ADJACENCY,
            TRIANGLES_ADJACENCY,
            TRIANGLE_STRIP_ADJACENCY,
            PATCHES
        };

        PrimitiveSet(Mode type = TRIANGLES , unsigned int begin = 0, unsigned int count = 0)
            :mType(type), mBegin(begin), mCount(count) 
        {
        }

        Mode mType;
        unsigned int mBegin;
        unsigned int mCount;
    };

    //存储多个图元
    class SubGeometry
        :public Object
    {
    public:
        SubGeometry(){}
        SubGeometry( PrimitiveSet::Mode type , unsigned int begin , unsigned int count )
        { 
            addPrimitiveSet( type,begin,count ); 
        }
        virtual ~SubGeometry(){}

        //查询包含多少渲染段
        unsigned int getSegment()
        {
            return PrimitiveSets.size();
        }

        void addPrimitiveSet(PrimitiveSet::Mode type, unsigned int begin, unsigned int count)
        {
            PrimitiveSet ps(type, begin, count);
            PrimitiveSets.push_back(ps);
        }

        //渲染段定义
        std::vector< PrimitiveSet > PrimitiveSets;
    };


    enum BufferType
    {
        BT_NULL
        , BT_BufferChar
        , BT_BufferUChar
        , BT_BufferShort
        , BT_BufferUShort
        , BT_BufferInt
        , BT_BufferUInt
        , BT_BufferFloat
        , BT_BufferDouble

        , BT_BufferVec2b
        , BT_BufferVec2ub
        , BT_BufferVec2s
        , BT_BufferVec2us
        , BT_BufferVec2i
        , BT_BufferVec2ui
        , BT_BufferVec2f
        , BT_BufferVec2d

        , BT_BufferVec3b
        , BT_BufferVec3ub
        , BT_BufferVec3s
        , BT_BufferVec3us
        , BT_BufferVec3i
        , BT_BufferVec3ui
        , BT_BufferVec3f
        , BT_BufferVec3d

        , BT_BufferVec4b
        , BT_BufferVec4ub
        , BT_BufferVec4s
        , BT_BufferVec4us
        , BT_BufferVec4i
        , BT_BufferVec4ui
        , BT_BufferVec4f
        , BT_BufferVec4d

        , BT_COUNT
    };

    //可变类型的数组基础类
    class EGG_ENGINE_EXPORT Array
        :public Object
    {
    public:

        Array(unsigned int size = 0) {}
        virtual ~Array() {}

        virtual unsigned int getSize() = 0;
        virtual unsigned int getSizeOf() = 0;
        virtual unsigned int getSizeOfComponents() = 0;

        virtual std::string getTypeID() = 0;
        virtual char* getData() = 0;

        //virtual BufferType getBufferType() = 0;
        //virtual DataType getDataType() = 0;
        //virtual void setDataType(DataType t) = 0;
    };

    //可变类型数组
    template<typename T, BufferType buffer_type, int components>
    class /*EGG_ENGINE_EXPORT*/ ArrayT
        :public Array
    {
    public:

        typedef T datatype;
        //const BufferType buffertype = buffer_type;
        static  BufferType getBufferType() { return buffer_type; }
        static  int getNumComponents() { return components; }

        ArrayT(/*unsigned int components  ,*/unsigned int count )
        {
            //mComponents = components;
            buffer.resize(sizeof(T) * components);
        }
        virtual ~ArrayT(){}

        virtual unsigned int getSize() override
        {
            return buffer.size();
        }
        virtual unsigned int getSizeOf()override
        {
            return sizeof(T);
        }
        virtual unsigned int getSizeOfComponents()override
        {
            return components;
        }
        virtual std::string getTypeID() override
        {
            return typeid(T).name();
        }
        virtual char* getData() override
        {
            return (char*)&buffer[0];
        }
        //virtual BufferType getBufferType() override
        //{
        //    return buffer_type;
        //}

        //virtual DataType getDataType() override
        //{
        //    if( T == float ){return ARRAY_FLOAT}
        //}
        //virtual void setDataType(DataType t) override{}

        inline T& operator [] (unsigned int i)
        {
            return buffer[i];
        }
        inline T operator [] (unsigned int i) const
        {
            return buffer[i];
        }
        //inline std::vector<T>& operator = () const
        //{
        //    return buffer;
        //}

    private:
        std::vector<T> buffer;
        //unsigned int mComponents;
    };



    //1
    typedef ArrayT<char, BT_BufferChar, 1>  BufferChar;
    typedef ArrayT<unsigned char, BT_BufferUChar, 1>  BufferUChar;
    typedef ArrayT<short, BT_BufferShort, 1>  BufferShort;
    typedef ArrayT<unsigned short, BT_BufferUShort, 1>  BufferUShort;
    typedef ArrayT<int, BT_BufferInt, 1>  BufferInt;
    typedef ArrayT<unsigned int, BT_BufferUInt, 1>  BufferUInt;

    typedef ArrayT<float, BT_BufferFloat, 1>  BufferFloat;
    typedef ArrayT<double, BT_BufferDouble, 1>  BufferDouble;

    //2
    typedef ArrayT<Eigen::Matrix<char, 2, 1, Eigen::DontAlign>, BT_BufferVec2b, 2>  BufferVec2b;
    typedef ArrayT<Eigen::Matrix<unsigned char, 2, 1, Eigen::DontAlign>, BT_BufferVec2ub, 2>  BufferVec2ub;
    typedef ArrayT<Eigen::Matrix<short, 2, 1, Eigen::DontAlign>, BT_BufferVec2s, 2>  BufferVec2s;
    typedef ArrayT<Eigen::Matrix<unsigned short, 2, 1, Eigen::DontAlign>, BT_BufferVec2us, 2>  BufferVec2us;
    typedef ArrayT<Eigen::Matrix<int, 2, 1, Eigen::DontAlign>, BT_BufferVec2i, 2>  BufferVec2i;
    typedef ArrayT<Eigen::Matrix<unsigned int, 2, 1, Eigen::DontAlign>, BT_BufferVec2ui, 2>  BufferVec2ui;

    typedef ArrayT<Eigen::Matrix<float, 2, 1, Eigen::DontAlign>, BT_BufferVec2f, 2>  BufferVec2f;
    typedef ArrayT<Eigen::Matrix<double, 2, 1, Eigen::DontAlign>, BT_BufferVec2d, 2>  BufferVec2d;

    //typedef ArrayT<Eigen::Vector2f, BT_BufferVec2f, 2>  BufferVec2f;
    //typedef ArrayT<Eigen::Vector2d, BT_BufferVec2d, 2>  BufferVec2d;

    //3
    typedef ArrayT<Eigen::Matrix<char, 3, 1, Eigen::DontAlign>, BT_BufferVec3b, 3>  BufferVec3b;
    typedef ArrayT<Eigen::Matrix<unsigned char, 3, 1, Eigen::DontAlign>, BT_BufferVec3ub, 3>  BufferVec3ub;
    typedef ArrayT<Eigen::Matrix<short, 3, 1, Eigen::DontAlign>, BT_BufferVec3s, 3>  BufferVec3s;
    typedef ArrayT<Eigen::Matrix<unsigned short, 3, 1, Eigen::DontAlign>, BT_BufferVec3us, 3>  BufferVec3us;
    typedef ArrayT<Eigen::Matrix<int, 3, 1, Eigen::DontAlign>, BT_BufferVec3i, 3>  BufferVec3i;
    typedef ArrayT<Eigen::Matrix<unsigned int, 3, 1, Eigen::DontAlign>, BT_BufferVec3ui, 3>  BufferVec3ui;

    typedef ArrayT<Eigen::Matrix<float, 3, 1, Eigen::DontAlign>, BT_BufferVec3f, 3>  BufferVec3f;
    typedef ArrayT<Eigen::Matrix<double, 3, 1, Eigen::DontAlign>, BT_BufferVec3d, 3>  BufferVec3d;

    //typedef ArrayT<Eigen::Vector3f, BT_BufferVec3f, 3>  BufferVec3f;
    //typedef ArrayT<Eigen::Vector3d, BT_BufferVec3d, 3>  BufferVec3d;

    //4
    typedef ArrayT<Eigen::Matrix<char, 4, 1, Eigen::DontAlign>, BT_BufferVec4b, 4>  BufferVec4b;
    typedef ArrayT<Eigen::Matrix<unsigned char, 4, 1, Eigen::DontAlign>, BT_BufferVec4ub, 4>  BufferVec4ub;
    typedef ArrayT<Eigen::Matrix<short, 4, 1, Eigen::DontAlign>, BT_BufferVec4s, 4>  BufferVec4s;
    typedef ArrayT<Eigen::Matrix<unsigned short, 4, 1, Eigen::DontAlign>, BT_BufferVec4us, 4>  BufferVec4us;
    typedef ArrayT<Eigen::Matrix<int, 4, 1, Eigen::DontAlign>, BT_BufferVec4i, 4>  BufferVec4i;
    typedef ArrayT<Eigen::Matrix<unsigned int, 4, 1, Eigen::DontAlign>, BT_BufferVec4ui, 4>  BufferVec4ui;

    typedef ArrayT<Eigen::Matrix<float, 4, 1, Eigen::DontAlign>, BT_BufferVec4f, 4>  BufferVec4f;
    typedef ArrayT<Eigen::Matrix<double, 4, 1, Eigen::DontAlign>, BT_BufferVec4d, 4>  BufferVec4d;

    //typedef ArrayT<Eigen::Vector4f, BT_BufferVec4f, 4>  BufferVec4f;
    //typedef ArrayT<Eigen::Vector4d, BT_BufferVec4d, 4>  BufferVec4d;



    //定点属性说明
    class EGG_ENGINE_EXPORT VectexAttribute
        :public Object
    {
    public:

        VectexAttribute(const std::string& name = ""
            , unsigned int noc = 3, egg::DataType type = egg::DT_FLOAT ,bool normalized = false
            , unsigned int stride = 0 , const std::string& buffer_name = "" )
        {
            this->name = name;
            this->noc = noc;
            this->type = type;
            this->normalized = normalized;
            this->stride = stride;
            this->buffer_name = buffer_name;
        }

        virtual ~VectexAttribute()
        {
        }

        //名称 通道号 分量数 数据类型
        std::string name;    //VA名称
        unsigned int noc;    /*Number of components*/ //1 2 3 4(default)
        egg::DataType type; //GL_BYTE,GL_UNSIGNED_BYTE, GL_SHORT,GL_UNSIGNED_SHORT,GL_FIXED, or GL_FLOAT(default)
        bool normalized;     // defualt false
        unsigned int stride; // 0(default)
        //std::string maya_semantics_name;
        std::string buffer_name;  //链接到buffer的名称
    };

    class nv_render;

    //可渲染对象
    class EGG_ENGINE_EXPORT Drawable
        :public Resource
    {
    public:
        Drawable() {}
        virtual ~Drawable() {}

        virtual void draw(nv_render* nv , int index = -1) = 0;
    };

    //资源对象
    //一个VBO的存储单位
    //  存放一组VBO对象  单VBO数组  VBO数组+index数组  离散VBO
    class EGG_ENGINE_EXPORT Geometry
        //:public Resource
        :public Drawable
    {
    public:
        Geometry(){}
        virtual ~Geometry(){}

        //virtual std::string getExt(){return "geometry";}
        virtual std::string ObjectClassName() override {return "geometry";} 

        virtual void draw(nv_render* nv, int index = -1) override;

        //查询包含多少渲染段
        unsigned int getNumSubGeometry(){return mSubGeometry.size();}

        //渲染指定段
        //void renderSubGeometry(nbPlugins::Renderer* render, Material::Pass* p, unsigned int id);

        //template <typename T>
        std::string getDataTypeName(BufferType t) const
        {
#if 0
            //if(typeid(T) == typeid(bool)) return "bool";
            if(typeid(T) == typeid(char)) return "char";
            if(typeid(T) == typeid(unsigned char)) return "uchar";
            if(typeid(T) == typeid(short)) return "short";
            if(typeid(T) == typeid(unsigned short)) return "ushort";
            if(typeid(T) == typeid(int)) return "int";
            if(typeid(T) == typeid(unsigned int)) return "uint";
            //if(typeid(T) == typeid(long)) return "long";
            //if(typeid(T) == typeid(unsigned long)) return "ulong";
            //if(typeid(T) == typeid(long long)) return "longlong";
            //if(typeid(T) == typeid(unsigned long)) return "ulonglong";
            if(typeid(T) == typeid(float)) return "float";
            if(typeid(T) == typeid(double)) return "double";
            //if(typeid(T) == typeid(std::string)) return "string";
            return "void";
#endif 
            if (t == BT_BufferChar) return "char";
            if (t == BT_BufferUChar) return "uchar";
            if (t == BT_BufferShort) return "short";
            if (t == BT_BufferUShort) return "ushort";
            if (t == BT_BufferInt) return "int";
            if (t == BT_BufferUInt) return "unit";
            if (t == BT_BufferFloat) return "float";
            if (t == BT_BufferDouble) return "double";

            if (t == BT_BufferVec2b) return "vec2b";
            if (t == BT_BufferVec2ub) return "vec2ub";
            if (t == BT_BufferVec2s) return "vec2s";
            if (t == BT_BufferVec2us) return "vec2us";
            if (t == BT_BufferVec2i) return "vec2i";
            if (t == BT_BufferVec2ui) return "vec2ui";
            if (t == BT_BufferVec2f) return "vec2f";
            if (t == BT_BufferVec2d) return "vec2d";

            if (t == BT_BufferVec3b) return "vec3b";
            if (t == BT_BufferVec3ub) return "vec3ub";
            if (t == BT_BufferVec3s) return "vec3s";
            if (t == BT_BufferVec3us) return "vec3us";
            if (t == BT_BufferVec3i) return "vec3i";
            if (t == BT_BufferVec3ui) return "vec3ui";
            if (t == BT_BufferVec3f) return "vec3f";
            if (t == BT_BufferVec3d) return "vec3d";

            if (t == BT_BufferVec4b) return "vec4b";
            if (t == BT_BufferVec4ub) return "vec4ub";
            if (t == BT_BufferVec4s) return "vec4s";
            if (t == BT_BufferVec4us) return "vec4us";
            if (t == BT_BufferVec4i) return "vec4i";
            if (t == BT_BufferVec4ui) return "vec4ui";
            if (t == BT_BufferVec4f) return "vec4f";
            if (t == BT_BufferVec4d) return "vec4d";

            return "null";
        }

        template <typename T>
        void save1dBuffer( std::ostream& os , Serializer* ser  ,Array* arr) const
        {
            //if(arr->getBufferType() != T().buffertype) return;

            T* at = dynamic_cast< T* >(arr);
            if(!at) return;

            //写入类型
            ser->writeString(os, getDataTypeName(T::getBufferType()));

            //写入数据
            unsigned int arr_num = arr->getSize();
            for (unsigned int i = 0; i < arr_num; i++)
            {
                if (arr->getSizeOfComponents() == 1)
                {
                    //一维数据
                    ser->writeNumber(os, (*at)[i]); 
                }
                else
                {
                    //多维数据
                    for (size_t e = 0; e < arr->getSizeOfComponents(); e++)
                    {
                        //ser->writeNumber(os, (*at)[i][e]);
                    }
                }
            }
        }
        template <typename T>
        void save234dBuffer(std::ostream& os, Serializer* ser, Array* arr) const
        {
            T* at = dynamic_cast<T*>(arr);
            if (!at) return;

            //写入类型
            ser->writeString(os, getDataTypeName(T::getBufferType()));

            //写入数据
            unsigned int arr_num = arr->getSize();
            for (unsigned int i = 0; i < arr_num; i++)
            {
                //多维数据
                for (size_t e = 0; e < arr->getSizeOfComponents(); e++)
                {
                    ser->writeNumber(os, (*at)[i][e]);
                }
            }
        }

#if 1
        template <typename T>
        void loat1dBuffer( std::istream& is , Serializer* ser ,std::string buffer_type ,std::string buffer_name, int buffer_count)
        {
            //判断类型
            if( buffer_type != getDataTypeName(T::getBufferType()) ) return;

            //读数组
            T* at = new T(buffer_count);
            mBuffers[buffer_name] = at;
            for (int j = 0; j < buffer_count; j++)
            {
                //一维数据
                ser->readNumber( is , (*at)[j] );
            }
        }
        template <typename T>
        void loat234dBuffer(std::istream& is, Serializer* ser, std::string buffer_type, std::string buffer_name, int buffer_count)
        {
            //判断类型
            if (buffer_type != getDataTypeName(T::getBufferType())) return;


            //读数组
            T* at = new T(buffer_count);
            mBuffers[buffer_name] = at;
            for (int j = 0; j < buffer_count; j++)
            {
                for (int e = 0; e < at->getSizeOfComponents(); e++)
                {
                    //多维数据
                    ser->readNumber(is, (*at)[j][e]);
                }
            }
        }
#endif

        virtual void save( egg::Path filename , std::ostream& os , Serializer* ser ) const override
        {
            Resource::save(filename ,os,ser);

            //写入所有buffer
            ser->writeNumber( os , (unsigned int)mBuffers.size() ); //通道数
            for (auto it = mBuffers.begin(); it != mBuffers.end(); it ++)
            {
                //通道名称
                ser->writeString( os , it->first ); 

                ////分量
                Array* arr = it->second.get();
                //unsigned int arr_com = arr->getSizeOfComponents();
                //ser->writeNumber(os, arr_com);

                //数量
                unsigned int arr_num = arr->getSize();
                ser->writeNumber( os , arr_num ); 
                
                save1dBuffer<BufferChar>(os, ser, arr);
                save1dBuffer<BufferUChar>(os, ser, arr);
                save1dBuffer<BufferShort>(os, ser, arr);
                save1dBuffer<BufferUShort>(os, ser, arr);
                save1dBuffer<BufferInt>(os, ser, arr);
                save1dBuffer<BufferUInt>(os, ser, arr);
                save1dBuffer<BufferFloat>(os, ser, arr);
                save1dBuffer<BufferDouble>(os, ser, arr);

                save234dBuffer<BufferVec2b>(os, ser, arr);
                save234dBuffer<BufferVec2ub>(os, ser, arr);
                save234dBuffer<BufferVec2s>(os, ser, arr);
                save234dBuffer<BufferVec2us>(os, ser, arr);
                save234dBuffer<BufferVec2i>(os, ser, arr);
                save234dBuffer<BufferVec2ui>(os, ser, arr);
                save234dBuffer<BufferVec2f>(os, ser, arr);
                save234dBuffer<BufferVec2d>(os, ser, arr);

                save234dBuffer<BufferVec3b>(os, ser, arr);
                save234dBuffer<BufferVec3ub>(os, ser, arr);
                save234dBuffer<BufferVec3s>(os, ser, arr);
                save234dBuffer<BufferVec3us>(os, ser, arr);
                save234dBuffer<BufferVec3i>(os, ser, arr);
                save234dBuffer<BufferVec3ui>(os, ser, arr);
                save234dBuffer<BufferVec3f>(os, ser, arr);
                save234dBuffer<BufferVec3d>(os, ser, arr);

                save234dBuffer<BufferVec4b>(os, ser, arr);
                save234dBuffer<BufferVec4ub>(os, ser, arr);
                save234dBuffer<BufferVec4s>(os, ser, arr);
                save234dBuffer<BufferVec4us>(os, ser, arr);
                save234dBuffer<BufferVec4i>(os, ser, arr);
                save234dBuffer<BufferVec4ui>(os, ser, arr);
                save234dBuffer<BufferVec4f>(os, ser, arr);
                save234dBuffer<BufferVec4d>(os, ser, arr);

                //当前数组内容
                //saveArray<char>(os,ser,arr);
                //saveArray<unsigned char>(os,ser,arr);
                //saveArray<short>(os,ser,arr);
                //saveArray<unsigned short>(os,ser,arr);
                //saveArray<int>(os,ser,arr);
                //saveArray<unsigned int>(os,ser,arr);
                //saveArray<float>(os,ser,arr);
                //saveArray<double>(os,ser,arr);
            }

            //写入SubGeometry
            ser->writeNumber( os , (unsigned int)mSubGeometry.size() ); //SubGeometry数量
            for (unsigned int i = 0; i < (unsigned int)mSubGeometry.size(); i++)
            {
                SubGeometry* sub = mSubGeometry[i].get();
                ser->writeNumber( os , (unsigned int)sub->PrimitiveSets.size() ); //PrimitiveSets数量
                for (unsigned int i = 0; i < (unsigned int)sub->PrimitiveSets.size(); i++)
                {
                    PrimitiveSet pset = sub->PrimitiveSets[0];
                    ser->writeNumber( os , (int)pset.mType );  //类型
                    ser->writeNumber( os , pset.mBegin ); //开始位置
                    ser->writeNumber( os , pset.mCount ); //长度
                }
            }

            //写入所有VA定义
            ser->writeNumber( os , (unsigned int)mVectexAttribute.size() ); //通道数
            for (auto it = mVectexAttribute.begin(); it != mVectexAttribute.end(); it ++)
            {
                //ser->writeString( os , it->first ); //通道名称

                VectexAttribute* va = it->second.get();
                ser->writeString( os , va->name );
                ser->writeNumber( os , va->noc );
                ser->writeNumber( os , (int)va->type );
                ser->writeNumber( os , (int)va->normalized );
                ser->writeNumber( os , va->stride );
                ser->writeString( os , va->buffer_name );
            }

        }

        virtual void load( egg::Path filename , std::istream& is , Serializer* ser , ObjectManager* om
            , egg::Referenced* rmg, bool load_ext) override
        {
            Resource::load(filename ,is,ser,om,rmg,load_ext);

            //读所有buffer
            unsigned int buffer_count = 0;
            ser->readNumber( is , buffer_count ); //通道数
            for (unsigned int i = 0; i < buffer_count; i++)
            {
                std::string buffer_name;
                ser->readString( is , buffer_name ); //通道名称

                //unsigned int buffer_com = 0;
                //ser->readNumber( is , buffer_com); //分量

                unsigned int buffer_count = 0;
                ser->readNumber(is, buffer_count); //数量

                std::string buffer_type;
                ser->readString( is , buffer_type );//读类型


                //1
                loat1dBuffer<BufferChar>(is, ser, buffer_type, buffer_name, buffer_count);
                loat1dBuffer<BufferUChar>(is, ser, buffer_type, buffer_name, buffer_count);
                loat1dBuffer<BufferShort>(is, ser, buffer_type, buffer_name, buffer_count);
                loat1dBuffer<BufferUShort>(is, ser, buffer_type, buffer_name, buffer_count);
                loat1dBuffer<BufferInt>(is, ser, buffer_type, buffer_name, buffer_count);
                loat1dBuffer<BufferUInt>(is, ser, buffer_type, buffer_name, buffer_count);
                loat1dBuffer<BufferFloat>(is, ser, buffer_type, buffer_name, buffer_count);
                loat1dBuffer<BufferDouble>(is, ser, buffer_type, buffer_name, buffer_count);

                loat234dBuffer<BufferVec2b>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec2ub>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec2s>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec2us>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec2i>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec2ui>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec2f>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec2d>(is, ser, buffer_type, buffer_name, buffer_count);

                loat234dBuffer<BufferVec3b>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec3ub>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec3s>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec3us>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec3i>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec3ui>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec3f>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec3d>(is, ser, buffer_type, buffer_name, buffer_count);

                loat234dBuffer<BufferVec4b>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec4ub>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec4s>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec4us>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec4i>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec4ui>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec4f>(is, ser, buffer_type, buffer_name, buffer_count);
                loat234dBuffer<BufferVec4d>(is, ser, buffer_type, buffer_name, buffer_count);

                //loatArray<char>(is, ser, buffer_type, buffer_name, buffer_com, buffer_count);
                //loatArray<unsigned char>(is, ser, buffer_type, buffer_name, buffer_com, buffer_count);
                //loatArray<short>(is, ser, buffer_type, buffer_name, buffer_com, buffer_count);
                //loatArray<unsigned short>(is, ser, buffer_type, buffer_name, buffer_com, buffer_count);
                //loatArray<int>(is, ser, buffer_type, buffer_name, buffer_com, buffer_count);
                //loatArray<unsigned int>(is, ser, buffer_type, buffer_name, buffer_com, buffer_count);
                //loatArray<float>(is, ser, buffer_type, buffer_name, buffer_com, buffer_count);
                //loatArray<double>(is, ser, buffer_type, buffer_name, buffer_com, buffer_count);

            }


            //读SubGeometry
            unsigned int subgeometry_count = 0;
            ser->readNumber( is , subgeometry_count ); //SubGeometry数量
            for (unsigned int i = 0; i < subgeometry_count; i++)
            {
                SubGeometry* sub = new SubGeometry();
                mSubGeometry.push_back(sub);

                unsigned int primitivesets_count = 0;
                ser->readNumber( is , primitivesets_count ); //PrimitiveSets数量
                for (unsigned int i = 0; i < primitivesets_count; i++)
                {
                    PrimitiveSet pset;
                    ser->readNumber( is , (int&)pset.mType );  //类型
                    ser->readNumber( is , pset.mBegin ); //开始位置
                    ser->readNumber( is , pset.mCount ); //长度

                    sub->PrimitiveSets.push_back( pset );
                }
            }


            //读所有VA定义
            unsigned int va_count = 0;
            ser->readNumber( is , va_count ); //通道数
            for (unsigned int i = 0; i < va_count; i++)
            {
                //std::string buffer_name;
                //ser->readString( is , buffer_name ); //通道名称

                VectexAttribute* va = new VectexAttribute();
                ser->readString( is , va->name );
                ser->readNumber( is , va->noc );
                ser->readNumber( is , (int&)va->type );
                ser->readNumber( is , (int&)va->normalized );
                ser->readNumber( is , va->stride );
                ser->readString( is , va->buffer_name );

                mVectexAttribute[va->name] = va;
            }

        }

        //查询当前是不是index方式的buffer定义
        //  一个geometry只能设置一组VBO 所以这一组VBO只能是index或者raw
        bool isIndex() { return false; }
        DataType getIndexDataType() { return egg::DataType::DT_UNSIGNED_SHORT; }

        //子对象
        std::vector< egg::ref_ptr< SubGeometry> > mSubGeometry;

        //渲染属性数据表
        //  名称可是是任何字符串 因为他是通过va属性关联的名字
        std::map<std::string/* buffer name */, egg::ref_ptr< Array > > mBuffers;

        //VA定义
        //  名称必须为VA预定义宏
        std::map<std::string/* VA name */, egg::ref_ptr< VectexAttribute > > mVectexAttribute;

    //private:

        //vbo对象表
        //   名称和VA一对一
        std::map<std::string, RendererResourceSet<handle_geometry> > mVBOs;

    };

}//namespace nbSG

#endif//NIUBI_SG_GEOMETRY_H
