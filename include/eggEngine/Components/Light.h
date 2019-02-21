#ifndef _EGG_Component_Light_H_
#define _EGG_Component_Light_H_

#include <eggEngine/Component.h>

namespace egg
{

    class EGG_ENGINE_EXPORT Light
        :public ComponentBase
    {
    public:
        Light() {}
        virtual ~Light() {}

        virtual std::string ObjectClassName() override { return "light"; }

        virtual void onUpdate(nv_update* nv) override {}
        virtual void onCull(nv_cull* nv, Eigen::Matrix4f& vm) override {}
        virtual void onRender(nv_render* nv, Eigen::Matrix4f& mv) override {}
        virtual void onInput(nv_input* nv) override {}

    };

}//namespace egg

#endif //_EGG_Component_Light_H_
