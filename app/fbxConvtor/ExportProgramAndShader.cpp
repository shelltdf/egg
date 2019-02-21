
#include <iostream>

#include <fbxsdk.h>

//#include <nbSG/Public.h>

using namespace fbxsdk;

#if 0


//这里仅仅只是建立了几个默认材质
//   maya一般只能输出 Phong Lambert
void ExportDefaultProgramAndShader(nbSG::ResourceManager* rmg)
{

    //default_normalmap_vs
    nbBase::ref_ptr<nbSG::Shader> default_normalmap_vs = new nbSG::Shader();
    default_normalmap_vs->setUniformPath("|shared|maya_normalmap_vs");
    {
        default_normalmap_vs->setShaderType(nbSG::Shader::NB_VERTEX_SHADER);
        default_normalmap_vs->setString(nbSG::Shader::ShaderProfileType::SPT_GL_CORE_330, "aasdasdasdasdasdasdasd asdasd asd 123123 asdas");

        default_normalmap_vs->setString(nbSG::Shader::ShaderProfileType::SPT_GLES_2,

            //"#version 110\n"
            //"precision mediump float;\n"
            "#define MAX_LIGHTS 8\n"

#include "gles_inverse.inc"
#include "gles_transpose.inc"
#include "uniform_LightSource.inc"

            "uniform mat4 nb_ModelViewMatrix; \n"
            "uniform mat4 nb_ProjectionMatrix; \n"

            "attribute vec3 nfx_va_position;\n"
            "attribute vec2 nfx_va_uv0; \n"
            "attribute vec2 nfx_va_uv1; \n"
            "attribute vec3 nfx_va_normal; \n"
            "attribute vec3 nfx_va_tangent0; \n"

            //输出
            "varying vec2 texCoordVar0; \n" //diffuse
            "varying vec2 texCoordVar1; \n" //lightmap
            "varying vec2 texCoordVar2; \n" //reflect
            "varying vec3 lightVec[MAX_LIGHTS];\n"
            "varying vec3 halfVec[MAX_LIGHTS];\n"
            "varying vec3 eyeVec;\n"
            "varying vec3 tangent;\n"

            //调试
            "varying vec4 debug_color; \n"


            "void main()\n"
            "{\n"


            //计算出NormalMatrix
            "    mat4 NormalMatrix = transpose(inverse( nb_ModelViewMatrix ));\n"


            //输出UV
            "    texCoordVar0 = nfx_va_uv0;\n"
            "    texCoordVar1 = nfx_va_uv1;\n"


            //计算骨骼影响
            "        vec4 pos = vec4(nfx_va_position.xyz,1.0);\n"
            "        vec3 nor = normalize(nfx_va_normal.xyz);\n"
            "        vec3 tan = normalize(nfx_va_tangent0.xyz);\n"


            //
            "    gl_Position = nb_ProjectionMatrix * nb_ModelViewMatrix * pos; \n"


            //计算TNB
            "    vec3 n = normalize(mat3(NormalMatrix) * nor);\n"  //这里绝对不能强制升级vec3为vec4
            "    vec3 t = normalize(mat3(NormalMatrix) * tan);\n"
            "    vec3 b = cross(n, t);\n"

            //输出tangent
            "    tangent = t;\n"

            //输出lightVec
            "    vec3 v;\n"
            "    vec3 vertexPosition = vec3(nb_ModelViewMatrix * pos);\n"
            "    int i;\n" //for i
            "    for (i=0; i<1/*nfx_LightSourceNum*/; ++i)\n"
            "    {\n"
            //"        vec3 lightDir = normalize(nfx_LightSource[i].position.xyz - vertexPosition);\n"
            "        vec3 lightDir = normalize(vec3(0,0,0) - vertexPosition);\n"
            "        v.x = dot(lightDir, t);\n"
            "        v.y = dot(lightDir, b);\n"
            "        v.z = dot(lightDir, n);\n"
            "        lightVec[i] = normalize(v);\n"

            //输出halfVec
            "        vec3 halfVector = normalize(vertexPosition + lightDir);\n"
            "        v.x = dot (halfVector, t);\n"
            "        v.y = dot (halfVector, b);\n"
            "        v.z = dot (halfVector, n);\n"
            "        halfVec[i] = v ; \n"
            "    }\n"

            //输出eyeVec
            //"    v.xyz = -v.xyz;\n"
            "    v.x = dot(vertexPosition, t);\n"
            "    v.y = dot(vertexPosition, b);\n"
            "    v.z = dot(vertexPosition, n);\n"
            "    eyeVec = normalize(v);\n"


            //计算反射
            "vec3 u = normalize( vec3(nb_ModelViewMatrix * pos) );\n"
            //"vec3 n = normalize( gl_NormalMatrix * gl_Normal );\n"
            "vec3 r = reflect( u, n );\n"
            "float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );\n"
            "texCoordVar2.s = r.x/m + 0.5;\n"
            "texCoordVar2.t = r.y/m + 0.5;\n"

            "}\n"
            "\n"
            "\0"
        );

    }

    //default_skinning_normalmap_vs
    nbBase::ref_ptr<nbSG::Shader> default_skinning_normalmap_vs = new nbSG::Shader();
    default_skinning_normalmap_vs->setUniformPath("|shared|maya_skinning_normalmap_vs");
    {
        default_skinning_normalmap_vs->setShaderType(nbSG::Shader::NB_VERTEX_SHADER);
        default_skinning_normalmap_vs->setString(nbSG::Shader::ShaderProfileType::SPT_GL_CORE_330, "aasdasdasdasdasdasdasd asdasd asd 123123 asdas");

        default_skinning_normalmap_vs->setString(nbSG::Shader::ShaderProfileType::SPT_GLES_2,

            //"#version 110\n"
            //"precision mediump float;\n"
            "#define MAX_LIGHTS 8\n"

#include "gles_inverse.inc"
#include "gles_transpose.inc"
#include "uniform_LightSource.inc"

            "uniform mat4 nb_ModelViewMatrix; \n"
            "uniform mat4 nb_ProjectionMatrix; \n"

            "attribute vec3 nfx_va_position;\n"
            "attribute vec2 nfx_va_uv0; \n"
            "attribute vec2 nfx_va_uv1; \n"
            "attribute vec3 nfx_va_normal; \n"
            "attribute vec3 nfx_va_tangent0; \n"

            //skin相关
            "#define MAX_BONES_PER_VERTEX 4\n"
            "#define MAX_BONES 32\n" //max 62 why
            "uniform int skinning;\n"
            "uniform mat4 boneMatrices[MAX_BONES];\n"

            "attribute vec4 nfx_va_bone_indices; \n"
            "attribute vec4 nfx_va_blend_weights; \n"

            //输出
            "varying vec2 texCoordVar0; \n" //diffuse
            "varying vec2 texCoordVar1; \n" //lightmap
            "varying vec2 texCoordVar2; \n" //reflect
            "varying vec3 lightVec[MAX_LIGHTS];\n"
            "varying vec3 halfVec[MAX_LIGHTS];\n"
            "varying vec3 eyeVec;\n"
            "varying vec3 tangent;\n"

            //调试
            "varying vec4 debug_color; \n"

            //skin函数
            "vec3 skin( vec3 v ,float w)\n"
            "{\n"
            "    vec3 ret = vec3(0,0,0);\n"
            "    float max_weight = 0.0;\n"
            //"    float indexf = 0.0;\n"
            //"    int i=0;\n"
            //"    for( i=0; i<MAX_BONES_PER_VERTEX; i++)\n"
            //"    {\n"
            //"        indexf = nfx_va_bone_indices.x;\n"
            "        int index0 = int(nfx_va_bone_indices.x);\n"
            "        int index1 = int(nfx_va_bone_indices.x);\n"
            "        int index2 = int(nfx_va_bone_indices.x);\n"
            "        int index3 = int(nfx_va_bone_indices.x);\n"

            //如果没有权重还是保持原样输出
            "        if( index0 < 0 && index1 < 0 && index2 < 0 && index3 < 0 )\n"
            "        {\n"
            "            return v;\n"
            "        }\n"

            //因为gles并不支持用[0]的形式访问xyzw数值 所以这里不能用for循环访问
            "        if( index0 > -1 )\n"
            "        {\n"
            "            ret += (boneMatrices[index0] * vec4(v,w)).xyz * nfx_va_blend_weights.x;\n"
            "            max_weight += nfx_va_blend_weights.x;\n"
            "        }\n"

            "        if( index1 > -1 )\n"
            "        {\n"
            "            ret += (boneMatrices[index1] * vec4(v,w)).xyz * nfx_va_blend_weights.y;\n"
            "            max_weight += nfx_va_blend_weights.y;\n"
            "        }\n"
            "        if( index2 > -1 )\n"
            "        {\n"
            "            ret += (boneMatrices[index2] * vec4(v,w)).xyz * nfx_va_blend_weights.z;\n"
            "            max_weight += nfx_va_blend_weights.z;\n"
            "        }\n"
            "        if( index3 > -1 )\n"
            "        {\n"
            "            ret += (boneMatrices[index3] * vec4(v,w)).xyz * nfx_va_blend_weights.w;\n"
            "            max_weight += nfx_va_blend_weights.w;\n"
            "        }\n"


#if 0
            "        if( index > -1 )\n"
            "        {\n"
            "            ret += (boneMatrices[index] * vec4(v,w)).xyz * nfx_va_blend_weights[i];\n"
            "            max_weight += nfx_va_blend_weights[i];\n"
            "        }\n"
#endif
            //"    }\n"
            "    return ret/max_weight;\n"
            //"    return ret;\n"
            "}\n"


            "void main()\n"
            "{\n"

            //"    gl_Position = nb_ProjectionMatrix * nb_ModelViewMatrix * vec4(nfx_va_position.xyz,1.0); \n"


            //计算出NormalMatrix
            "    mat4 NormalMatrix = transpose(inverse( nb_ModelViewMatrix ));\n"


            //输出UV
            "    texCoordVar0 = nfx_va_uv0;\n"
            "    texCoordVar1 = nfx_va_uv1;\n"


            //因为兼容性问题 直接从属性中获取是否存在skin变形需求
            //"    int s = int(nfx_va_bone_indices[0])+int(nfx_va_bone_indices[1])+int(nfx_va_bone_indices[2])+int(nfx_va_bone_indices[3]);\n" //for skinning
            //"    int s = int(nfx_va_blend_weights[0] + nfx_va_blend_weights[1] + nfx_va_blend_weights[2] + nfx_va_blend_weights[3]);\n" //for skinning
            "    int s = int(nfx_va_bone_indices[0])+1;\n" //for skinning

            //计算骨骼影响
            "        vec4 pos = vec4(nfx_va_position.xyz,1.0);\n"
            "        vec3 nor = normalize(nfx_va_normal.xyz);\n"
            "        vec3 tan = normalize(nfx_va_tangent0.xyz);\n"
            "       if( s > 0 ){\n" //因为兼容性问题  这里不能直接用uniform int skinning作为判断条件
            //"    if( int(nfx_va_bone_indices[0]) >= 0 ){\n"
            //"       for (i=0; i<1; ++i) { \n"
            "           pos = vec4(skin(nfx_va_position.xyz,1.0),1.0);\n"
            "           nor = normalize(skin(nfx_va_normal.xyz,0.0));\n"
            "           tan = normalize(skin(nfx_va_tangent0.xyz,0.0));\n"
            "       }\n"


            //
            "    gl_Position = nb_ProjectionMatrix * nb_ModelViewMatrix * pos; \n"


            //计算TNB
            "    vec3 n = normalize(mat3(NormalMatrix) * nor);\n"  //这里绝对不能强制升级vec3为vec4
            "    vec3 t = normalize(mat3(NormalMatrix) * tan);\n"
            "    vec3 b = cross(n, t);\n"

            //输出tangent
            "    tangent = t;\n"

            //输出lightVec
            "    vec3 v;\n"
            "    vec3 vertexPosition = vec3(nb_ModelViewMatrix * pos);\n"
            "    int i;\n" //for i
            "    for (i=0; i<1/*nfx_LightSourceNum*/; ++i)\n"
            "    {\n"
            //"        vec3 lightDir = normalize(nfx_LightSource[i].position.xyz - vertexPosition);\n"
            "        vec3 lightDir = normalize(vec3(0,0,0) - vertexPosition);\n"
            "        v.x = dot(lightDir, t);\n"
            "        v.y = dot(lightDir, b);\n"
            "        v.z = dot(lightDir, n);\n"
            "        lightVec[i] = normalize(v);\n"

            //输出halfVec
            "        vec3 halfVector = normalize(vertexPosition + lightDir);\n"
            "        v.x = dot (halfVector, t);\n"
            "        v.y = dot (halfVector, b);\n"
            "        v.z = dot (halfVector, n);\n"
            "        halfVec[i] = v ; \n"
            "    }\n"

            //输出eyeVec
            //"    v.xyz = -v.xyz;\n"
            "    v.x = dot(vertexPosition, t);\n"
            "    v.y = dot(vertexPosition, b);\n"
            "    v.z = dot(vertexPosition, n);\n"
            "    eyeVec = normalize(v);\n"


            //计算反射
            "vec3 u = normalize( vec3(nb_ModelViewMatrix * pos) );\n"
            //"vec3 n = normalize( gl_NormalMatrix * gl_Normal );\n"
            "vec3 r = reflect( u, n );\n"
            "float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );\n"
            "texCoordVar2.s = r.x/m + 0.5;\n"
            "texCoordVar2.t = r.y/m + 0.5;\n"


            //"    debug_color = vec4(eyeVec,1.0);\n"
            //"    debug_color = vec4(s,s,s,1.0);\n"
            //"    debug_color = vec4(1.0,1.0,1.0,1.0);\n"

            "}\n"
            "\n"
            "\0"
        );

    }
    //rmg->writeResource(default_normalmap_vs.get());


    //fs_normalmap
    nbBase::ref_ptr<nbSG::Shader> default_normalmap_fs = new nbSG::Shader();
    default_normalmap_fs->setUniformPath("|shared|maya_normalmap_fs");
    {
        default_normalmap_fs->setShaderType(nbSG::Shader::NB_FRAGMENT_SHADER);
        default_normalmap_fs->setString(nbSG::Shader::ShaderProfileType::SPT_GL_CORE_330, "aasdasdasdasdasdasdasd asdasd asd 123123 asdas");

        default_normalmap_fs->setString(nbSG::Shader::ShaderProfileType::SPT_GLES_2,
            "precision mediump float;\n"

            "#define MAX_LIGHTS 8\n"

#include "uniform_LightSource.inc"
#include "uniform_FrontMaterial.inc"

            //输入
            "varying vec3 lightVec[MAX_LIGHTS];\n"
            "varying vec3 halfVec[MAX_LIGHTS];\n"
            "varying vec3 eyeVec;\n"
            "varying vec3 tangent;\n"
            "varying vec2 texCoordVar0; \n"
            "varying vec2 texCoordVar1; \n"
            "varying vec2 texCoordVar2; \n" //reflect


            //贴图
            "uniform sampler2D nb_tex0;\n" //diffuse
            "uniform sampler2D nb_tex1;\n"
            "uniform sampler2D nb_tex2;\n" //normal
            "uniform sampler2D nb_tex3;\n"
            "uniform sampler2D nb_tex4;\n" //specular
            "uniform sampler2D nb_tex5;\n" //reflect


            "varying vec4 debug_color; \n"

            "void main()\n"
            "{\n"

            "    vec2 uv0 = vec2(texCoordVar0.x,1.0-texCoordVar0.y);\n"
            "    vec2 uv2 = vec2(texCoordVar2.x,1.0-texCoordVar2.y);\n"

            //获取法线
             //lookup normal from normal map, move from [0,1] to  [-1, 1] range, normalize
            "    vec3 normal = 2.0 * texture2D(nb_tex2, uv0).rgb - 1.0;\n"
            "    normal = normalize (normal);\n"



            //最终结果
            "    vec4 final_color = vec4(0.0,0.0,0.0,1.0);\n"
            "    int i;\n"
            "    for (i=0; i<1/*nfx_LightSourceNum*/; ++i)\n"
            "    {\n"

            //      compute diffuse lighting
            "       float lamberFactor = max (dot (lightVec[i], normal), 0.0);\n"

            //      compute ambient
            "       vec4 ambientLight =vec4(0.0,0.0,0.0,1.0);\n"
            //"       vec4 ambientLight = nfx_LightSource[i].ambient;\n"

            "       if (lamberFactor > 0.0)\n"
            "       {\n"
            "           vec4 diffuseMaterial = texture2D (nb_tex0, uv0);\n"
            //"           vec4 diffuseLight  = nfx_LightSource[i].diffuse;\n"
            "           vec4 diffuseLight =vec4(1.0,1.0,1.0,1.0);\n"

            "           vec4 specular = texture2D (nb_tex4, uv0);\n"

            //          In doom3, specular value comes from a texture 
            "           vec4 specularMaterial = specular;\n"
            //"           vec4 specularLight = nfx_LightSource[i].specular;\n"
            "           vec4 specularLight = vec4(1.0,1.0,1.0,1.0);\n"
            //"           float shininess = pow (max (dot (lightVec[i], normal), 0.0), nfx_FrontMaterial.shininess);\n"
            "           float shininess = pow (max (dot (lightVec[i], normal), 0.0), 30.0);\n"

            "           vec4 envMaterial = texture2D (nb_tex5, uv2);\n"

            "           final_color += diffuseMaterial * diffuseLight * lamberFactor;\n"
            //#if 0
            "           final_color += specularMaterial * (specularLight * shininess) * vec4(envMaterial.y,envMaterial.y,envMaterial.y,1.0);\n"

            //"           final_color = envMaterial;\n"
//#endif
"       }\n"

"       final_color += ambientLight;\n"

"    }\n"

//"    final_color = vec4(lightVec[0],1.0);\n"
//"    final_color = vec4(tangent,1.0);\n"
"gl_FragColor = final_color;\n"



//测试输出
//"           vec4 diffuse  = texture2D(nb_tex0, uv0);\n"
//"           vec4 specular = texture2D(nb_tex4, uv0);\n"
//"    gl_FragColor = texture2D(texture, texCoordVar);\n"
//"    gl_FragColor = vec4(1.0,0.0,1.0,1.0);\n"
//"    gl_FragColor = vec4(texCoordVar0.x,texCoordVar0.y,0.0,1.0);\n"
//"    gl_FragColor = debug_color;\n"
//"    gl_FragColor = vec4(normal,1.0);\n"
"}\n"
"\n"
"\0"
);
    }
    //rmg->writeResource(default_normalmap_fs.get());

    //maya_normalmap_program
    maya_normalmap_program = new nbSG::Program();
    maya_normalmap_program->setUniformPath("|shared|maya_normalmap");
    {
        maya_normalmap_program->setShader(default_normalmap_vs->getShaderType(), default_normalmap_vs.get());
        maya_normalmap_program->setShader(default_normalmap_fs->getShaderType(), default_normalmap_fs.get());
        maya_normalmap_program->mVANameMapping[NFX_VectexAttribute_position] = 0;
        maya_normalmap_program->mVANameMapping[NFX_VectexAttribute_uv0] = 1;
        maya_normalmap_program->mVANameMapping[NFX_VectexAttribute_uv1] = 2;
        maya_normalmap_program->mVANameMapping[NFX_VectexAttribute_normal] = 3;
        maya_normalmap_program->mVANameMapping[NFX_VectexAttribute_tangent0] = 4;
    }


    //maya_skinning_normalmap_program
    maya_skinning_normalmap_program = new nbSG::Program();
    maya_skinning_normalmap_program->setUniformPath("|shared|maya_skinning_normalmap");
    {
        maya_skinning_normalmap_program->setShader(default_skinning_normalmap_vs->getShaderType(), default_skinning_normalmap_vs.get());
        maya_skinning_normalmap_program->setShader(default_normalmap_fs->getShaderType(), default_normalmap_fs.get());
        maya_skinning_normalmap_program->mVANameMapping[NFX_VectexAttribute_position] = 0;
        maya_skinning_normalmap_program->mVANameMapping[NFX_VectexAttribute_uv0] = 1;
        maya_skinning_normalmap_program->mVANameMapping[NFX_VectexAttribute_uv1] = 2;
        maya_skinning_normalmap_program->mVANameMapping[NFX_VectexAttribute_normal] = 3;
        maya_skinning_normalmap_program->mVANameMapping[NFX_VectexAttribute_tangent0] = 4;
        maya_skinning_normalmap_program->mVANameMapping[NFX_VectexAttribute_bone_indices] = 5;
        maya_skinning_normalmap_program->mVANameMapping[NFX_VectexAttribute_blend_weights] = 6;
    }
    //rmg->writeResource(maya_normalmap_program.get());

}

#endif 

