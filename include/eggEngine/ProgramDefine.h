#ifndef NIUBI_SG_PROGRAM_DEFINE_H
#define NIUBI_SG_PROGRAM_DEFINE_H

//
//VectexAttribute 保留名称
//   从效率上考虑每个VA都应该有一个固定的location
//   这样可以避免program无意义的重复link
//   从数据记录的层面上考虑，buffer的内容在内存中是无所谓顺序的，draw的时候才确定location。
//

#define egg_va_position "egg_va_position"      //位置        float4  (但是除了dx以外 GL貌似只支持float3)
#define egg_va_position2 "egg_va_position2"    //位置        float2
#define egg_va_normal "egg_va_normal"          //法线        float3

#define egg_va_tangent0 "egg_va_tangent0"      //切线0       float3
#define egg_va_tangent1 "egg_va_tangent1"      //切线1       float3
#define egg_va_tangent2 "egg_va_tangent2"      //切线2       float3
#define egg_va_tangent3 "egg_va_tangent3"      //切线3       float3

// #define NFX_VectexAttribute_binormal "nfx_va_binormal"    //二法线

#define egg_va_color0 "egg_va_color0"          //顶点色0     float4
#define egg_va_color1 "egg_va_color1"          //顶点色1     float4
#define egg_va_color2 "egg_va_color2"          //顶点色2     float4
#define egg_va_color3 "egg_va_color3"          //顶点色3     float4

#define egg_va_uv0 "egg_va_uv0"                //UV0         float2
#define egg_va_uv1 "egg_va_uv1"                //UV1         float2
#define egg_va_uv2 "egg_va_uv2"                //UV2         float2
#define egg_va_uv3 "egg_va_uv3"                //UV3         float2

#define egg_va_bone_indices  "egg_va_bone_indices"    //Bone Indices         int4
#define egg_va_blend_weights "egg_va_blend_weights"   //Blend Weights        float4

static int getVALocation(std::string va_name)
{
    if (va_name == egg_va_position) return 0;
    if (va_name == egg_va_normal) return 1;
    if (va_name == egg_va_tangent0) return 2;
    if (va_name == egg_va_tangent1) return 3;
    if (va_name == egg_va_tangent2) return 4;
    if (va_name == egg_va_tangent3) return 5;
    if (va_name == egg_va_color0) return 6;
    if (va_name == egg_va_color1) return 7;
    if (va_name == egg_va_color2) return 8;
    if (va_name == egg_va_color3) return 9;
    if (va_name == egg_va_uv0) return 10;
    if (va_name == egg_va_uv1) return 11;
    if (va_name == egg_va_uv2) return 12;
    if (va_name == egg_va_uv3) return 13;
    if (va_name == egg_va_bone_indices) return 14;
    if (va_name == egg_va_blend_weights) return 15;
    return -1;
}


//
//Uniform 内置常量
//build-in constants 1.1
//
#if 0
#define NFX_gl_MaxLights "gl_MaxLights" //int
#define NFX_gl_MaxClipPlanes "gl_MaxClipPlanes" //int
#define NFX_gl_MaxTextureUnits "gl_MaxTextureUnits" //int
#define NFX_gl_MaxTextureCoords "gl_MaxTextureCoords" //int
#define NFX_gl_MaxVertexAttribs "gl_MaxVertexAttribs" //int
#define NFX_gl_MaxVertexUniformComponents "gl_MaxVertexUniformComponents" //int
#define NFX_gl_MaxVaryingFloats "gl_MaxVaryingFloats" //int
#define NFX_gl_MaxVertexTextureImageUnits "gl_MaxVertexTextureImageUnits" //int
#define NFX_gl_MaxCombinedTextureImageUnits "gl_MaxCombinedTextureImageUnits" //int
#define NFX_gl_MaxTextureImageUnits "gl_MaxTextureImageUnits" //int
#define NFX_gl_MaxFragmentUniformComponents "gl_MaxFragmentUniformComponents" //int
#define NFX_gl_MaxDrawBuffers "gl_MaxDrawBuffers" //int
#endif



//
//uniform 内置状态
//built-in uniform state 1.1
//

#define egg_ModelViewMatrix "egg_ModelViewMatrix" //mat4
#define egg_ProjectionMatrix "egg_ProjectionMatrix" //mat4
#define egg_ViewPort "egg_ViewPort" //vec4
#if 0
#define NFX_gl_ModelViewProjectionMatrix "gl_ModelViewProjectionMatrix" //mat4
#define NFX_gl_TextureMatrix "gl_TextureMatrix" //mat4 []
#define NFX_gl_NormalMatrix "gl_NormalMatrix" //mat3
#define NFX_gl_ModelViewMatrixInverse "gl_ModelViewMatrixInverse" //mat4
#define NFX_gl_ProjectionMatrixInverse "gl_ProjectionMatrixInverse" //mat4
#define NFX_gl_ModelViewProjectionMatrixInverse "gl_ModelViewProjectionMatrixInverse" //mat4
#define NFX_gl_TextureMatrixInverse "gl_TextureMatrixInverse" //mat4 []
#define NFX_gl_ModelViewMatrixTranspose "gl_ModelViewMatrixTranspose" //mat4
#define NFX_gl_ProjectionMatrixTranspose "gl_ProjectionMatrixTranspose" //mat4
#define NFX_gl_ModelViewProjectionMatrixTranspose "gl_ModelViewProjectionMatrixTranspose" //mat4
#define NFX_gl_TextureMatrixTranspose "gl_TextureMatrixTranspose" //mat4 []
#define NFX_gl_ModelViewMatrixInverseTranspose "gl_ModelViewMatrixInverseTranspose" //mat4
#define NFX_gl_ProjectionMatrixInverseTranspose "gl_ProjectionMatrixInverseTranspose" //mat4
#define NFX_gl_ModelViewProjectionMatrixInverseTranspose "gl_ModelViewProjectionMatrixInverseTranspose" //mat4
#define NFX_gl_TextureMatrixInverseTranspose "gl_TextureMatrixInverseTranspose" //mat4 []
#define NFX_gl_NormalScale "gl_NormalScale" //float
#define NFX_gl_DepthRange_near "gl_DepthRange.near" //float
#define NFX_gl_DepthRange_far "gl_DepthRange.far" //float
#define NFX_gl_DepthRange_diff "gl_DepthRange.diff" //float
#define NFX_gl_ClipPlane "gl_ClipPlane" //vec4 []
#define NFX_gl_Point_size "gl_Point.size" //float
#define NFX_gl_Point_sizeMin "gl_Point.sizeMin" //float
#define NFX_gl_Point_sizeMax "gl_Point.sizeMax" //float
#define NFX_gl_Point_fadeThresholdSize "gl_Point.fadeThresholdSize" //float
#define NFX_gl_Point_distanceConstanAttenuation "gl_Point.distanceConstanAttenuation" //float
#define NFX_gl_Point_distanceLinearAttenuation "gl_Point.distanceLinearAttenuation" //float
#define NFX_gl_Point_distanceQuadraticAttenuation "gl_Point.distanceQuadraticAttenuation" //float
#endif
#define egg_FrontMaterial_emission "egg_FrontMaterial.emission" //vec4
#define egg_FrontMaterial_ambient "egg_FrontMaterial.ambient" //vec4
#define egg_FrontMaterial_diffuse "egg_FrontMaterial.diffuse" //vec4
#define egg_FrontMaterial_specular "egg_FrontMaterial.specular" //vec4
#define egg_FrontMaterial_shininess "egg_FrontMaterial.shininess" //float
#if 0
#define NFX_gl_BackMaterial_emission "gl_BackMaterial.emission" //vec4
#define NFX_gl_BackMaterial_ambient "gl_BackMaterial.ambient" //vec4
#define NFX_gl_BackMaterial_diffuse "gl_BackMaterial.diffuse" //vec4
#define NFX_gl_BackMaterial_specular "gl_BackMaterial.specular" //vec4
#define NFX_gl_BackMaterial_shininess "gl_BackMaterial.shininess" //float
#define NFX_gl_LightSource_ambient "gl_LightSource[0].ambient" //vec4
#define NFX_gl_LightSource_diffuse "gl_LightSource.diffuse" //vec4
#define NFX_gl_LightSource_specular "gl_LightSource.specular" //vec4
#define NFX_gl_LightSource_postion "gl_LightSource.position" //vec4
#define NFX_gl_LightSource_halfVector "gl_LightSource.halfVector" //vec4
#define NFX_gl_LightSource_spotDirection "gl_LightSource.spotDirection" //vec3
#define NFX_gl_LightSource_spotExponect "gl_LightSource.spotExponect" //float
#define NFX_gl_LightSource_spotCutoff "gl_LightSource.spotCutoff" //float
#define NFX_gl_LightSource_spotCosCutoff "gl_LightSource.spotCosCutoff" //float
#define NFX_gl_LightSource_constanAttenuation "gl_LightSource.constanAttenuation" //float
#define NFX_gl_LightSource_linearAttenuation "gl_LightSource.linearAttenuation" //float
#define NFX_gl_LightSource_quadraticAttenuation "gl_LightSource.quadraticAttenuation" //float
#define NFX_gl_LightModel_ambient "gl_LightModel.ambient" //vec4
#define NFX_gl_FrontLightModeProduct_sceneColor "gl_FrontLightModeProduct.sceneColor" //vec4
#define NFX_gl_BackLightModeProduct_ambient "gl_BackLightModeProduct.ambient" //vec4
#define NFX_gl_FrontLightProduct_ambient "gl_FrontLightProduct.ambient" //vec4
#define NFX_gl_FrontLightProduct_diffuse "gl_FrontLightProduct.diffuse" //vec4
#define NFX_gl_FrontLightProduct_specular "gl_FrontLightProduct.specular" //vec4
#define NFX_gl_BackLightProduct_ambient "gl_BackLightProduct.ambient" //vec4
#define NFX_gl_BackLightProduct_diffuse "gl_BackLightProduct.diffuse" //vec4
#define NFX_gl_BackLightProduct_specular "gl_BackLightProduct.specular" //vec4
#define NFX_gl_TextureEnvColor "gl_TextureEnvColor" //vec4 []
#define NFX_gl_EyePlaneS "gl_EyePlaneS" //vec4 []
#define NFX_gl_EyePlaneT "gl_EyePlaneT" //vec4 []
#define NFX_gl_EyePlaneR "gl_EyePlaneR" //vec4 []
#define NFX_gl_EyePlaneQ "gl_EyePlaneQ" //vec4 []
#define NFX_gl_ObjectEyePlaneS "gl_ObjectEyePlaneS" //vec4 []
#define NFX_gl_ObjectEyePlaneT "gl_ObjectEyePlaneT" //vec4 []
#define NFX_gl_ObjectEyePlaneR "gl_ObjectEyePlaneR" //vec4 []
#define NFX_gl_ObjectEyePlaneQ "gl_ObjectEyePlaneQ" //vec4 []
#define NFX_gl_Fog_color "gl_Fog.color" //vec4
#define NFX_gl_Fog_density "gl_Fog.density" //float
#define NFX_gl_Fog_start "gl_Fog.start" //float
#define NFX_gl_Fog_end "gl_Fog.end" //float
#define NFX_gl_Fog_scale "gl_Fog.scale" //float
#endif


//补充定义
#if 1
#define egg_FrontMaterial_opacity "egg_FrontMaterial.opacity" //float
#define egg_FrontMaterial_reflectivity "egg_FrontMaterial.reflectivity" //float
#endif

#define egg_texture0 "egg_texture0"           //int
#define egg_texture1 "egg_texture1"
#define egg_texture2 "egg_texture2"
#define egg_texture3 "egg_texture3"
#define egg_texture4 "egg_texture4"
#define egg_texture5 "egg_texture5"
#define egg_texture6 "egg_texture6"
#define egg_texture7 "egg_texture7"

#define egg_texture0_enable "egg_texture0_enable"   //int 0 disable 1 enable
#define egg_texture1_enable "egg_texture1_enable"   //int 0 disable 1 enable
#define egg_texture2_enable "egg_texture2_enable"   //int 0 disable 1 enable
#define egg_texture3_enable "egg_texture3_enable"   //int 0 disable 1 enable
#define egg_texture4_enable "egg_texture4_enable"   //int 0 disable 1 enable
#define egg_texture5_enable "egg_texture5_enable"   //int 0 disable 1 enable
#define egg_texture6_enable "egg_texture6_enable"   //int 0 disable 1 enable
#define egg_texture7_enable "egg_texture7_enable"   //int 0 disable 1 enable



//补充定义 状态开关
#if 0
#define NB_AlphaTest "nb_AlphaTest"     //bool
#define NB_Blend "nb_Blend"             //bool
#define NB_DepthTest "nb_DepthTest"     //bool
#define NB_CullFace "nb_CullFace"       //bool
#define NB_Dither "nb_Dither"           //bool
#define NB_ColorMaterial "nb_ColorMaterial" //bool
#define NB_Lighting "nb_Lighting"       //bool
#endif




//
// VectexAttribute 对应语义
//
#if 0
#define NFX_Semantics_position "maya.position"          //位置
#define NFX_Semantics_normal "maya.normal"              //法线
//#define NFX_Semantics_componentId "maya.componentId"    //作用不明
#define NFX_Semantics_tangent "maya.tangent"            //切线    后面加.map1 为uvset的名称
//#define NFX_Semantics_binormal "maya.binormal"          //二法线  后面加.map1 为uvset的名称
#define NFX_Semantics_texCoord "maya.uv"                //UV      后面加.map1 为uvset的名称
#define NFX_Semantics_color "maya.color"                //顶点色  后面加.map1 为uvset的名称
#endif



//
// uniform 对应语义
//
#if 0
#define NFX_Semantics_FrameID "maya.FrameID"   //int
#define NFX_Semantics_Time "maya.Time"         //float second
#define NFX_Semantics_ModelViewMatrix "maya.ModelViewMatrix"    //mat4
#define NFX_Semantics_ProjectionMatrix "maya.ProjectionMatrix"  //mat4
#define NFX_Semantics_LightSource_Num "maya.LightSourceNum"     //int
#define NFX_Semantics_LightSource_postion "maya.LightSource[0].position"    //vec4
#define NFX_Semantics_LightSource_ambient "maya.LightSource[0].ambient"    //vec4
#define NFX_Semantics_LightSource_diffuse "maya.LightSource[0].diffuse"    //vec4
#define NFX_Semantics_LightSource_specular "maya.LightSource[0].specular"  //vec4
#endif


#endif//NIUBI_SG_PROGRAM_DEFINE_H
