
#include "Export.h"


int main(int argc, char* argv[])
{
    //命令行开关
    bool collapse_mesh = false;                 //允许mesh塌陷 但是强制skin mesh塌陷(不可控制)
    bool exoprt_sikinning_controler = false;    //允许输出skin动画控制器
    bool exoprt_animation_controler = false;    //允许输出动画控制器
    bool exoprt_animation = false;               //输出动画信息文件
    bool only_animation = false;                 //只输出动画信息文件

    //默认输出格式 exe xx.fbx
    if (argc < 2) return 1;
    std::string fbxfilename = argv[1];

    //判断开关
    if (argc == 2) //只有文件名 没有开关
    {
        //默认输出尽可能保留场景原始信息
        collapse_mesh = false;
        exoprt_sikinning_controler = true;
        exoprt_animation_controler = true;
        exoprt_animation = true;
    }
    else if (argc >= 3)
    {
        if (std::string(argv[2]) == "-static")
        {
            //静态模型数据 只保留静态的可渲染信息
            collapse_mesh = true;
            exoprt_sikinning_controler = false;
            exoprt_animation_controler = false;
            exoprt_animation = false;
        }
        if (std::string(argv[2]) == "-skinning")
        {
            //输出skin角色
            collapse_mesh = true;  //角色身体上的附加模型因为没有skin变形器 所以可以跳过塌陷操作
            exoprt_sikinning_controler = true;
            exoprt_animation_controler = true;
            exoprt_animation = false;
        }
        if (std::string(argv[2]) == "-animation")
        {
            //只输出动画
            exoprt_animation = true;
            only_animation = true;
        }
    }


    //导入FBX场景
    FbxManager* lSdkManager = FbxManager::Create();
    FbxScene* fbx_scene = importFBXScene(lSdkManager, fbxfilename.c_str());
    if (!fbx_scene)
    {
        lSdkManager->Destroy();
        return 1;
    }

    //getPose( fbx_scene );

    //初始化资源资源管理器
    egg::niubi_entry_arg arg;
    egg::ref_ptr< egg::ResourceManager > rmg;
    rmg = new egg::ResourceManager();
    rmg->init(arg, true);

    //注册写入包路径
    std::string export_dirname = fbxfilename.substr(0, fbxfilename.size() - 4);
    rmg->registerArchive("", export_dirname, true);

    //建立默认资源
    //ExportDefaultProgramAndShader(rmg.get());
    //ExportDefaultTextureAndImage( rmg.get() );
    rmg->updateUniformAssetPath(); //更新asset表


    //建立动画信息（当前默认的stack）
    egg::ref_ptr<egg::Animation> anim = LoadAnimation(fbx_scene);

    //如果只是输出动画  打开现有场景 直接写入动画控制器 以便达到替换动画的目的
    if (only_animation)
    {
        //直接写入animation
        rmg->writeResource(anim.get());

        //打开现有场景
        egg::ResourceGroup rg;
        egg::Resource* res = rmg->readResourceGroup(egg::Path("|scene.scene"), &rg, false);
        if (res)
        {
            egg::Scene* nb_scene = dynamic_cast<egg::Scene*>(res);
            if (nb_scene)
            {
                printf("load scene : %s\n", nb_scene->getUniformPath().getPathMel().c_str());

                egg::SkeletonActorControler* sac = nb_scene->getRoot()->getComponent<egg::SkeletonActorControler>();
                if (sac) sac->setAnimation(anim.get());

                egg::ActorControler* ac = nb_scene->getRoot()->getComponent<egg::SkeletonActorControler>();
                if (sac) sac->setAnimation(anim.get());
            }
            rmg->writeResource(nb_scene);
        }

        // Destroy the SDK manager and all the other objects it was handling.
        lSdkManager->Destroy();
        printf("done animation\n");
        return 0;
    }


    //建立DOM scene
    egg::Scene* scene = new egg::Scene();
    scene->setUniformPath("|scene");

    //搜索可能存在的骨骼动画管理器
    egg::SkeletonActorControler* sac = LoadSkeletonActorControler(fbx_scene);
    if (sac && exoprt_sikinning_controler)
    {
        if (anim.valid() && exoprt_animation) sac->setAnimation(anim.get());
        scene->getRoot()->addComponent(sac);
    }
    else
    {
        //如果存在动画信息 就建立层级动画管理器
        if (anim.valid() && exoprt_animation_controler)
        {
            egg::ActorControler* ac = new egg::ActorControler();
            if (anim.valid() && exoprt_animation) ac->setAnimation(anim.get());
            scene->getRoot()->addComponent(ac);
        }
    }


    //需要复制的外部数据 主要是image
    //from 本地绝对路径
    //to   uniform路径
    std::map<std::string/*from*/, std::string/*to*/> copy_image_list;

    //从根节点开始递归输出
    printf("export root node\n");
    FbxNode* lRootNode = fbx_scene->GetRootNode();
    if (lRootNode)
    {
        for (int i = 0; i < lRootNode->GetChildCount(); i++)
        {
            egg::SceneNode* child = exportNode(lRootNode->GetChild(i), sac
                , copy_image_list, collapse_mesh, exoprt_sikinning_controler);
            scene->getRoot()->addChild(child);
        }
    }
    printf("export root node done\n");

    //copy images
    printf("copy images\n");
    for (auto it = copy_image_list.begin(); it != copy_image_list.end(); it++)
    {
        printf("copy \"%s\" \"%s\"\n", it->first.c_str(), it->second.c_str());
        std::vector<char> f = readfile(it->first);
        rmg->writeFile(egg::Path(it->second), &f[0], f.size());
    }

    //build resource group
    egg::ResourceGroup rg(scene);
    rg.searchAll();

    //write resource group
    printf("write resource group\n");
    rmg->writeResourceGroup(&rg);

    //destroy fbx sdk manager
    lSdkManager->Destroy();
    printf("done\n");
    return 0;
}
