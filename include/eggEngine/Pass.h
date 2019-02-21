#ifndef NIUBI_PASSS_H
#define NIUBI_PASSS_H

#include <eggEngine/Export.h>

#include <eggEngine/Program.h>
#include <eggEngine/Texture.h>

namespace egg
{

    /*
    *  材质辅助信息定义
    *       主要是为了从DCCT中输出扩展的属性
    *           扩展渲染状态       比如 alpha test 在不同的渲染实现里解释不同
    *           扩展uniform数值    比如 alpha ref值 （说起来很勉强 应该也没有什么绝对的uniform需要从这里设置）
    *           处理流程分拣属性   比如 skinning 就是用来分拣shader用的  normalmap反转属性 是辅助处理程序转换贴图用的
    *  注意 在max中的枚举是从1开始记录的
    */
    class EGG_ENGINE_EXPORT CustomMaterialProperty
    {
    public:
        CustomMaterialProperty()
            /*默认值的原则是即使不设置也可以正常渲染*/
            :_alpha_test(false), _alpha_ref(0.5), _alpha_mode(7 - 1)
            , _blend(false), _blend_src(5 - 1), _blend_dst(6 - 1)
            , _depth_test(true), _depth_mode(4 - 1), _depth_write(true), _depth_near(0.0), _depth_far(1.0)
            , _reflect_type(1 - 1), _cubemap_z_flip(false), _normalmap_x_flip(false), _normalmap_y_flip(false)
            , _skin(false)
            , _dither(false)
            , _sample_alpha_to_coverage(false)
            , _sample_coverage(false)
            , _wire(false)
            , _double_sided(false)
        {
        }

        bool _alpha_test;   //渲染状态
        float _alpha_ref;
        int _alpha_mode;

        bool _blend;        //渲染状态
        int _blend_src;
        int _blend_dst;

        bool _depth_test;   //渲染状态
        int _depth_mode;
        bool _depth_write;  //渲染状态
        float _depth_near;
        float _depth_far;

        int _reflect_type;      //提示反射贴图的类型
        bool _cubemap_z_flip;   //提示cubemap的z轴向    cubemap 只有z正反两种模式
        bool _normalmap_x_flip; //提示normalmap的x轴向  我猜z轴不可能反转 而且只可能是切线空间
        bool _normalmap_y_flip; //提示normalmap的y轴向

        bool _skin;                     //提示使用skinning的shader
        bool _dither;                   //渲染状态
        bool _sample_alpha_to_coverage; //渲染状态
        bool _sample_coverage;          //渲染状态
        bool _wire;                     //提示使用线框方式渲染
        bool _double_sided;             //提示关闭cull face


        void save( /*nbBase::Path filename ,*/ std::ostream& os, Serializer* ser) const;

        void load( /*nbBase::Path filename ,*/ std::istream& is, Serializer* ser
            //, ObjectManager* om, nbBase::Referenced* rmg, bool load_ext
        );
    };


    //Pass类型
    class EGG_ENGINE_EXPORT Pass
        :public Object
    {
    public:
        friend class Material;
        friend class MeshRender;

        Pass()
        {
            //mProgram = new Program();
        }
        virtual ~Pass() {}

        void changeProgramSetting(Program* program);
        void changeDefaultProgramSetting();

#if 0
        void bind(nbPlugins::Renderer* render, vmml::mat4f& mv, vmml::mat4f& proj);
        void unBind();
#endif

        //通用uniform操作
        bool hasUniform(const std::string& name);
        void delUniform(const std::string& name);
        unsigned int getUniformNum();
        std::vector< std::string > getUniformNames();

        //uniform信息查询
        void getUniformInfo(const std::string& name, UniformBase::UniformDataType& type, unsigned int& count, std::string& semantics);

        //数值类型uniform操作
        template <class T>
        bool setUniform(const std::string& name, UniformBase::UniformDataType t, T* v, unsigned int count, const std::string& semantics)
        {
            mUniforms[name] = new Uniform<T>(name, t, count, v, semantics);
            return true;
        }
        template <class T>
        bool getUniform(const std::string& name, T* v, unsigned int& count, std::string& semantics)
        {
            if (!hasUniform(name)) return false;
            UniformBase::UniformDataType type;
            UniformBase* ub = mUniforms[name].get();
            Uniform<T>* u = dynamic_cast<Uniform<T>*>(ub);
            type = u->getType();
            count = u->getCount();
            u->get(name, count, v, semantics);
            return true;
        }

        //texture通道类型操作 sampler 强制用int取代 主要是为了兼容性和直接转换
        void setTextureUnit(const std::string& name, int int_id);
        unsigned int getTextureUnit(const std::string& name, int& int_id);

        void setCustomMaterialProperty(CustomMaterialProperty cmp) { mCustomMaterialProperty = cmp; }
        CustomMaterialProperty getCustomMaterialProperty() { return mCustomMaterialProperty; }

        std::map<std::string, egg::ref_ptr<UniformBase> > getUniforms() { return mUniforms; }

        int getRenderOrder() { return mRenderOrder; }
        void setRenderOrder(int order) { mRenderOrder = order; }

    private:

        int mRenderOrder = 0;

        //附加定义属性
        CustomMaterialProperty mCustomMaterialProperty;

        //attribute对应关系 (这里只记录语义和名称的链接  VA名称记录在program里)
        //std::map<std::string, nbBase::ref_ptr<VectexAttribute> > mAttributes;
        //std::vector< nbBase::ref_ptr<VectexAttribute> > mAttributes;
        std::map<std::string/*attribute*/, std::string/*semantics*/ > mAttributeSemantics;

        //uniform对象
        std::map<std::string, egg::ref_ptr<UniformBase> > mUniforms;
        //std::vector< nbBase::ref_ptr<UniformBase> > mUniforms; //这里修改成set get del的接口 map表 

        //shader对象
        //nbBase::ref_ptr< Program > mProgram; //vs fs ...
        RESOURCE_LINK(Program);

        //texture
        RESOURCE_LINK_VECTOR(Texture);

    };

}//namespace egg

#endif //NIUBI_PASSS_H

