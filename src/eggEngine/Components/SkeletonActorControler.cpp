#include <eggEngine/Components/SkeletonActorControler.h>

#include <eggEngine/Material.h>
#include <eggEngine/SceneNode.h>
#include <eggEngine/Components/MeshRender.h>

using namespace egg;

class nv_find_material
    :public NodeVisitor
{
public:
    nv_find_material() {}
    virtual ~nv_find_material() {}
    virtual void apply(Node& node) override
    {
        SceneNode* sn = dynamic_cast<SceneNode*>(&node);
        if (sn)
        {
            MeshRender* meshrender = sn->getComponent<MeshRender>();
            if (meshrender)
            {
                for (unsigned int i = 0; i < meshrender->getNumMaterial(); i++)
                {
                    Material* m = meshrender->getMaterial(i);
                    if (m)
                    {
                        mMaterialList.insert(m);
                    }
                }

            }
        }
        traverse(node);
    }

    std::set< ref_ptr< Material > > mMaterialList;
};



void SkeletonActorControler::computeWorldMatrix(int index, const std::map< std::string, JointMapping >& sm, std::vector<float>& joint_matrix)
{
    //计算自己的矩阵
    if (index < int(joint_matrix.size() / 16) && index >= 0)
    {
        int pid = SkeletonActorControler::getSkeletonParent(index, sm);
        if (pid >= 0)
        {
            //父矩阵
            //vmml::mat4f pm;
            //pm.set(&joint_matrix[pid * 16], &joint_matrix[pid * 16 + 15], false);
            Eigen::Matrix4f pm(&joint_matrix[pid * 16]);

            //当前local矩阵
            //vmml::mat4f lm;
            //lm.set(&joint_matrix[index * 16], &joint_matrix[index * 16 + 15], false);
            Eigen::Matrix4f lm(&joint_matrix[index * 16]);

            //覆盖当前矩阵为world
            Eigen::Matrix4f gm = pm * lm;
            float* ga = gm.data();
            memcpy(&joint_matrix[index * 16], ga, sizeof(float) * 16);
        }
    }

    //遍历子节点
    std::vector<int> ch = SkeletonActorControler::getSkeletonChild(index, sm);
    for (unsigned int i = 0; i < ch.size(); i++)
    {
        computeWorldMatrix(ch[i], sm, joint_matrix);
    }
}

void SkeletonActorControler::onUpdate(nv_update* nv)
{
    float dt = nv->dt;//0.01; //更新时间

    //当前动画时间
    //if( start_time<0 ) start_time = dt;
    current_time += dt;

    //printf("dt = %f time = %f\n",dt,current_time);


    //输入 从当前的animation中得到
    //输出到对应的名称对象中

    //动画控制器回调
    //nbSG::SkeletonActorControler* sac = nb_scene->getRoot()->getComponent<nbSG::SkeletonActorControler>();
    //if(!sac) return;

    //获取joint角色空间矩阵(动画控制器所在节点的空间)
    //   这个过程可以设置非常多的结算逻辑来实现复杂动画控制
    std::vector<float> joint_matrix;
    joint_matrix.resize(this->mSkeletonMapping.size() * 16);
    float idm[16] = { 1,0,0,0  ,0,1,0,0  ,0,0,1,0  ,0,0,0,1 };
    for (unsigned int i = 0; i < joint_matrix.size() / 16; i++)
    {
        memcpy(&joint_matrix[i * 16], idm, sizeof(float) * 16);//恢复单位矩阵
    }

    //计算骨骼节点的local矩阵
    Animation* anim = this->getAnimation();
    if (anim)
    {
        //计算更新时间
        int fps = 25;  //假设播放动画的fps
        int frame_num = anim->getFrameNum(); //动画帧数
        float count = float(frame_num - 1) / float(fps);//动画长度时间s
        float current = fmodf(current_time, count); //当前动画循环中的时间
        float current_frame = current*fps; //当前插值帧数
                                           //printf("    %f\n",current_frame);

        for (auto it = anim->getIndex().begin(); it != anim->getIndex().end(); it++)
        {
            std::string name = it->first;

            //求插值
            Eigen::Matrix4f r = anim->interpolation(name, current_frame);

            //赋值local
            int id = SkeletonActorControler::getSkeletonIndex(name, this->mSkeletonMapping);
            if (id >= 0)
            {
                float* ra = r.data();
                memcpy(&joint_matrix[id * 16], ra, sizeof(float) * 16);
            }
        }
    }

    //计算world矩阵 记录在joint_matrix中
    computeWorldMatrix(-1, this->mSkeletonMapping, joint_matrix);


    //搜索子节点上的全部material对象
    ref_ptr<nv_find_material> find_material = new nv_find_material();
    Node* parent = this->getNode();
    if (parent)
    {
        //搜索其下的全部材质
        parent->accept(*find_material);

        //计算骨骼矩阵
        for (unsigned int i = 0; i < mSkeletonMapping.size(); i++)
        {
            //更新骨骼的uniform  就是bindpose inv matrix * bone world matrix
            //osg::Matrix bone(&joint_matrix[i*16]);
            //osg::Matrix inv  = vmml2osg(sac->mBindPoseMatrixInv[i]);
            //u->setElement(i,inv * bone);

            //vmml::mat4f bone;
            //bone.set(&joint_matrix[i * 16], &joint_matrix[i * 16 + 15], false);
            Eigen::Matrix4f bone(&joint_matrix[i * 16]);
            bone = bone * mBindPoseMatrixInv[i];

            float* b = bone.data();
            memcpy(&joint_matrix[i * 16], b, sizeof(float) * 16);
        }

        //循环全部材质
        for (auto it = find_material->mMaterialList.begin(); it != find_material->mMaterialList.end(); it++)
        {
            Pass* pass = (*it)->getTech((*it)->getCurrentTech())->mPasses[0].get();

            //骨骼矩阵
            pass->setUniform("boneMatrices", UniformBase::UniformDataType::UDT_MAT4, &joint_matrix[0], joint_matrix.size(), "");
        }
    }//if(parent)

    return;
}


