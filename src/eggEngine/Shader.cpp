
#include <eggEngine/Shader.h>

//#include <eggEngine/Renderer.h>
//#include <eggEngine/GraphicsContext.h>
#include <eggEngine/RenderState.h>
#include <eggEngine/Renderer.h>
#include <eggEngine/Program.h>
#include <eggEngine/Pass.h>

using namespace egg;

#if 1
void Shader::bind(HANDLE_TYPE contextID, Renderer* renderer, Program* program)
{
    //printf("");
    //RenderState* rs = render->getRenderState();
    handle_program& p = program->getProgram().getID(contextID);

    //auto it = mString.find(SPT_GLES_2);
    //if (it != mString.end())
    {
        SHADER_TYPE type = ST_VS;
        if (mShaderType == NB_VERTEX_SHADER) type = ST_VS;
        if (mShaderType == NB_FRAGMENT_SHADER) type = ST_FS;

        //如果当前环境不存在显存对象 就建立一个
        if (mShader.getID(contextID) == 0)
        {
            handle_shader s = renderer->createShader(p, type, &mString[0]);
            mShader.setID(contextID, s);
        }

        //如果attach的目标不一样 才执行attach
        if (p.shader_id[type] != mShader.getID(contextID))
        {
            renderer->attachShader(p, type, mShader.getID(contextID));
        }

        //link 操作让program的dirty来完成
    }
}

void Shader::unBind()
{
}

#endif
