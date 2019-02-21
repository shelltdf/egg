#ifndef NIUBI_SG_ACTORCONTROLER_H
#define NIUBI_SG_ACTORCONTROLER_H

#include <eggEngine/Component.h>

#include <eggEngine/Animation.h>


namespace egg
{
    class EGG_ENGINE_EXPORT ActorControler
        :public ComponentBase
    {
    public:

        ActorControler(){}
        virtual ~ActorControler(){}

        virtual std::string ObjectClassName() override {return "actorcontroler";} 

        //消息分拣
        virtual void traverse(NodeVisitor& nv) override
        {
            if (nv.as<nv_collectionResourceObject>())
            {
                nv_collectionResourceObject* cro = nv.as<nv_collectionResourceObject>();

                //Animation
                if(mAnimation.valid() && mAnimation->isLoaded())
                {
                    cro->result[ mAnimation->getPath() ] = mAnimation->getResource();
                }
            }
            else if (nv.as<nv_collectionResourceLink>())
            {
                nv_collectionResourceLink* crl = nv.as<nv_collectionResourceLink>();

                //Animation
                if(mAnimation.valid() && !mAnimation->isLoaded())
                {
                    crl->result.push_back( mAnimation.get() );
                }
            }
            ComponentBase::traverse(nv);
        }

        virtual void onUpdate(nv_update* nv) override;
        virtual void onCull(nv_cull* nv, Eigen::Matrix4f& vm) override {}
        virtual void onRender(nv_render* nv, Eigen::Matrix4f& mv) override{}
        virtual void onInput(nv_input* nv) override {}

        virtual void save( Path filename , std::ostream& os , Serializer* ser ) const override
        {
            ComponentBase::save(filename,os,ser);

            //Animation
            saveResourceLink<Animation>(  mAnimation.get() ,Path(filename),  os , ser );
        }

        virtual void load( Path filename , std::istream& is , Serializer* ser , ObjectManager* om, Referenced* rmg, bool load_ext) override
        {
            ComponentBase::load(filename,is,ser,om,rmg,load_ext);

            //Animation
            mAnimation = new ResourceLinkT< Animation >();
            loadResourceLink<Animation>( filename , mAnimation.get() ,  is , ser );
        }

        RESOURCE_LINK(Animation);

    protected:

        //启用解算结果
        //virtual void applyMatrix( std::string , vmml::mat4f mat );
    };
}//namespace egg

#endif//NIUBI_SG_ACTORCONTROLER_H
