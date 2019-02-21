  
//√÷≤πƒ⁄÷√±‰¡ø
//      int  nfx_LightSourceNum 
//      vec4 nfx_LightSource[MAX_LIGHTS].position
//      vec4 nfx_LightSource[MAX_LIGHTS].ambient
//      vec4 nfx_LightSource[MAX_LIGHTS].diffuse
//      vec4 nfx_LightSource[MAX_LIGHTS].specular
//      vec4 nfx_LightModel.ambient


//"#define MAX_LIGHTS 8\n"

"uniform int nfx_LightSourceNum;\n"
"struct LightSource\n"
"{\n"
"   vec4 position;\n"
"   vec4 ambient;\n"
"   vec4 diffuse;\n"
"   vec4 specular;\n"
"};\n"
"uniform LightSource nfx_LightSource[MAX_LIGHTS];\n"

"struct nb_LightModelParameters\n"
"{\n"
"   vec4 ambient;\n"
"};\n"
"uniform nb_LightModelParameters nfx_LightModel;\n"

