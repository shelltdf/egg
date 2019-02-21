
#include "Export.h"

#include <iostream>
#include <map>

#include <fbxsdk.h>

//#include <nbSG/Public.h>
#include <eggEngine/Components/SkeletonActorControler.h>


using namespace fbxsdk;



//搜索场景找到是否存在skin的变形器
//    顺便收集skeleton的映射关系信息
//    如果没有 说明根本不存在SkeletonActorControler的需求
void getSkeleonMapping(FbxNode* pNode, bool& has_skin_deformer
    , std::map< std::string, egg::JointMapping >& sm
    , int& this_id, int parent_id = -1)
{
    //如果父节点小于0 说明当前id还没有开始自增
    //if( parent_id < 0 ) this_id = -1;

    //查询joint属性
    if (pNode->GetSkeleton())
    {
        //计数器自增
        this_id++;

        printf(" -- joint : %s parent_id %d this_id %d \n", pNode->GetName(), parent_id, this_id);

        egg::JointMapping jm;
        jm.name = pNode->GetName();
        jm.parent_id = parent_id;
        jm.this_id = this_id;

        //重名检查
        auto it = sm.find(jm.name);
        if (it != sm.end())
        {
            printf("error: joint has same name : %s \n", jm.name.c_str());
        }

        //这里暂时不支持重名骨骼 如果有重名直接就覆盖了 从而导致错误
        sm[jm.name] = jm;

        //给子节点计算正确的id
        parent_id = this_id;
    }

    if (pNode->GetMesh())
    {
        int deformerCount = pNode->GetMesh()->GetDeformerCount();
        if (deformerCount > 0)  has_skin_deformer = true;
    }

    // Recursively children.
    for (int j = 0; j < pNode->GetChildCount(); j++)
    {
        getSkeleonMapping(pNode->GetChild(j), has_skin_deformer, sm, this_id, parent_id);
    }
}


//补充不存在的骨骼bindpose
void buildBindPose(FbxNode* pNode
    , std::map< FbxNode*, FbxMatrix >& bind
    , int& this_id, int parent_id = -1)
{
    //查询joint属性
    if (pNode->GetSkeleton())
    {
        //计数器自增
        this_id++;
        printf(" -- joint : %s parent_id %d this_id %d \n", pNode->GetName(), parent_id, this_id);

        auto it = bind.find(pNode);
        if (it != bind.end())
        {
        }
        else
        {
            //FbxMatrix m = pNode->EvaluateGlobalTransform(0);
            vmml::mat4f current_mesh_matrix = getWorldMatrix(pNode);
            float* vm = current_mesh_matrix;
            FbxMatrix m(
                vm[0], vm[1], vm[2], vm[3]
                , vm[4], vm[5], vm[6], vm[7]
                , vm[8], vm[9], vm[10], vm[11]
                , vm[12], vm[13], vm[14], vm[15]
            );
            bind[pNode] = m;
        }

        //给子节点计算正确的id
        parent_id = this_id;
    }

    // Recursively children.
    for (int j = 0; j < pNode->GetChildCount(); j++)
    {
        buildBindPose(pNode->GetChild(j), bind, this_id, parent_id);
    }
}

//收集骨骼的bindpose矩阵 世界矩阵
std::map< FbxNode*, FbxMatrix > getBindPose(FbxScene* scene)
{
    std::map< FbxNode*, FbxMatrix > ret;

    unsigned int num_pose = scene->GetPoseCount();
    for (unsigned int i = 0; i < num_pose; i++)
    {
        FbxPose* pose = scene->GetPose(i);

        //如果不是bindpose就跳过
        if (!pose->IsBindPose()) continue;

        //遍历当前pose内的全部node
        unsigned int num_node = pose->GetCount();
        for (unsigned int j = 0; j < num_node; j++)
        {
            FbxNode* node = pose->GetNode(j);

            //这里得到的都是bind的世界矩阵
            FbxMatrix m = pose->GetMatrix(j);
            ret[node] = m;

            printf(" -- bindpose %s\n", node->GetName());
        }
    }

    return ret;
}

//尝试搜索骨骼动画有关信息 如果存在就返回 SkeletonActorControler
egg::SkeletonActorControler* LoadSkeletonActorControler(FbxScene* scene)
{
    //判断是否存在skin的变形器 如果没有 就说明没需求
    std::map< std::string, egg::JointMapping > skeleton_mapping;
    bool has_skin_deferoer = false;
    int this_id = -1; //mast be -1
    getSkeleonMapping(scene->GetRootNode(), has_skin_deferoer, skeleton_mapping, this_id);
    if (has_skin_deferoer)
    {
        printf(" -- has skin deferoer\n");
    }
    else
    {
        printf(" -- has no skin deferoer\n");
        return nullptr;
    }

    //建立骨骼动画控制器
    egg::SkeletonActorControler* actor = new egg::SkeletonActorControler();
    actor->mSkeletonMapping = skeleton_mapping;

    //收集bindpose 世界矩阵
    std::map< FbxNode*, FbxMatrix > bindpose = getBindPose(scene);

    //如果找不到bind信息 可能是CS骨骼输出的缺失问题
    //    所以这里遍历全部骨骼 用第一帧当作bindpose了
    //    前提是CS动画的第一帧正好和bindpose重合
    this_id = -1; //mast be -1
    buildBindPose(scene->GetRootNode(), bindpose, this_id);


    //填充bind信息
    actor->mBindPoseMatrix.resize(skeleton_mapping.size());
    actor->mBindPoseMatrixInv.resize(skeleton_mapping.size());
    for (auto it = bindpose.begin(); it != bindpose.end(); it++)
    {
        if (it->first->GetSkeleton())
        {
            //当前矩阵
            double* m = it->second;
            float mf[16] = { 0 };
            for (int i = 0; i < 16; i++) { mf[i] = (float)m[i]; }

            //逆矩阵
            FbxMatrix inv = it->second.Inverse();
            double* m_inv = inv;
            float mf_inv[16] = { 0 };
            for (int i = 0; i < 16; i++) { mf_inv[i] = (float)m_inv[i]; }

            //查找映射的索引
            int ske_index = egg::SkeletonActorControler::getSkeletonIndex(it->first->GetName(), skeleton_mapping);
            if (ske_index < 0) continue;

            //记录bind矩阵和逆矩阵 世界坐标
            actor->mBindPoseMatrix[ske_index].set(&mf[0], &mf[15], false);
            actor->mBindPoseMatrixInv[ske_index].set(&mf_inv[0], &mf_inv[15], false);
        }
    }


    return actor;
}
