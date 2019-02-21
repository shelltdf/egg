#include <egg/Reflection/ObjectHandle.h>
using namespace egg;

std::map<std::string, ref_ptr<ObjectHandle> > ObjectHandle::mObjectHandle;


ref_ptr<ObjectHandle> ObjectHandle::handle(Object* object)
{
    ref_ptr<ObjectHandle> handle = getHandle(object);
    //if (handle)
    //{
    //    handle->mObject = object;
    //}
    return handle;
}

//Object* ObjectHandle::create(const char* className)
//{
//    //具体类型的 ObjectHandle 扩展
//    ref_ptr<ObjectHandle> handle = getHandle(className);
//    if (handle.valid())
//    {
//        //创建对象指针
//        Object* object = handle->create();
//        //if (object)
//        //{
//        //    handle->mObject = object;
//        //}
//        return object;
//    }
//    //包裹 handle 对象返回
//    return 0;
//}

void ObjectHandle::registorHandle(ObjectHandle* objectHandle)
{
    //mObjectHandle[objectHandle->getClassName()] = CLRPTR<ObjectHandle>(objectHandle);

    std::string typeID = objectHandle->getClassID();
    mObjectHandle[typeID] = ref_ptr<ObjectHandle>(objectHandle);
}

ref_ptr<ObjectHandle> ObjectHandle::getHandle(const char* className)
{
    for (auto it = mObjectHandle.begin(); it != mObjectHandle.end(); it++)
    {
        if (it->second->getClassName() == className)
        {
            return it->second;
        }
    }
    return 0;
}
ref_ptr<ObjectHandle> ObjectHandle::getHandle(Object* object)
{
    for (auto it = mObjectHandle.begin(); it != mObjectHandle.end(); it++)
    {
        std::string typeID = typeid(*object).name();

        if (it->first == typeID)
        {
            return it->second;
        }
    }
    return 0;
}




ObjectHandle::ObjectHandle(const char* className)
    :mClassName(className)
    //, mObject(0)
{

}

ObjectHandle::~ObjectHandle()
{

}

std::vector<std::string> ObjectHandle::getAttrNames()
{
    std::vector<std::string> ret;

    //递归父对象
    for (size_t i = 0; i < mParentObjectHandle.size(); i++)
    {
        std::vector<std::string> attrnames = mParentObjectHandle[i]->getAttrNames();
        ret.insert(ret.end(), attrnames.begin(), attrnames.end());
    }

    for (auto it = mAttrs.begin(); it != mAttrs.end(); it++)
    {
        ret.push_back(it->first);
    }
    return ret;
}

ref_ptr<ValueHandle> ObjectHandle::getAttr(const char* name)
{
    //递归父对象
    for (size_t i = 0; i < mParentObjectHandle.size(); i++)
    {
        ref_ptr<ValueHandle> attr = mParentObjectHandle[i]->getAttr(name);
        if (attr.valid())
        {
            return attr;
        }
    }

    for (auto it = mAttrs.begin(); it != mAttrs.end(); it++)
    {
        if (it->first == name)
        {
            return it->second;
        }
    }
    return 0;
}

void ObjectHandle::registorAttr(ValueHandle* valueHandle)
{
    mAttrs[valueHandle->getName()] = ref_ptr<ValueHandle>(valueHandle);
}


#if 0
UniformNamingObject* UniformNamingObject::Instance = 0;

UniformNamingObject* UniformNamingObject::getInstance()
{
    if (Instance == 0)
    {
        Instance = new UniformNamingObject();
    }
    return Instance;
}
void UniformNamingObject::delInstance()
{
    if (Instance)
    {
        delete Instance;
        Instance = 0;
    }
}

#endif