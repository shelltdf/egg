
#include "Export.h"

#include <iostream>

#include <fbxsdk.h>

//#include <nbSG/Public.h>
#include <eggEngine/Material.h>


using namespace fbxsdk;



class CustomMaterialProperty
{
public:
    CustomMaterialProperty()
        :_skin(false)
    {
    }
    bool _skin;
};

CustomMaterialProperty getCustomMaterialProperty(FbxSurfaceMaterial* pNode)
{
    FbxProperty p;
    CustomMaterialProperty ret;

    p = pNode->FindProperty("_skin", false);
    if (p.IsValid())
    {
        std::string nodeName = p.GetName();
        std::cout << "found property: " << nodeName << std::endl;
        ret._skin = p.Get<FbxBool>();
    }

    return ret;
}


egg::Material* LoadMaterialAttribute(FbxSurfaceMaterial* pSurfaceMaterial, std::map<std::string, std::string>& copy_image_list)
{
    //查询自定义属性
    CustomMaterialProperty cmp = getCustomMaterialProperty(pSurfaceMaterial);

    // Get the name of material  
    //pSurfaceMaterial->GetName();
    //printf("Material type = %s name = %s\n" , pSurfaceMaterial->GetClassId().GetName(),pSurfaceMaterial->GetName() );

    //建立材质
    egg::Material* mat = new egg::Material();
    mat->setUniformPath(pSurfaceMaterial->GetName());

    //初始化默认材质信息 如果不能被正确识别 就显示紫色
    egg::Material::Tech* tech = mat->getTech();
    egg::Pass* pass0 = new egg::Pass();
    tech->mPasses.push_back(pass0);

    float ambient[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float emission[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float opacity = 1.0f;
    float specular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };   //适应Lambert的默认值
    float shininess = 0.0f;                         //适应Lambert的默认值
    float reflectivity = 0.0f;                      //适应Lambert的默认值


    /*
    材质输出标准
    tex0 diffuse
    tex1 lightmap/ambient
    tex2 normal
    tex3 emission/illumination
    tex4 specular
    tex5 reflection
    */
    egg::Texture* tex_diffuse = 0;   //本色
    egg::Texture* tex_ambient = 0;   //lightmap
    egg::Texture* tex_bump = 0;   //normalmap
    egg::Texture* tex_emissive = 0;   //Emissive
    egg::Texture* tex_specular = 0;   //高光
    egg::Texture* tex_reflection = 0;   //反射


    // Phong material  
    if (pSurfaceMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
    {
        FbxDouble3 AmbientColor = ((FbxSurfacePhong*)pSurfaceMaterial)->Ambient;
        FbxDouble3 DiffuseColor = ((FbxSurfacePhong*)pSurfaceMaterial)->Diffuse;
        FbxDouble3 EmissiveColor = ((FbxSurfacePhong*)pSurfaceMaterial)->Emissive;
        FbxDouble3 SpecularColor = ((FbxSurfacePhong*)pSurfaceMaterial)->Specular;
        FbxDouble  Shininess = ((FbxSurfacePhong*)pSurfaceMaterial)->Shininess;
        FbxDouble  Opacity = ((FbxSurfacePhong*)pSurfaceMaterial)->TransparencyFactor;
        FbxDouble  Reflectivity = ((FbxSurfacePhong*)pSurfaceMaterial)->ReflectionFactor;

        tex_diffuse = LoadMaterialTexture(&((FbxSurfacePhong*)pSurfaceMaterial)->Diffuse, copy_image_list);    //本色
        tex_ambient = LoadMaterialTexture(&((FbxSurfacePhong*)pSurfaceMaterial)->Ambient, copy_image_list);    //lightmap
        tex_bump = LoadMaterialTexture(&((FbxSurfacePhong*)pSurfaceMaterial)->Bump, copy_image_list);       //normalmap
        if (!tex_bump)tex_bump = LoadMaterialTexture(&((FbxSurfacePhong*)pSurfaceMaterial)->NormalMap, copy_image_list);       //normalmap
        tex_emissive = LoadMaterialTexture(&((FbxSurfacePhong*)pSurfaceMaterial)->Emissive, copy_image_list);       //Emissive
        tex_specular = LoadMaterialTexture(&((FbxSurfacePhong*)pSurfaceMaterial)->Specular, copy_image_list);   //高光
        tex_reflection = LoadMaterialTexture(&((FbxSurfacePhong*)pSurfaceMaterial)->Reflection, copy_image_list); //反射

        //return mat;
    }

    // Lambert material  
    else if (pSurfaceMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
    {
        FbxDouble3 AmbientColor = ((FbxSurfaceLambert*)pSurfaceMaterial)->Ambient;
        FbxDouble3 DiffuseColor = ((FbxSurfaceLambert*)pSurfaceMaterial)->Diffuse;
        FbxDouble3 EmissiveColor = ((FbxSurfaceLambert*)pSurfaceMaterial)->Emissive;
        FbxDouble  Opacity = ((FbxSurfaceLambert*)pSurfaceMaterial)->TransparencyFactor;

        tex_ambient = LoadMaterialTexture(&((FbxSurfacePhong*)pSurfaceMaterial)->Ambient, copy_image_list);
        tex_diffuse = LoadMaterialTexture(&((FbxSurfacePhong*)pSurfaceMaterial)->Diffuse, copy_image_list);
        tex_bump = LoadMaterialTexture(&((FbxSurfacePhong*)pSurfaceMaterial)->Bump, copy_image_list);
        if (!tex_bump)tex_bump = LoadMaterialTexture(&((FbxSurfacePhong*)pSurfaceMaterial)->NormalMap, copy_image_list);       //normalmap
        tex_emissive = LoadMaterialTexture(&((FbxSurfacePhong*)pSurfaceMaterial)->Emissive, copy_image_list);

        //return mat;
    }

    //设置默认shader
    if (cmp._skin)
    {
        //pass0->setProgram(maya_skinning_normalmap_program.get());
    }
    else
    {
        //pass0->setProgram(maya_normalmap_program.get());
    }


    pass0->setUniform<float>(egg_FrontMaterial_ambient, egg::UniformBase::UniformDataType::UDT_FLOAT_VEC4, &ambient[0], 4, "");
    pass0->setUniform<float>(egg_FrontMaterial_diffuse, egg::UniformBase::UniformDataType::UDT_FLOAT_VEC4, &diffuse[0], 4, "");
    pass0->setUniform<float>(egg_FrontMaterial_emission, egg::UniformBase::UniformDataType::UDT_FLOAT_VEC4, &emission[0], 4, "");
    pass0->setUniform<float>(egg_FrontMaterial_opacity, egg::UniformBase::UniformDataType::UDT_FLOAT, &opacity, 1, "");
    pass0->setUniform<float>(egg_FrontMaterial_specular, egg::UniformBase::UniformDataType::UDT_FLOAT_VEC4, &specular[0], 4, "");
    pass0->setUniform<float>(egg_FrontMaterial_shininess, egg::UniformBase::UniformDataType::UDT_FLOAT, &shininess, 1, "");
    pass0->setUniform<float>(egg_FrontMaterial_reflectivity, egg::UniformBase::UniformDataType::UDT_FLOAT, &reflectivity, 1, "");

    if (tex_diffuse)
    {
        pass0->setTextureUnit(egg_texture0_enable, 1);
        pass0->setTextureUnit(egg_texture0, 0);
        pass0->setTexture(0, tex_diffuse);
    }
    if (tex_ambient)
    {
        pass0->setTextureUnit(egg_texture1_enable, 1);
        pass0->setTextureUnit(egg_texture1, 1);
        pass0->setTexture(1, tex_ambient);
    }
    if (tex_bump)
    {
        pass0->setTextureUnit(egg_texture2_enable, 1);
        pass0->setTextureUnit(egg_texture2, 2);
        pass0->setTexture(2, tex_bump);
    }
    if (tex_emissive)
    {
        pass0->setTextureUnit(egg_texture3_enable, 1);
        pass0->setTextureUnit(egg_texture3, 3);
        pass0->setTexture(3, tex_emissive);
    }
    if (tex_specular)
    {
        pass0->setTextureUnit(egg_texture4_enable, 1);
        pass0->setTextureUnit(egg_texture4, 4);
        pass0->setTexture(4, tex_specular);
    }
    if (tex_reflection)
    {
        pass0->setTextureUnit(egg_texture5_enable, 1);
        pass0->setTextureUnit(egg_texture5, 5);
        pass0->setTexture(5, tex_reflection);
    }

    //默认开关设置
    int skinning = 0;
    pass0->setUniform<int>("skinning", egg::UniformBase::UniformDataType::UDT_INT, &skinning, 1, "");

    return mat;
}

