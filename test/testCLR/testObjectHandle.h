#ifndef _TEST_ObjectHandle_H_
#define _TEST_ObjectHandle_H_

#include "testBaseClass.h"


class ObjectHandleAAAA
    :public egg::ObjectHandle
{
public:
    ObjectHandleAAAA(const char* className = "AAAA")
        :egg::ObjectHandle(className)
    {
        std::string typeID = typeid(AAAA).name();
        mClassID = typeID;

        //mInheritance.push_back();

        ObjectHandle::addEnum("OBJECTTYPE", "OC_AAAA", OC_AAAA);
        ObjectHandle::addEnum("OBJECTTYPE", "OC_BBBB", OC_BBBB);
        ObjectHandle::addEnum("OBJECTTYPE", "OC_CCCC", OC_CCCC);

        {
            egg::ValueHandle* vh_type = new egg::ValueHandle("OBJECTTYPE", "type", egg::VHVT_Enum, egg::VHST_Single);
            vh_type->getDataString = [=](egg::Object* object) {return ObjectHandle::getEnum("OBJECTTYPE", ((AAAA*)object)->type); };
            vh_type->setDataString = [=](egg::Object* object, std::string str) {((AAAA*)object)->type = (OBJECTTYPE)ObjectHandle::getEnum("OBJECTTYPE", str); };
            this->registorAttr(vh_type);
        }
        {
            egg::ValueHandle* vh_id = new egg::ValueHandle("int", "id", egg::VHVT_Value, egg::VHST_Single);
            vh_id->getDataString = [=](egg::Object* object) {return egg::numToString(((AAAA*)object)->id); };
            vh_id->setDataString = [=](egg::Object* object, std::string str) {((AAAA*)object)->id = atoi(str.c_str()); };
            this->registorAttr(vh_id);
        }
        {
            egg::ValueHandle* vh_name = new egg::ValueHandle("std::string", "name", egg::VHVT_Value, egg::VHST_Single);
            vh_name->getDataString = [=](egg::Object* object) {return egg::numToString(((AAAA*)object)->name); };
            vh_name->setDataString = [=](egg::Object* object, std::string str) {((AAAA*)object)->name = str; };
            this->registorAttr(vh_name);
        }
        {
            egg::ValueHandle* vh_ref = new egg::ValueHandle("AAAA*", "ref", egg::VHVT_Ref, egg::VHST_Single);
            //vh_ref->getDataString = [&](CLROBJECT* object) {return ""; };
            //vh_ref->setDataString = [&](CLROBJECT* object, std::string str) {};
            vh_ref->getObject = [=](egg::Object* object) {return ((AAAA*)object)->ref; };
            vh_ref->setObject = [=](egg::Object* object, egg::Object* attr_object) { ((AAAA*)object)->ref = (AAAA*)attr_object; };
            this->registorAttr(vh_ref);
        }

        {
            egg::ValueHandle* vh_children = new egg::ValueHandle("std::vector<AAAA*>", "children", egg::VHVT_Ref, egg::VHST_Vector);
            //vh_children->getDataString = [=](CLROBJECT* object) {return ""; };
            //vh_children->setDataString = [=](CLROBJECT* object, std::string str) {};
            vh_children->getObject = [=](egg::Object* object)
            {
                return ((AAAA*)object)->children[vh_children->mIndex[0]];
            };
            vh_children->setObject = [=](egg::Object* object, egg::Object* attr_object)
            {
                ((AAAA*)object)->children[vh_children->mIndex[0]] = (AAAA*)attr_object;
            };
            vh_children->getDimension = [=](egg::Object* object) {return 1; };
            vh_children->getDimensionSize = [=](egg::Object* object, int dim) {return ((AAAA*)object)->children.size(); };
            vh_children->setIndex = [=](egg::Object* object, int dim, int index)
            {
                if (dim >= vh_children->mIndex.size())vh_children->mIndex.resize(dim+1);
                vh_children->mIndex[dim] = index; 
            };
            vh_children->remove = [=](egg::Object* object, int dim)
            {
                auto it = ((AAAA*)object)->children.begin()+vh_children->mIndex[0];
                ((AAAA*)object)->children.erase(it);
            };
            vh_children->clear = [=](egg::Object* object) {((AAAA*)object)->children.clear(); };
            //vh_children->pushDataString = [=](CLROBJECT* object, int dim, std::string str)
            //{
            //    ((AAAA*)object)->children.push_back(str);
            //};
            vh_children->pushObject = [=](egg::Object* object, int dim, egg::Object* attr_object)
            {
                ((AAAA*)object)->children.push_back((AAAA*)attr_object);
            };

            this->registorAttr(vh_children);
        }

        {
            egg::ValueHandle* vh_children = new egg::ValueHandle("std::map<std::string,CLRPTR<AAAA>>", "library", egg::VHVT_Ref, egg::VHST_Map);
            vh_children->getObject = [=](egg::Object* object)
            {
                //return ((AAAA*)object)->library[vh_children->mKeys[0]].get();
                return ((AAAA*)object)->library[vh_children->mKeys[0]];
            };
            vh_children->setObject = [=](egg::Object* object, egg::Object* attr_object)
            {
                //((AAAA*)object)->library[vh_children->mKeys[0]] = CLRPTR<AAAA>((AAAA*)attr_object);
                ((AAAA*)object)->library[vh_children->mKeys[0]] = ((BBBB*)attr_object);
            };
            vh_children->getDimension = [=](egg::Object* object) {return 1; };
            vh_children->getDimensionSize = [=](egg::Object* object, int dim) {return ((AAAA*)object)->library.size(); };
            vh_children->getKeys = [=](egg::Object* object, int dim)
            {
                //std::map<std::string, CLRPTR<AAAA>>& library = ((AAAA*)object)->library;
                std::vector<std::string> ret;
                for (auto it = ((AAAA*)object)->library.begin()
                    ; it != ((AAAA*)object)->library.end()
                    ; it++)
                {
                    ret.push_back(it->first);
                }
                return ret;
            };
            vh_children->setKey = [=](egg::Object* object, int dim, std::string key)
            {
                if (dim >= vh_children->mKeys.size())vh_children->mKeys.resize(dim + 1);
                vh_children->mKeys[dim] = key;
            };
            vh_children->remove = [=](egg::Object* object, int dim)
            {
                auto it = ((AAAA*)object)->library.find(vh_children->mKeys[dim]);
                if (it != ((AAAA*)object)->library.end())
                {
                    ((AAAA*)object)->library.erase(it);
                }
            };
            vh_children->clear = [=](egg::Object* object) {((AAAA*)object)->children.clear(); };
            vh_children->insertObject = [=](egg::Object* object, int dim, std::string key, egg::Object* attr_object)
            {
                //((AAAA*)object)->library[key] = CLRPTR<AAAA>((AAAA*)attr_object);
                ((AAAA*)object)->library[key] = ((BBBB*)attr_object);
            };

            this->registorAttr(vh_children);
        }
    }

    virtual ~ObjectHandleAAAA()
    {
    }

    //virtual egg::Object* create() override
    //{
    //    return new AAAA();
    //}
private:

};

class ObjectHandleBBBB
    :public egg::ObjectHandle
{
public:
    ObjectHandleBBBB(const char* className = "BBBB")
        :egg::ObjectHandle(className)
    {
        std::string typeID = typeid(BBBB).name();
        mClassID = typeID;

        //mParentObjectHandle.push_back(CLRPTR<ObjectHandle>(new ObjectHandleAAAA()));
        mParentObjectHandle.push_back(getHandle("AAAA"));

        egg::ValueHandle* vh_name = new egg::ValueHandle("std::string", "nameB");
        vh_name->getDataString = [&](egg::Object* object) {return egg::numToString(((BBBB*)object)->nameB); };
        vh_name->setDataString = [&](egg::Object* object, std::string str) {((BBBB*)object)->nameB = str; };
        this->registorAttr(vh_name);

    }

    virtual ~ObjectHandleBBBB()
    {
    }

    //virtual egg::Object* create() override
    //{
    //    return new BBBB();
    //}
private:

};

class ObjectHandleCCCC
    :public egg::ObjectHandle
{
public:
    ObjectHandleCCCC(const char* className = "CCCC")
        :egg::ObjectHandle(className)
    {
        std::string typeID = typeid(CCCC).name();
        mClassID = typeID;

        //mInheritance.push_back(CLRPTR<ObjectHandle>(new ObjectHandleAAAA()));
        //mParentObjectHandle.push_back(CLRPTR<ObjectHandle>(new ObjectHandleBBBB()));
        mParentObjectHandle.push_back(getHandle("BBBB"));

        egg::ValueHandle* vh_name = new egg::ValueHandle("std::string", "nameC");
        vh_name->getDataString = [&](egg::Object* object) {return egg::numToString(((CCCC*)object)->nameC); };
        vh_name->setDataString = [&](egg::Object* object, std::string str) {((CCCC*)object)->nameC = str; };
        this->registorAttr(vh_name);
    }

    virtual ~ObjectHandleCCCC()
    {
    }

    //virtual egg::Object* create()override
    //{
    //    return new CCCC();
    //}
private:

};


#endif // !_TEST_ObjectHandle_H_

