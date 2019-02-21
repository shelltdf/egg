
#include <eggEngine/Pass.h>

using namespace egg;


void CustomMaterialProperty::save( /*nbBase::Path filename ,*/ std::ostream& os, Serializer* ser) const
{
    ser->writeNumber(os, (int)_alpha_test);
    ser->writeNumber(os, _alpha_ref);
    ser->writeNumber(os, _alpha_mode);

    ser->writeNumber(os, (int)_blend);
    ser->writeNumber(os, _blend_src);
    ser->writeNumber(os, _blend_dst);

    ser->writeNumber(os, (int)_depth_test);
    ser->writeNumber(os, _depth_mode);
    ser->writeNumber(os, (int)_depth_write);
    ser->writeNumber(os, _depth_near);
    ser->writeNumber(os, _depth_far);

    ser->writeNumber(os, _reflect_type);
    ser->writeNumber(os, (int)_cubemap_z_flip);
    ser->writeNumber(os, (int)_normalmap_x_flip);
    ser->writeNumber(os, (int)_normalmap_y_flip);

    ser->writeNumber(os, (int)_skin);
    ser->writeNumber(os, (int)_dither);
    ser->writeNumber(os, (int)_sample_alpha_to_coverage);
    ser->writeNumber(os, (int)_sample_coverage);
    ser->writeNumber(os, (int)_wire);
    ser->writeNumber(os, (int)_double_sided);

}

void CustomMaterialProperty::load( /*nbBase::Path filename ,*/ std::istream& is, Serializer* ser
    //, ObjectManager* om, nbBase::Referenced* rmg, bool load_ext
)
{
    ser->readNumber(is, (int&)_alpha_test);
    ser->readNumber(is, _alpha_ref);
    ser->readNumber(is, _alpha_mode);

    ser->readNumber(is, (int&)_blend);
    ser->readNumber(is, _blend_src);
    ser->readNumber(is, _blend_dst);

    ser->readNumber(is, (int&)_depth_test);
    ser->readNumber(is, _depth_mode);
    ser->readNumber(is, (int&)_depth_write);
    ser->readNumber(is, _depth_near);
    ser->readNumber(is, _depth_far);

    ser->readNumber(is, _reflect_type);
    ser->readNumber(is, (int&)_cubemap_z_flip);
    ser->readNumber(is, (int&)_normalmap_x_flip);
    ser->readNumber(is, (int&)_normalmap_y_flip);

    ser->readNumber(is, (int&)_skin);
    ser->readNumber(is, (int&)_dither);
    ser->readNumber(is, (int&)_sample_alpha_to_coverage);
    ser->readNumber(is, (int&)_sample_coverage);
    ser->readNumber(is, (int&)_wire);
    ser->readNumber(is, (int&)_double_sided);
}


#if 0
void bind(nbPlugins::Renderer* render, vmml::mat4f& mv, vmml::mat4f& proj)
{
    if (!mProgram->getResource()) return;

    //bind program
    mProgram->getResource()->bind(render, mv, proj);
    nbPlugins::Renderer::handle_program handle = getProgram()->getProgram();

    //set uniform
    for (auto it_uni = mUniforms.begin(); it_uni != mUniforms.end(); it_uni++)
    {
        UniformBase* ub = it_uni->second.get();
        unsigned int noc = 0;

        if (it_uni->second->getType() == UniformBase::UDT_INT
            || it_uni->second->getType() == UniformBase::UDT_SAMPLER_2D
            || it_uni->second->getType() == UniformBase::UDT_SAMPLER_CUBE
            || it_uni->second->getType() == UniformBase::UDT_FLOAT
            )
        {
            noc = 1;
        }

        if (it_uni->second->getType() == UniformBase::UDT_INT_VEC2
            || it_uni->second->getType() == UniformBase::UDT_FLOAT_VEC2
            )
        {
            noc = 2;
        }

        if (it_uni->second->getType() == UniformBase::UDT_INT_VEC3
            || it_uni->second->getType() == UniformBase::UDT_FLOAT_VEC3
            )
        {
            noc = 3;
        }

        //只处理int float matrix
        switch (it_uni->second->getType())
        {
        case UniformBase::UDT_SAMPLER_2D:       //强行记录为int
        case UniformBase::UDT_SAMPLER_CUBE:     //强行记录为int
        case UniformBase::UDT_INT:
        case UniformBase::UDT_INT_VEC2:
        case UniformBase::UDT_INT_VEC3:
        case UniformBase::UDT_INT_VEC4:
        {
            Uniform<int>* u = dynamic_cast<Uniform<int>*>(ub);
            if (u)
            {
                render->setUniform(handle, ub->getName().c_str(), noc, ub->getCount(), u->getDataPtr());
            }
        }
        break;

        case UniformBase::UDT_FLOAT:
        case UniformBase::UDT_FLOAT_VEC2:
        case UniformBase::UDT_FLOAT_VEC3:
        case UniformBase::UDT_FLOAT_VEC4:
        {
            Uniform<float>* u = dynamic_cast<Uniform<float>*>(ub);
            if (u)
            {
                render->setUniform(handle, ub->getName().c_str(), noc, ub->getCount(), u->getDataPtr());
            }
        }
        break;

        case UniformBase::UDT_MAT2:
        case UniformBase::UDT_MAT3:
        case UniformBase::UDT_MAT4:
        {
            Uniform<float>* u = dynamic_cast<Uniform<float>*>(ub);
            if (u)
            {
                render->setUniformMatrix(handle, ub->getName().c_str(), ub->getCount() / 16, u->getDataPtr());
            }
        }
        break;
        default:
            break;
        }//switch

    }//for uniform

     //bind texture
    for (unsigned int i = 0; i < getNumTexture(); i++)
    {
        Texture* tex = getTexture(i);
        if (!tex) continue;

        //输入不存在就建立对应的GL对象
        if (tex->mGLHandle == 0)
        {
            Image* image = tex->getImage();
            if (image == 0) continue;

            //查询外部路径作为内存和外存的区别
            unsigned char* image_data_prt = image->getData();
            if (image_data_prt)
            {
                //内存中存在image的数据
                tex->mGLHandle = render->createTexture();

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

                render->setTextureData(tex->mGLHandle, image->getS(), image->getT(), (char*)image_data_prt, image->getPixelFormat());

            }
        }//if(tex->mGLHandle == 0)

         //如果句柄存在就bind
        if (tex->mGLHandle > 0)
        {
            //render->enable(nbPlugins::Renderer::TEXTURE_2D);//默认开启
            render->bindTexture(tex->mGLHandle, i);
        }

    }//for textrue
}

void unBind()
{
    if (mProgram->getResource()) mProgram->getResource()->unBind();
}
#endif

//通用uniform操作
bool Pass::hasUniform(const std::string& name)
{
    auto it = mUniforms.find(name);
    return (it != mUniforms.end());
}
void Pass::delUniform(const std::string& name)
{
    auto it = mUniforms.find(name);
    if (it != mUniforms.end())
    {
        mUniforms.erase(it);
    }
}
unsigned int Pass::getUniformNum()
{
    return mUniforms.size();
}
std::vector< std::string > Pass::getUniformNames()
{
    std::vector< std::string > ret;
    for (auto it = mUniforms.begin(); it != mUniforms.end(); it++)
    {
        ret.push_back(it->first);
    }
    return ret;
}

//uniform信息查询
void Pass::getUniformInfo(const std::string& name, UniformBase::UniformDataType& type, unsigned int& count, std::string& semantics)
{
    if (!hasUniform(name)) return;
    UniformBase* u = mUniforms[name].get();
    type = u->getType();
    count = u->getCount();
    semantics = u->getSemantics();
}


//texture通道类型操作 sampler 强制用int取代 主要是为了兼容性和直接转换
void Pass::setTextureUnit(const std::string& name, int int_id)
{
    mUniforms[name] = new Uniform<int>(name, UniformBase::UniformDataType::UDT_SAMPLER_2D, int_id, "");
}
unsigned int Pass::getTextureUnit(const std::string& name, int& int_id)
{
    auto it = mUniforms.find(name);
    if (it != mUniforms.end())
    {
        Uniform<int>* u = dynamic_cast<Uniform<int>*>(it->second.get());
        int_id = u->getData(0);
        return int_id;
    }
    return 0;
}


void Pass::changeProgramSetting(Program* program)
{
    //记录当前的属性

    //使用默认值覆盖
    changeDefaultProgramSetting();

    //根据重名重新还原属性
    // 如果没有重名就等于是全部恢复默认值了


}

void Pass::changeDefaultProgramSetting()
{

}


