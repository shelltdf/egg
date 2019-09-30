#ifndef _EGG_Factory_H_
#define _EGG_Factory_H_

#include <egg/Singleton.h>
//#include <eggRuntime/Object.h>
#include <egg/ref_ptr.h>
#include <map>
#include <string>

namespace egg
{

#if 0
    /*
    FACTORY 主要用于代理工厂注册
    */
#define DECLARE_FACTORY(T)\
class FactoryBase :public egg::Object\
{\
public:\
    FactoryBase() {}\
    virtual ~FactoryBase() {}\
    virtual T* create() = 0;\
};\
template <class P>\
class Factory :public FactoryBase\
{\
public:\
    Factory() {}\
    virtual ~Factory() {}\
    virtual T* create() override { return new P(); }\
};\
static std::map<const char* ,egg::ref_ptr<FactoryBase> > mFactoryMap;\
static void registerFactory(const char* name , FactoryBase* factory);\
static void unregisterFactory(const char* name);\
static T* create(const char* name);

#define IMPLEMENT_FACTORY(T)\
std::map<const char*, egg::ref_ptr<T::FactoryBase> > T::mFactoryMap; \
void T::registerFactory(const char* name, T::FactoryBase* factory){mFactoryMap[name]=factory;}\
void T::unregisterFactory(const char* name){auto it = mFactoryMap.find(name);if(it!=mFactoryMap.end()) mFactoryMap.erase(it);}\
T* T::create(const char* name){auto it = mFactoryMap.find(name);if(it!=mFactoryMap.end()) return it->second->create();else return 0;}

#endif

    class Object;

    class CrteatorBase :public egg::Referenced
    {
    public:
        CrteatorBase() {}
        virtual ~CrteatorBase() {}
        virtual Object* create() = 0;
    };

    template <class P>
    class Crteator :public CrteatorBase
    {
    public:
        Crteator() {}
        virtual ~Crteator() {}
        virtual Object* create() override { return new P(); }
    };

    template<class T>
    class Factory
    {
    public:
        Factory() {}
        ~Factory() {}

        //DECLARE_SINGLETON(Factory)

        //static Factory* mInstance;
        //static Factory* getOrCreateInstance();
        //static Factory* getInstance();
        //static void delInstance()

        void registerCrteator(const std::string& name, CrteatorBase* creator)
        {
            mCrteatorMap[name] = creator;
        }
        void unregisterCrteator(const std::string& name)
        {
            auto it = mCrteatorMap.find(name);
            if (it != mCrteatorMap.end()) mCrteatorMap.erase(it);
        }

        T* create(const std::string& name)
        {
            auto it = mCrteatorMap.find(name);
            if (it != mCrteatorMap.end())
            {
                ref_ptr<Object> obj = it->second->create();

                return dynamic_cast<T*>(obj.release());
            }
            else return 0;
        }

        const std::map<const std::string, egg::ref_ptr<CrteatorBase> >& getCrteatorMap() { return mCrteatorMap; }

    protected:
    //private:

        std::map<const std::string, egg::ref_ptr<CrteatorBase> > mCrteatorMap;
    };

    //IMPLEMENT_SINGLETON(Factory)
    //T* T::mInstance = 0; \
    //T* T::getOrCreateInstance() { if (mInstance == 0) { mInstance = new T(); } return mInstance; }\
    //T* T::getInstance() { return mInstance; }\
    //void T::delInstance() { mInstance = 0; }

}//namespace egg

#endif //_EGG_Factory_H_

