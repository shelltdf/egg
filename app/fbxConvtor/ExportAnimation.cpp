
#include "Export.h"

#include <iostream>
#include <fstream>

#include <fbxsdk.h>

//#include <nbSG/Public.h>
#include <eggEngine/Animation.h>


using namespace fbxsdk;

//递归填充动画信息
//    要求动画节点不能重名
//    只能输出帧信息 从指定的帧数区间 输出到0开始的索引
void getAnimation(FbxNode* pNode, FbxTime start_time, FbxTime end_time, egg::Animation* anim, egg::Animation* anim_global)
{
    //名字
    std::string name = pNode->GetName();

    //判断node的动画信息是否有效
    FbxAnimCurveNode* acn0 = pNode->LclTranslation.GetCurveNode();
    FbxAnimCurveNode* acn1 = pNode->LclRotation.GetCurveNode();
    FbxAnimCurveNode* acn2 = pNode->LclScaling.GetCurveNode();
    FbxAnimCurveNode* acn3 = pNode->PreRotation.GetCurveNode();
    if (acn0 || acn1 || acn2 || acn3)
    {
        //给每个名字对应一个float16的数组 表示local矩阵

        //起始位置和步长计算
        double s = start_time.GetSecondDouble();
        double e = end_time.GetSecondDouble();
        FbxLongLong sn = start_time.GetFrameCount();
        FbxLongLong en = end_time.GetFrameCount();
        double step = (end_time - start_time).GetSecondDouble() / (en - sn);

        //增加一个新对象通道
        int size = anim->getIndex().size();
        anim->getIndex()[name] = size;
        anim->getData().resize(size + 1);
        anim->getData()[size].resize((en - sn + 1) * 16);

        size = anim_global->getIndex().size();
        anim_global->getIndex()[name] = size;
        anim_global->getData().resize(size + 1);
        anim_global->getData()[size].resize((en - sn + 1) * 16);


        //printf(" ");
        printf("anim %d %s ", size, name.c_str());
        for (FbxLongLong i = 0; i <= en - sn; i++)
        {
            printf(" %lld", (sn + i));

            //计算当前帧的时间
            FbxTime keyTimer;
            keyTimer.SetSecondDouble(s + i*step);
            FbxString str = keyTimer.GetTimeString();

            //评估矩阵
            FbxAMatrix curveKeyLocalMatrix = pNode->EvaluateLocalTransform(keyTimer);
            FbxAMatrix curveKeyGlobalMatrix = pNode->EvaluateGlobalTransform(keyTimer);
            double* lm = curveKeyLocalMatrix;
            double* gm = curveKeyGlobalMatrix;

            //填充结果
            for (int m = 0; m < 16; m++)
            {
                anim->getData()[size][i * 16 + m] = lm[m];
                anim_global->getData()[size][i * 16 + m] = gm[m];
            }

            //printf("  a %s\n",name.c_str() );

        }

        printf("\n");
    }

    // Recursively children.
    for (int j = 0; j < pNode->GetChildCount(); j++)
    {
        getAnimation(pNode->GetChild(j), start_time, end_time, anim, anim_global);
    }
}


void exportDefaultAnimationCode(egg::Animation* anim)
{
    std::ofstream ofs_pose("pose.inc");
    std::ofstream ofs_anim("anim.inc");

    ofs_pose << "#define pose_channal " << anim->getIndex().size() << "\n";
    ofs_pose << "#define pose_frame " << 1 << "\n";
    ofs_pose << "struct pose{std::string name;float data[pose_frame*16];};\n";
    ofs_pose << "pose _pose[pose_channal] = {\n";

    ofs_anim << "#define anim_channal " << anim->getIndex().size() << "\n";
    ofs_anim << "#define anim_frame " << anim->getFrameNum() << "\n";
    ofs_anim << "struct anim{std::string name;float data[anim_frame*16];};\n";
    ofs_anim << "anim _anim[anim_channal] = {\n";

    std::map< std::string, unsigned int > index = anim->getIndex();
    for (auto it = index.begin(); it != index.end(); it++)
    {
        if (it != index.begin())
        {
            ofs_pose << ",";
            ofs_anim << ",";
        }

        ofs_pose << "\"" << it->first.c_str() << "\" , {";
        ofs_anim << "\"" << it->first.c_str() << "\" , {";
        for (unsigned int i = 0; i < anim->getData()[it->second].size(); i++)
        {
            if (i < 16)
            {
                if (i != 0)
                {
                    ofs_pose << ",";
                }
                //ofs_pose.precision(1);
                ofs_pose << std::fixed << anim->getData()[it->second][i] << "f";
            }

            if (i != 0)
            {
                ofs_anim << ",";
            }
            ofs_anim << std::fixed << anim->getData()[it->second][i] << "f";
        }
        ofs_pose << "}\n";
        ofs_anim << "}\n";
    }


    ofs_pose << "};\n";
    ofs_pose.close();
    ofs_anim << "};\n";
    ofs_anim.close();
}

//搜索场景 建立一个Animation
//    _frame_begin 帧开始位置 -1为当前场景最早的帧数
//    _frame_count 帧长度     -1为自动识别最多帧数
egg::Animation* LoadAnimation(FbxScene* scene, int _frame_begin, int _frame_count)
{
    //建立anim对象
    egg::ref_ptr<egg::Animation> anim = new egg::Animation();
    egg::ref_ptr<egg::Animation> anim_global = new egg::Animation();
    anim->setUniformPath("scene");
    anim_global->setUniformPath("scene");

    //查询默认动画长度
    FbxAnimStack* stack = scene->GetCurrentAnimationStack();
    FbxTimeSpan localtimespan = stack->GetLocalTimeSpan();
    FbxTimeSpan reftimespan = stack->GetReferenceTimeSpan();

    //默认长度
    FbxTime start_time = reftimespan.GetStart();
    FbxTime end_time = reftimespan.GetStop();
    //FbxTime step_time  = reftimespan.GetSignedDuration();

    //根据输入参数修改开始终止位置
    if (_frame_begin > 0)
    {
        //FbxString aa = start_time.GetTimeString();
        //start_time.SetTimeString();
    }
    if (_frame_count > 0)
    {
    }

    //获取可能存在的动画信息
    getAnimation(scene->GetRootNode(), start_time, end_time, anim.get(), anim_global.get());
    if (anim->getIndex().size() == 0) return nullptr; //如果不存在直接返回null

    //输出动画的代码
    exportDefaultAnimationCode(anim_global.get());

    //返回正确的对象 并且释放内存管理
    return anim.release();
}

