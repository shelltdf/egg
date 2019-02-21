
#include <eggEngine/Material.h>
#include <eggEngine/ResourceManager.h>

using namespace egg;


Material::Material()
{
    mCurrentTech = NB_DEFAULT_THCH_NAME;

    //默认tech
    Tech* tech = new Tech();
    mTechs[NB_DEFAULT_THCH_NAME] = tech;

    //默认pass
    Pass* p0 = new Pass();
    tech->mPasses.push_back(p0);

    //设置默认program
    std::string abs_program_path = "|assets|shaders|glcore|standard.program";
    ResourceManager* rm = ResourceManager::getInstance();
    Resource* resource = rm->readResource(abs_program_path,false);
    Program* program = dynamic_cast<Program*>(resource);
    p0->setProgram(program, egg::ResourceLinkType::RLT_Absolute);

    //根据program的模板初始化pass的设置
    p0->changeProgramSetting(program);

}

Material::~Material()
{
}



//bool Material::isInited()
//{
//    if
//}
//bool Material::init()
//{
//}

#if 0
Material::Pass* Material::getOrCreatePass(unsigned int index, std::string tech_name)
{
    Material::Tech* t = getTech(tech_name);
    if (t)
    {
        Material::Pass* p = getPass(index, tech_name);
        if (!p)
        {
        }
        return p;
    }
    return 0;
}

Material::Pass* Material::getPass(unsigned int index, std::string tech_name)
{
    Material::Tech* t = getTech(tech_name);
    if (t && (index < t->mPasses.size()))
    {
        return t->mPasses[index].get();
    }
    return 0;
}

unsigned int Material::getNumPass(std::string tech_name)
{
    Material::Tech* t = getTech(tech_name);
    if (t)
    {
        return t->mPasses.size();
    }
    return 0;
}

void Material::removePass(unsigned int index, std::string tech_name)
{
    Material::Tech* t = getTech(tech_name);
    if (t)
    {
    }
}

void Material::removePass(Pass* pass, std::string tech_name)
{
    Material::Tech* t = getTech(tech_name);
    if (t)
    {
    }
}
#endif

std::vector<std::string > Material::getTechNames()
{
    std::vector< std::string > ret;
    for (auto it = mTechs.begin(); it != mTechs.end(); it++)
    {
        ret.push_back(it->first);
    }
    return ret;
}

Material::Tech* Material::getOrCreateTech(std::string tech_name)
{
    Material::Tech* t = getTech(tech_name);
    if (!t)
    {
        //Tech tech();
        mTechs[tech_name] = new Tech();;
    }
    t = getTech(tech_name);
    return t;
}

Material::Tech* Material::getTech(std::string tech_name)
{
    auto it = mTechs.find(tech_name);
    if (it != mTechs.end())
    {
        return it->second.get();
    }
    return 0;
}

void Material::removeTech(std::string tech_name)
{
    for (auto it = mTechs.begin(); it != mTechs.end(); it++)
    {
        if (it->first == tech_name)
        {
            mTechs.erase(it);
            break;
        }
    }
}

void Material::removeTech(Tech* t)
{
    for (auto it = mTechs.begin(); it != mTechs.end(); it++)
    {
        if (it->second.get() == t)
        {
            mTechs.erase(it);
            break;
        }
    }
}


//收集已经加载的资源链接
void Material::collectionResourceObject(std::map< egg::Path, egg::ref_ptr< Resource > >& result)
{
    for (auto it = mTechs.begin(); it != mTechs.end(); it++)
    {
        unsigned int pass_num = it->second->mPasses.size();
        for (unsigned int i = 0; i < pass_num; i++)
        {
            Pass* pass = it->second->mPasses[i].get();

            //Texture
            for (auto itp = pass->mTexture.begin(); itp != pass->mTexture.end(); itp++)
            {
                if ((*itp).valid() && (*itp)->isLoaded())
                {
                    egg::Path p = (*itp)->getPath();
                    if (p.size() > 0)
                    {
                        result[p] = (*itp)->getResource();
                    }
                    else
                    {
                        printf("Material path is null\n");
                    }
                }
            }

            //Program
            if (pass->mProgram->isLoaded())
            {
                result[pass->mProgram->getPath()] = pass->mProgram->getResource();
            }

        }//pass
    }//tech
}

//收集尚未加载的资源路径
void Material::collectionResourceLink(std::vector< egg::ref_ptr< ResourceLink > >& result)
{
    for (auto it = mTechs.begin(); it != mTechs.end(); it++)
    {
        unsigned int pass_num = it->second->mPasses.size();
        for (unsigned int i = 0; i < pass_num; i++)
        {
            Pass* pass = it->second->mPasses[i].get();

            //Texture
            for (auto itp = pass->mTexture.begin(); itp != pass->mTexture.end(); itp++)
            {
                if ((*itp).valid() && !(*itp)->isLoaded())
                {
                    result.push_back((*itp).get());
                }
            }


            //Program
            if (!pass->mProgram->isLoaded())
            {
                result.push_back(pass->mProgram.get());
            }

        }//pass
    }//tech
}


void Material::save(egg::Path filename, std::ostream& os, Serializer* ser) const
{
    Resource::save(filename, os, ser);

    //当前tech
    ser->writeString(os, mCurrentTech);

    //写入所有attrbute
    //ser->writeNumber( os , mAttrbuteIDs.size() ); //通道数
    //for (auto it = mAttrbuteIDs.begin(); it != mAttrbuteIDs.end(); it ++)
    //{
    //    ser->writeString( os , it->first );   //通道名称
    //    ser->writeNumber( os , it->second );  //ID
    //}

    //写入所有tech
    ser->writeNumber(os, (unsigned int)mTechs.size()); //tech数
    for (auto it = mTechs.begin(); it != mTechs.end(); it++)
    {
        ser->writeString(os, it->first); //tech名称

        unsigned int pass_num = it->second->mPasses.size();
        ser->writeNumber(os, pass_num);  //pass数量

        for (unsigned int i = 0; i < pass_num; i++)
        {
            Pass* pass = it->second->mPasses[i].get();

            //附加属性
            pass->mCustomMaterialProperty.save(os, ser);

            //Program
            saveResourceLink<Program>(pass->mProgram.get(), egg::Path(filename), os, ser);

            //Texture
            unsigned int tex_num = pass->getNumTexture();
            ser->writeNumber(os, tex_num);  //Texture数量
            for (unsigned int j = 0; j < tex_num; j++)
            {
                if (pass->mTexture[j].get())
                {
                    ser->writeNumber(os, int(1)); //有
                    saveResourceLink<Texture>(pass->mTexture[j].get(), this->uniform_path, os, ser);
                }
                else
                {
                    ser->writeNumber(os, int(0)); //无
                }
            }

#if 0
            //VectexAttribute
            unsigned int va_num = pass->mAttributes.size();
            ser->writeNumber(os, va_num);  //VectexAttribute数量
            for (auto it_va = pass->mAttributes.begin(); it_va != pass->mAttributes.end(); it_va++)
            {
                VectexAttribute* va = (*it_va).get();//it_va->second.get();
                ser->writeString(os, va->name);
                ser->writeNumber(os, va->index);
                ser->writeNumber(os, va->noc);
                ser->writeNumber(os, va->type);
                ser->writeNumber(os, va->normalized);
                ser->writeNumber(os, va->stride);
                ser->writeString(os, va->maya_semantics_name);
            }
#endif

            //uniform
            unsigned int uniform_num = pass->mUniforms.size();
            ser->writeNumber(os, uniform_num);  //uniform数量
            for (auto it_uni = pass->mUniforms.begin(); it_uni != pass->mUniforms.end(); it_uni++)
            {
                ser->writeString(os, it_uni->second->getDataTypeName()); //类型
                it_uni->second->save(filename, os, ser);            //数据

                                                                    //ser->writeString(os,(*it_uni)->getDataTypeName()); //类型
                                                                    //(*it_uni)->save( filename , os , ser );            //数据
            }

        }//for pass_num
    }//for tech
}

void Material::load(egg::Path filename, std::istream& is, Serializer* ser, ObjectManager* om
    , egg::Referenced* rmg, bool load_ext)
{
    Resource::load(filename, is, ser, om, rmg, load_ext);

    //当前tech
    ser->readString(is, mCurrentTech);
    //ser->readNumber( is , (int&)mShaderType );


    //所有tech
    unsigned int tech_num = 0;
    ser->readNumber(is, tech_num); //tech数
    for (unsigned int t = 0; t < tech_num; t++)
    {
        std::string tech_name;
        ser->readString(is, tech_name); //tech名称

        Tech* tech = this->getOrCreateTech(tech_name);

        unsigned int pass_num = 0;
        ser->readNumber(is, pass_num);  //pass数量

        for (unsigned int p = 0; p < pass_num; p++)
        {
            Pass* pass = new Pass();
            tech->mPasses.push_back(pass);

            //附加属性
            pass->mCustomMaterialProperty.load(is, ser);

            //Program
            pass->mProgram = new ResourceLinkT< Program >();
            loadResourceLink<Program>(filename, pass->mProgram.get(), is, ser);

            //Texture
            unsigned int tex_num = 0;
            ser->readNumber(is, tex_num);  //Texture数量
            for (unsigned int t = 0; t < tex_num; t++)
            {
                int has = 0;
                ser->readNumber(is, has); //这个通道是否存在贴图

                if (has > 0)
                {
                    ResourceLinkT< Texture >* texture = new ResourceLinkT< Texture >();
                    loadResourceLink<Texture>(filename, texture, is, ser);

                    if (t >= pass->mTexture.size()) pass->mTexture.resize(t + 1);
                    pass->mTexture[t] = (texture);


                    //pass->setTexture(t,texture->getResource());
                    //Texture* rt = pass->getTexture(t);
                    //rt->setUniformPath(texture->getPath());
                }
            }
#if 0
            //VectexAttribute
            unsigned int va_num = 0;
            ser->readNumber(is, va_num);  //VectexAttribute数量
            for (unsigned int v = 0; v < tex_num; v++)
            {
                VectexAttribute* va = new VectexAttribute();
                ser->readString(is, va->name);
                ser->readNumber(is, va->index);
                ser->readNumber(is, va->noc);
                ser->readNumber(is, (int&)va->type);
                ser->readNumber(is, (int&)va->normalized);
                ser->readNumber(is, va->stride);
                ser->readString(is, va->maya_semantics_name);
                pass->mAttributes.push_back(va);
            }
#endif

            //uniform
            unsigned int uniform_num = 0;
            ser->readNumber(is, uniform_num);  //uniform数量
            for (unsigned int u = 0; u < uniform_num; u++)
            {
                std::string data_type;
                ser->readString(is, data_type); //读类型

                UniformBase* uniform = newUniform(data_type); //分析类型
                uniform->load(filename, is, ser);         //读取内容

                pass->mUniforms[uniform->getName()] = uniform;
                //pass->mUniforms.push_back(uniform);
            }

        }//for pass_num
    }//for tech

}

