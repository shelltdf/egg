
#ifndef _NSG_CORE_OBJECT_H_
#define _NSG_CORE_OBJECT_H_

#include <egg/Export.h>

#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

#include <egg/ref_ptr.h>
#include <egg/observer_ptr.h>

#include <egg/Path.h>
#include <egg/Serializer.h>
#include <egg/Factory.h>

#include <egg/Event.h>


namespace egg
{

    class ObjectManager;

    /*
    *  基本对象类
    *  可序列化的基础类
    */
    class EGG_RUNTIME_EXPORT Object
        :public Referenced
    {
    public:

        Object(std::string name = "object")
        {
            //std::string name = getUniformName("object");
            setName(name);
            addObject(this);
        }
        virtual ~Object()
        {
            delObject(this);
        }

        virtual std::string ObjectClassName(){return "object";}

        //名称
        void setName(std::string name)
        { 
            name = getUniformName(this, name);
            mName = name;
        }
        std::string getName() { return mName; }


        //序列化功能支持
        virtual void save( Path filename , std::ostream& os , Serializer* ser ) const
        {
            ser->writeString(os, mName); //名字
        }

        virtual void load( Path filename , std::istream& is , Serializer* ser 
            , ObjectManager* om , Referenced* data , bool load_ext )
        {
            ser->readString(is, mName); //名字
        }

        //统一名称功能接口
        static std::string getUniformName(Object* self,std::string name);
        static void addObject(Object* object);
        static void delObject(Object* object);
        //static void cleanup();

        //属性连接和计算支持
        //（等待添加功能）


        //listener模式
        //（等待添加功能）
        void addListender(Object* listender)
        {
            mListenders.insert(listender);
        }
        void removeListender(Object* listender)
        {
            auto it = mListenders.find(listender);
            if (it != mListenders.end())
            {
                mListenders.erase(it);
            }
        }
        void fireEvent(Event* event)
        {
            //for (size_t i = 0; i < mListenders.size(); i++)
            for (auto it = mListenders.begin(); it != mListenders.end(); it++)
            {
                //发送消息
                ref_ptr<Object> obj = (*it).get();
                obj->onEvent(this, obj.get(), event);

                //为了保证弱指针不会释放
                obj.release();
            }
        }
        virtual void onEvent(Object* from, Object* to, Event* event) {}

    private:

        //这里统计全部Object的实例
        static std::set< observer_ptr<Object> > Objects;

    private:

        std::string mName;

        std::set< observer_ptr<Object> > mListenders;

    };

    //对象工厂
    class ObjectFactory
        :public Referenced
    {
    public:
        ObjectFactory(){}
        virtual ~ObjectFactory(){}
        virtual Object* newObject() = 0;
    };

    //对象工厂 模板类
    template<class T>
    class ObjectFactoryT
        :public ObjectFactory
    {
    public:
        ObjectFactoryT(){}
        virtual ~ObjectFactoryT(){}
        Object* newObject()
        {
            return new T();
        }
    };

    //对象管理器
    class EGG_RUNTIME_EXPORT ObjectManager
        :public Object
    {
    public:
        ObjectManager() { registerDefaultObjectClass(); }
        virtual ~ObjectManager(){}

        //manager(ObjectManager)
        DECLARE_SINGLETON(ObjectManager)

        template<class T>
        void registerObjectClass( const std::string& classname )
        {
            //强制注册小写名称
            std::string a = classname;
            std::transform(a.begin(), a.end(), a.begin(), ::tolower);

            mObjectFactory[a] = new ObjectFactoryT<T>();
        }

        Object* newObject( const std::string& classname )
        {
            //auto it = mObjectFactory.find(classname);
            //if(it != mObjectFactory.end() )
            //{
            //    return it->second->newObject();
            //}

            //忽略大小写 强制使用小写
            for (auto it = mObjectFactory.begin(); it != mObjectFactory.end(); it++)
            {
                std::string a = classname;
                std::string b = it->first;

                std::transform(a.begin(), a.end(), a.begin(), ::tolower);
                std::transform(b.begin(), b.end(), b.begin(), ::tolower);

                if( a == b )
                {
                    return it->second->newObject();
                }
            }
            return nullptr;
        }

        void registerDefaultObjectClass();

    private:

        std::map< std::string , ref_ptr< ObjectFactory > > mObjectFactory;
    };


}//namespace nbSG


#endif //_NSG_CORE_OBJECT_H_
