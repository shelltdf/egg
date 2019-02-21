
#include <eggEngine/RenderState.h>
#include <eggEngine/Pass.h>

using namespace egg;


egg::Pass* RenderState::top()
{
    if (mStack.size() > 0)
    {
        return mStack.top().get();
    }
    return 0;
}

//void RenderState::set(egg::Material::Pass* pass) 
//{
//    mStack.
//}

void RenderState::push(egg::Pass* pass)
{
    mStack.push(pass);
}

void RenderState::pop()
{
    mStack.pop();
}


