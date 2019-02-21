
#include <egg/Reflection/ObjectHandle.h>
#include <string>

#include "testObjectHandle.h"

std::string getIndent(int indent)
{
    std::string str;
    for (size_t i = 0; i < indent; i++)
    {
        str += " ";
    }
    return str;
}

void printObject(egg::Object* object, int indent);

void printValue(egg::Object* object, egg::ref_ptr<egg::ValueHandle> attr, int indent = 0)
{
    std::string k = getIndent(indent);

    egg::ValueHandleValueType attr_value_type = attr->getValueType();
    egg::ValueHandleSetType attr_set_type = attr->getSetType();
    std::string name = attr->getName();

    if (attr_value_type == egg::VHVT_Value)
    {
        std::string type = attr->getDataType();
        std::string codec = attr->getDataCodec();
        std::string str = attr->getDataString(object);

        printf("%s    Value %s", k.c_str(), name.c_str());
        printf(" type %s", type.c_str());
        printf(" codec %s", codec.c_str());
        printf(" str \"%s\"", str.c_str());
        printf("\n");
    }
    if (attr_value_type == egg::VHVT_Enum)
    {
        std::string str = attr->getDataString(object);

        printf("%s    Enum %s", k.c_str(), name.c_str());
        printf(" str \"%s\"", str.c_str());
        printf("\n");

    }
    if (attr_value_type == egg::VHVT_Ref)
    {
        printf("%s    Ref %s", k.c_str(), name.c_str());
        printf("\n");

        //进入递归方式
        egg::Object* attr_object = attr->getObject(object);
        std::string data_type = attr->getDataType();
        if (attr_object)
        {
            printf("%s    {\n", k.c_str());
            printObject(/*data_type.c_str(),*/ attr_object, (indent + 8));
            printf("%s    }\n", k.c_str());
        }
    }
}

void printObject(egg::Object* object, int indent = 0)
{
    std::string k = getIndent(indent);

    egg::ref_ptr<egg::ObjectHandle> oh = egg::ObjectHandle::handle(object);

    printf("%sclass %s\n", k.c_str(), oh->getClassName().c_str());

    //输出全部对象属性
    std::vector<std::string> attr_name_list = oh->getAttrNames();
    for (size_t i = 0; i < attr_name_list.size(); i++)
    {
        egg::ref_ptr<egg::ValueHandle> attr = oh->getAttr(attr_name_list[i].c_str());
        egg::ValueHandleValueType attr_value_type = attr->getValueType();
        egg::ValueHandleSetType attr_set_type = attr->getSetType();
        std::string name = attr->getName();

        if (attr_set_type == egg::VHST_Single)
        {
            printValue(object, attr, indent);
        }
        if (attr_set_type == egg::VHST_Vector)
        {
            printf("%s    Vector %s", k.c_str(), name.c_str());
            //printf("\n");

            int dim = attr->getDimension(object);
            printf(" dim %d\n", dim);

            printf("%s    [\n", k.c_str());
            for (size_t d = 0; d < dim; d++)
            {
                int dim_size = attr->getDimensionSize(object,d);
                for (size_t i = 0; i < dim_size; i++)
                {
                    attr->setIndex(object, d, i);
                    egg::ValueHandleValueType attr_value_type = attr->getValueType();
                    if (attr_value_type == egg::VHVT_Ref)
                    {
                        egg::Object* attr_object = attr->getObject(object);
                        //std::string data_type = attr->getDataType();
                        if (attr_object)
                        {
                            //printf("%s    {\n", k.c_str());
                            printObject(attr_object, (indent + 8));
                            //printf("%s    }\n", k.c_str());
                        }
                    }
                    else
                    {
                        printValue(object, attr, indent);
                    }
                }
            }
            printf("%s    ]\n", k.c_str());
            
        }
        if (attr_set_type == egg::VHST_Map)
        {
            printf("%s    Map %s", k.c_str(), name.c_str());
            //printf("\n");

            int dim = attr->getDimension(object);
            printf(" dim %d\n", dim);

            printf("%s    [\n", k.c_str());
            for (size_t d = 0; d < dim; d++)
            {
                //int dim_size = attr->getDimensionSize(object, d);
                //for (size_t i = 0; i < dim_size; i++)

                std::vector<std::string> keys = attr->getKeys(object, d);
                for (size_t i = 0; i < keys.size(); i++)
                {
                    attr->setKey(object, d, keys[i]);
                    egg::ValueHandleValueType attr_value_type = attr->getValueType();
                    if (attr_value_type == egg::VHVT_Ref)
                    {
                        egg::Object* attr_object = attr->getObject(object);
                        //std::string data_type = attr->getDataType();
                        if (attr_object)
                        {
                            //printf("%s    {\n", k.c_str());
                            printObject(attr_object, (indent + 8));
                            //printf("%s    }\n", k.c_str());
                        }
                    }
                    else
                    {
                        printValue(object, attr, indent);
                    }
                }
            }
            printf("%s    ]\n", k.c_str());
        }
        //if (attr_set_type == CLR::VHST_Set)
        //{
        //    printf("%s    Set %s", k.c_str(), name.c_str());
        //    printf("\n");

        //}

        
    }
}


//void printMyClassInfo(void *p) 
//{
//    type_info*** vtable = (type_info***)(*(int*)p);
//    type_info** v1 = vtable[-1];
//    type_info* v = v1[3];
//    printf("%s\n", v->name());
//}

void test_1()
{
    // 测试新建对象方式
    // 测试属性类型的set get操作

    printf("test_1 begin\n");

    egg::ObjectManager* om = egg::ObjectManager::getInstance();

    //建立对象
    egg::Object* aaaa = om->newObject("AAAA");
    egg::Object* bbbb = om->newObject("BBBB");
    egg::Object* cccc = om->newObject("CCCC");

    //printMyClassInfo(aaaa);

    std::string temp1(typeid(*cccc).name());


    //设置属性值
    egg::ObjectHandle::handle(aaaa)->getAttr("name")->setDataString(aaaa, "QQ");
    egg::ObjectHandle::handle(aaaa)->getAttr("id")->setDataString(aaaa, "99");

    egg::ObjectHandle::handle(bbbb)->getAttr("name")->setDataString(bbbb, "bQQ");
    egg::ObjectHandle::handle(bbbb)->getAttr("id")->setDataString(bbbb, "199");
    egg::ObjectHandle::handle(bbbb)->getAttr("nameB")->setDataString(bbbb, "bbbbbbbbbbbbb");

    egg::ObjectHandle::handle(cccc)->getAttr("name")->setDataString(cccc, "cQQ");
    egg::ObjectHandle::handle(cccc)->getAttr("id")->setDataString(cccc, "299");
    egg::ObjectHandle::handle(cccc)->getAttr("nameB")->setDataString(cccc, "ssssssssssssss");
    egg::ObjectHandle::handle(cccc)->getAttr("nameC")->setDataString(cccc, "cccccccccccccc");

    //引用对象
    egg::ObjectHandle::handle(aaaa)->getAttr("ref")->setObject(aaaa, bbbb);
    egg::ObjectHandle::handle(bbbb)->getAttr("ref")->setObject(bbbb, cccc);

    //测试vector
    egg::ObjectHandle::handle(aaaa)->getAttr("children")->pushObject(aaaa, 0, bbbb);
    egg::ObjectHandle::handle(aaaa)->getAttr("children")->pushObject(aaaa, 0, cccc);

    //测试map
    egg::ObjectHandle::handle(aaaa)->getAttr("library")->insertObject(aaaa, 0, "ID_01", bbbb);
    egg::ObjectHandle::handle(aaaa)->getAttr("library")->insertObject(aaaa, 0, "ID_02", bbbb);


    //输出全部对象属性
    printObject(/*"AAAA",*/ aaaa);
    printObject(/*"BBBB",*/ bbbb);
    printObject(/*"CCCC",*/ cccc);

    printf("test_1 end\n");
}

//void test_2()
//{
//    //测试从对象创建handle
//
//    printf("test_2 begin\n");
//
//
//    AAAA* aaaa = new AAAA();
//    BBBB* bbbb = new BBBB();
//    CCCC* cccc = new CCCC();
//
//    //CLR::ObjectHandle object_b(b);
//
//    CLRPTR<CLR::ObjectHandle> object_aaaa = CLR::ObjectHandle::handle("AAAA", aaaa);
//    CLRPTR<CLR::ObjectHandle> object_bbbb = CLR::ObjectHandle::handle("BBBB", bbbb);
//    CLRPTR<CLR::ObjectHandle> object_cccc = CLR::ObjectHandle::handle("CCCC", cccc);
//
//    //设置属性值
//    object_aaaa->getAttr("name")->setDataString("TT");
//    object_aaaa->getAttr("id")->setDataString("66");
//
//    //输出全部对象属性
//    print(object_aaaa);
//    print(object_bbbb);
//    print(object_cccc);
//
//    printf("test_2 end\n");
//
//}

void test_cmd()
{
    //测试使用命令对象操作数据

    //执行命令需要有一个context容器

}

void test_rw()
{
    //测试保存和读取

    //保存的过程是将内存对象全部转换成 Command 然后再保存

    //读文件后 通过 Command 完成对内存对象的操作
    
}

int main(int argc, char *argv[])
{
    egg::ObjectManager* om = egg::ObjectManager::getInstance(true);
    om->registerObjectClass<AAAA>("AAAA");
    om->registerObjectClass<BBBB>("BBBB");
    om->registerObjectClass<CCCC>("CCCC");


    egg::ObjectHandle::registorHandle(new ObjectHandleAAAA());
    egg::ObjectHandle::registorHandle(new ObjectHandleBBBB());
    egg::ObjectHandle::registorHandle(new ObjectHandleCCCC());


    test_1();
    //test_2();

    test_rw();

}

