
#include <eggEngine/Components/ActorControler.h>

using namespace egg;



void ActorControler::onUpdate(nv_update* nv)
{
    //printf("a");
    float dt = 0.1f; //更新时间

    //当前动画时间
    static double time = 0;
    time += dt;

    //输入 从当前的animation中得到
    //输出到对应的名称对象中

    //动画控制器回调
    //nbSG::SkeletonActorControler* sac = nb_scene->getRoot()->getComponent<nbSG::SkeletonActorControler>();
    //if(!sac) return;

    ////获取joint角色空间矩阵(动画控制器所在节点的空间)
    ////   这个过程可以设置非常多的结算逻辑来实现复杂动画控制
    //std::vector<float> joint_matrix;
    //joint_matrix.resize(sac->mSkeletonMapping.size()*16);
    //float idm[16] = {1,0,0,0  ,0,1,0,0  ,0,0,1,0  ,0,0,0,1};
    //for (unsigned int i = 0; i < joint_matrix.size()/16; i++)
    //{
    //    memcpy(&joint_matrix[i*16],idm,sizeof(float)*16);//恢复单位矩阵
    //}

    ////计算骨骼节点的local矩阵
    //nbSG::Animation* anim = sac->getAnimation();
    //if(anim)
    //{
    //    //计算更新时间
    //    int fps = 25;  //假设播放动画的fps
    //    int frame_num = anim->getFrameNum(); //动画帧数
    //    float count = float(frame_num-1)/float(fps);//动画长度时间s
    //    float current = fmodf(time,count); //当前动画循环中的时间
    //    float current_frame = current*fps; //当前插值帧数
    //    //printf("    %f\n",current_frame);

    //    for (auto it = anim->getIndex().begin(); it != anim->getIndex().end(); it++)
    //    {
    //        std::string name = it->first;

    //        //求插值
    //        vmml::mat4f r = anim->interpolation( name , current_frame );

    //        //赋值local
    //        int id = nbSG::SkeletonActorControler::getSkeletonIndex(name,sac->mSkeletonMapping);
    //        if(id >= 0)
    //        {
    //            float* ra = r;
    //            memcpy(&joint_matrix[id*16],ra,sizeof(float)*16);
    //        }
    //    }
    //}

    ////计算world矩阵 记录在joint_matrix中
    //computeWorldMatrix( -1 , sac->mSkeletonMapping , joint_matrix);


    //applyMatrix( "" , vmml::mat4f mat );
}

#if 0
void ActorControler::applyMatrix( std::string , vmml::mat4f mat )
{


}
#endif
