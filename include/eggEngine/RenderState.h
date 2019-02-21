#ifndef _EGG_RenderState_H_
#define _EGG_RenderState_H_

#include <eggEngine/Export.h>

#include <vector>
#include <string>
#include <map>
#include <stack>

#include <eggRuntime/Object.h>
//#include <eggEngine/Pass.h>


namespace egg
{

    class GraphicsContext;
    class Renderer;
    class Pass;

    //渲染状态对象
    class EGG_ENGINE_EXPORT RenderState
        :public Object
    {
    public:

        RenderState() {}
        virtual ~RenderState() {}

        //矩阵不需要堆栈操作
        void setModelViewMatrix(float* m44) { memcpy(&modelViewMatrix[0], &m44[0], 16 * sizeof(float)); }
        void setProjectionMatrix(float* m44) { memcpy(&projectionMatrix[0], &m44[0], 16 * sizeof(float)); }

        //使用pass来调整当前渲染状态
        egg::Pass* top();
        //void set(egg::Material::Pass* pass);
        void push(egg::Pass* pass);
        void pop();


        //绑定当前渲染状态
        virtual void bindRenderPass(GraphicsContext* gc, Renderer* renderer) = 0;
        virtual void unbindRenderPass() = 0;

        //仅仅更新空间位置
        virtual void applyMatrix(egg::GraphicsContext* gc, egg::Renderer* renderer) = 0;

    protected:

        //matrix
        float modelViewMatrix[16];
        float projectionMatrix[16];

    private:
        //
        std::stack< ref_ptr<egg::Pass> > mStack;

    };


}//namespace egg


#endif // _EGG_RenderState_H_
