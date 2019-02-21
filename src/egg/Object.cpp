
#include <egg/Object.h>



//#include <nbSG/Node.h>
//#include <nbSG/SceneNode.h>
//
//#include <nbSG/Geometry.h>
//#include <nbSG/Image.h>
//#include <nbSG/Material.h>
//#include <nbSG/Program.h>
//#include <nbSG/Scene.h>
//#include <nbSG/Shader.h>
//#include <nbSG/Texture.h>
//#include <nbSG/Animation.h>
//
//
//#include <nbSG/MeshRender.h>
//#include <nbSG/ActorControler.h>
//#include <nbSG/SkeletonActorControler.h>


using namespace egg;

IMPLEMENT_SINGLETON(ObjectManager)

#define ROC(CLASS) registerObjectClass<CLASS>(#CLASS);

void ObjectManager::registerDefaultObjectClass()
{
    //registerObjectClass<Object>("Object");
    ROC(Object)
    
#if 0
    //资源相关
    ROC(Geometry)
    ROC(Image)
    ROC(Material)
    ROC(Program)
    ROC(Scene)
    ROC(Shader)
    ROC(Texture)
    ROC(Animation)

    //场景相关
    ROC(Node)
    ROC(SceneNode)

    //行为组件
    ROC(Transfrom3D)
    ROC(Joint3D)
    ROC(MeshRender)
    ROC(ActorControler)
    ROC(SkeletonActorControler)
    ROC(Light)
    ROC(Camera)
    //ROC(Line)
#endif

}


std::set< observer_ptr<Object> > Object::Objects;

std::string Object::getUniformName(Object* self, std::string name)
{
    std::string ret = name;

    //收集已有名称
    std::set< std::string > name_list;
    for (auto it = Objects.begin(); it != Objects.end(); it++)
    {
        ref_ptr<Object> s = (*it).get();

        std::string n = s->getName();
        if (s.get() != self) name_list.insert(n);

        //防止弱指针被释放
        s.release();
    }

    //如果存在就一直循环
    while (name_list.find(ret) != name_list.end())
    {
        //////////////////////////////////////////////////////////////////////////
        //复杂的方式  判断末尾的数字 并加1
        //////////////////////////////////////////////////////////////////////////

        //确定最后一组有效数字的开始index
        int shuzu_index = ret.size();//这个索引表示不可能找到数字
        for (int i = ret.size() - 1; i >= 0; i--)
        {
            if (ret[i] >= '0' &&  ret[i] <= '9')
            {
                shuzu_index = i;
            }
            else
            {
                break;
            }
        }

        //判断是否没有找数字
        if (shuzu_index == ret.size())
        {
            ret += "1";
        }

        //找到了数字
        else
        {
            //得到数字区域
            std::string shuzi = ret.substr(shuzu_index, ret.size() - shuzu_index);
            int int_shuzi = atoi(shuzi.c_str()) + 1;

            //计算结果
            char bu[128] = {};
            sprintf(bu, "%d", int_shuzi);
            ret = ret.substr(0, shuzu_index) + bu;
        }
    }

    return ret;
}

void Object::addObject(Object* object)
{
    Objects.insert(object);
}

void Object::delObject(Object* object)
{
    auto it = Objects.find(object);
    if (it != Objects.end())
    {
        Objects.erase(it);
    }
}

//void Object::cleanup()
//{
//    //for (auto it = Objects.begin(); it != Objects.end(); it++)
//    //{
//    //    if(it->release)
//    //}
//
//}

