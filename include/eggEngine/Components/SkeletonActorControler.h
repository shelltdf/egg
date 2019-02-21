#ifndef NIUBI_SG_SKELETONACTORCONTROLER_H
#define NIUBI_SG_SKELETONACTORCONTROLER_H

#include <eggEngine/Component.h>

#include <eggEngine/Components/ActorControler.h>

namespace egg
{

    //骨骼映射信息
    struct JointMapping
    {
        std::string name;
        int parent_id;
        unsigned int this_id;
    };

    class EGG_ENGINE_EXPORT SkeletonActorControler
        :public ActorControler
    {
    public:

        SkeletonActorControler()
            :current_time(0.0){}
        virtual ~SkeletonActorControler(){}

        virtual std::string ObjectClassName() override {return "skeletonactorcontroler";}

        //消息分拣
        virtual void traverse(NodeVisitor& nv) override
        {
            if (nv.as<nv_collectionResourceObject>())
            {
                nv_collectionResourceObject* cro = nv.as<nv_collectionResourceObject>();
            }
            else if (nv.as<nv_collectionResourceLink>())
            {
                nv_collectionResourceLink* crl = nv.as<nv_collectionResourceLink>();
            }

            ActorControler::traverse(nv);
        }

        virtual void onUpdate(nv_update* nv) override ;
        //{
        //    ActorControler::onUpdate(nv);
        //}
        virtual void onCull(nv_cull* nv, Eigen::Matrix4f& vm) override {}
        virtual void onRender(nv_render* nv, Eigen::Matrix4f& mv) override {}

        virtual void save( Path filename , std::ostream& os , Serializer* ser ) const override
        {
            ActorControler::save(filename,os,ser);

            //写入骨骼映射
            ser->writeNumber( os , mSkeletonMapping.size() ); //通道数
            for (auto it = mSkeletonMapping.begin(); it != mSkeletonMapping.end(); it ++)
            {
                JointMapping jm = it->second;
                ser->writeString( os , jm.name );
                ser->writeNumber( os , jm.parent_id );
                ser->writeNumber( os , jm.this_id );
            }


            //bindpose
            ser->writeNumber( os , mBindPoseMatrix.size() ); //通道数
            for (auto it = mBindPoseMatrix.begin(); it != mBindPoseMatrix.end(); it ++)
            {
                const float* m = (*it).data();
                for (int i = 0; i < 16; i++){ ser->writeNumber( os , m[i] ); } 
            }

            //bindpose inv
            ser->writeNumber( os , mBindPoseMatrixInv.size() ); //通道数
            for (auto it = mBindPoseMatrixInv.begin(); it != mBindPoseMatrixInv.end(); it ++)
            {
                const float* m = (*it).data();
                for (int i = 0; i < 16; i++){ ser->writeNumber( os , m[i] ); } 
            }
        }

        virtual void load( Path filename , std::istream& is , Serializer* ser , ObjectManager* om, Referenced* rmg, bool load_ext) override
        {
            ActorControler::load(filename,is,ser,om,rmg,load_ext);

            //读骨骼映射
            int jm_count = 0;
            ser->readNumber( is , jm_count ); //通道数
            for (int i = 0; i < jm_count; i++)
            {
                JointMapping jm;
                ser->readString( is , jm.name );
                ser->readNumber( is , jm.parent_id );
                ser->readNumber( is , jm.this_id );
                mSkeletonMapping[jm.name] = jm;
            }

            //bindpose
            int bind_count = 0;
            ser->readNumber( is , bind_count ); //通道数
            for (int i = 0; i < bind_count; i++)
            {
                float m[16];
                for (int i = 0; i < 16; i++){ ser->readNumber( is , m[i] ); }
                //vmml::mat4f v;
                //v.set(&m[0],&m[15],false);
                Eigen::Matrix4f v(m);
                mBindPoseMatrix.push_back(v);
            }

            //bindpose inv
            int bind_inv_count = 0;
            ser->readNumber( is , bind_inv_count ); //通道数
            for (int i = 0; i < bind_inv_count; i++)
            {
                float m[16];
                for (int i = 0; i < 16; i++){ ser->readNumber( is , m[i] ); }
                //vmml::mat4f v;
                //v.set(&m[0],&m[15],false);
                Eigen::Matrix4f v(m);
                mBindPoseMatrixInv.push_back(v);
            }
        }

        //
        static int getSkeletonIndex( const std::string& name , const std::map< std::string , JointMapping >& joint_map )
        {
            for (auto it_ske = joint_map.begin(); it_ske != joint_map.end(); it_ske++)
            {
                if( it_ske->first == name )
                {
                    return it_ske->second.this_id;
                }
            }
            return -1;
        }
        static int getSkeletonParent( const std::string& name , const std::map< std::string , JointMapping >& joint_map )
        {
            for (auto it_ske = joint_map.begin(); it_ske != joint_map.end(); it_ske++)
            {
                if( it_ske->first == name )
                {
                    return it_ske->second.parent_id;
                }
            }
            return -1;
        }
        static int getSkeletonParent( unsigned int index , const std::map< std::string , JointMapping >& joint_map )
        {
            for (auto it_ske = joint_map.begin(); it_ske != joint_map.end(); it_ske++)
            {
                if( it_ske->second.this_id == index )
                {
                    return it_ske->second.parent_id;
                }
            }
            return -1;
        }
        static std::string getSkeletonName( unsigned int index , const std::map< std::string , JointMapping >& joint_map )
        {
            for (auto it_ske = joint_map.begin(); it_ske != joint_map.end(); it_ske++)
            {
                if( it_ske->second.this_id == index )
                {
                    return it_ske->first;
                }
            }
            return "";
        }
        static std::vector<int> getSkeletonChild( unsigned int index , const std::map< std::string , JointMapping >& joint_map )
        {
            std::vector<int> ret;
            for (auto it_ske = joint_map.begin(); it_ske != joint_map.end(); it_ske++)
            {
                if( it_ske->second.parent_id == index )
                {
                    ret.push_back(it_ske->second.this_id);
                }
            }
            return ret;
        }



        //骨骼名称对应ID表
        //   ID是从0开始顺序的表
        std::map< std::string , JointMapping > mSkeletonMapping;

        //bindpose 矩阵
        std::vector< Eigen::Matrix4f > mBindPoseMatrix;

        //bindpose 逆矩阵
        std::vector< Eigen::Matrix4f > mBindPoseMatrixInv;

    private:

        //递归计算当前名称的节点的世界矩阵
        void computeWorldMatrix(int index ,const std::map< std::string , JointMapping >& sm , std::vector<float>& joint_matrix);


        //当前动画起点时间
        float current_time;
    };
}//namespace nbSG

#endif//NIUBI_SG_SKELETONACTORCONTROLER_H
