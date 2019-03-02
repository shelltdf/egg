#ifndef NIUBI_SG_RESOURCE_H
#define NIUBI_SG_RESOURCE_H

#include <eggEngine/Export.h>

#include <map>
#include <vector>

//#include <eggBase/Public.h>
//#include <eggRuntime/Object.h>
#include <egg/Node.h>
#include <egg/NodeVisitor.h>

namespace egg
{

    /*
    * 资源对象基础类
    */

//单值资源链接
#define RESOURCE_LINK(TYPE) \
public:\
    void set##TYPE( TYPE* r , ResourceLinkType t = RLT_Local_Relative )\
    {\
        m##TYPE = new ResourceLinkT< TYPE >( r , t );\
    }\
    TYPE* get##TYPE()const{return m##TYPE.valid()?m##TYPE->getResource():nullptr;}\
    ResourceLinkType getLinkType()const{return m##TYPE.valid()?m##TYPE->getLinkType():RLT_Null;}\
private:\
    egg::ref_ptr< ResourceLinkT< TYPE > > m##TYPE;


//std::vector 资源链接   /*这里对越界的操作只是作为了追加*/
#define RESOURCE_LINK_VECTOR(TYPE) \
public:\
    void set##TYPE( unsigned int index , TYPE* r , ResourceLinkType t = RLT_Local_Relative )\
    {\
        if( index >= m##TYPE.size() ){ m##TYPE.resize(index+1); }\
        m##TYPE[index] = new ResourceLinkT< TYPE >( r , t );\
    }\
    TYPE* get##TYPE( unsigned int index )const\
    {\
        return ( (index<m##TYPE.size()) && (m##TYPE[index].valid()) )?m##TYPE[index]->getResource():nullptr;\
    }\
    void add##TYPE( TYPE* r , ResourceLinkType t = RLT_Local_Relative )\
    {\
        m##TYPE.push_back( new ResourceLinkT< TYPE >( r , t ) );\
    }\
    void remove##TYPE( unsigned int index )\
    {\
        if( index < m##TYPE.size()) m##TYPE.erase(m##TYPE.begin()+index);\
    }\
    ResourceLinkType getLinkType( unsigned int index )const{return (index<m##TYPE.size()&&m##TYPE[index].valid())?m##TYPE[index]->getLinkType():RLT_Null;}\
    unsigned int getNum##TYPE()const{ return m##TYPE.size(); }\
private:\
    std::vector< egg::ref_ptr< ResourceLinkT< TYPE > > > m##TYPE;


//std::map 资源链接
#define RESOURCE_LINK_MAP(KEY,TYPE) \
public:\
    void set##TYPE( KEY key , TYPE* r , ResourceLinkType t = RLT_Local_Relative )\
    {\
        m##TYPE[key] = new ResourceLinkT< TYPE >( r , t );\
    }\
    TYPE* get##TYPE( KEY key ) const\
    {\
        auto it = m##TYPE.find(key);\
        if(it != m##TYPE.end()) return it->second.valid()?it->second->getResource():nullptr;\
        else return nullptr;\
    }\
    ResourceLinkType getLinkType( KEY key ) const\
    {\
        auto it = m##TYPE.find(key);\
        if(it != m##TYPE.end()) return it->second.valid()?it->second->getLinkType():RLT_Null;\
        else return RLT_Null;\
    }\
    unsigned int getNum##TYPE() const { return m##TYPE.size(); }\
    std::vector<KEY> getKeys##TYPE() const\
    {\
        std::vector<KEY> ret;\
        for( auto it = m##TYPE.begin() ; it != m##TYPE.end(); it++)\
        {\
            ret.push_back(it->first);\
        }\
        return ret;\
    }\
private:\
    std::map< KEY , egg::ref_ptr< ResourceLinkT< TYPE > > > m##TYPE;




    class ResourceLink;

    class EGG_ENGINE_EXPORT Resource
        //:public nbBase::Referenced
        :public Object
    {
    public:
        Resource()
        {
            uniform_path.setAbsolutePath(true); //统一路径在包内是绝对路径
            uniform_path.set("null"); //为了避免资源map表识别错误 默认给资源一个名字
        }
        virtual ~Resource(){}

        //存储的文件后缀 必须和CLASS名称一致
        //virtual std::string getExt(){return "resource";}
        virtual std::string ObjectClassName() override {return "resource";}

        //收集已经加载的资源链接
        virtual void collectionResourceObject( std::map< egg::Path , egg::ref_ptr< Resource > >& result){}

        //收集尚未加载的资源路径
        virtual void collectionResourceLink(std::vector< egg::ref_ptr< ResourceLink > >& result){}

        virtual void save(egg::Path filename , std::ostream& os , Serializer* ser ) const override;

        virtual void load(egg::Path filename , std::istream& is , Serializer* ser 
            , ObjectManager* om , egg::Referenced* rmg, bool load_ext) override;

        //获取统一路径
        egg::Path getUniformPath()const { return uniform_path; }

        //设置统一路径 如果结尾不是class后缀就补充上去
        void setUniformPath(const std::string& path);
        void setUniformPath(egg::Path path);

    protected:

        //绝对路径  
        egg::Path uniform_path; 
    };


    enum ResourceLinkType
    {
        RLT_Null             //当前记录为无效
        , RLT_Absolute         //记录为绝对路径
        , RLT_Parent_Relative //记录为父级的相对路径
        , RLT_Local_Relative  //记录为当前目录之下的相对路径
    };


    //资源链接基础类
    class ResourceLink
        :public egg::Referenced
    {
    public:
        ResourceLink(){}
        virtual ~ResourceLink(){}

        virtual egg::Path getPath() = 0;
        virtual void setPath( egg::Path p ) = 0;
        //virtual ResourceLinkType getLinkType() const  = 0;
        //virtual void setLinkType( ResourceLinkType t ) = 0;
        virtual void setRes( Resource* r ) = 0;
        virtual bool isLoaded() const  = 0;
    };


    //资源链接具体类型
    template< class T >
    class ResourceLinkT
        :public ResourceLink
    {
    public:
        ResourceLinkT( T* r = NULL , ResourceLinkType t = RLT_Local_Relative )
        {
            setResource( r , t );
        }
        ResourceLinkT(egg::Path p , ResourceLinkType t = RLT_Local_Relative )
        {
            setPath( p , t );
        }

        bool isLoaded() const override {return res.valid();}

        T* getResource() const { return res.get(); }
        void setResource( T* r = NULL , ResourceLinkType t = RLT_Local_Relative  )
        {
            res = r; type = t; 
            if(r) path = r->getUniformPath();
        }
        
        virtual void setRes( Resource* r ) override
        {
            //setResource( r , type )
            res = dynamic_cast<T*>(r);
            if(r) path = r->getUniformPath();
        }

        egg::Path getPath() override
        {
            if( res.valid() )
            {
                path = res->getUniformPath();
            }
            return path;
        }
        void setPath(egg::Path p ) override
        {
            if( res.valid() )
            {
                res->setUniformPath(p);
            }
            path = p;
        }

        ResourceLinkType getLinkType() const {return type;}
        void setLinkType( ResourceLinkType t ) { type = t; }

    private:
        /*
            因为资源和链接是互相转换和依赖的关系 
            所以不允许任何人直接访问这些属性 完全依靠setget来操作
        */
        egg::ref_ptr< T > res;       //实际链接对象
        ResourceLinkType type;          //记录类型
        egg::Path path;              //路径
    };


    //加载资源链接本身
    //   filename 是引用这个链接的文件路径，也就是包含链接的文件地址。
    //   resource_link 资源链接内存对象
    template< class T >
    void loadResourceLink(egg::Path filename , ResourceLinkT<T>* resource_link
        , std::istream& is , Serializer* ser )
    {
        //读引用类型
        int t = 0;
        ser->readNumber( is , t);
        resource_link->setLinkType((ResourceLinkType)t);

        //如果为无效 就放弃下面的操作
        if( resource_link->getLinkType() == RLT_Null ) return;

        //读引用路径
        std::string name;
        ser->readString(is,name);

        //计算资源相对关系
        filename.pop();
        filename.push(name);

        //设置路径
        resource_link->setPath(filename);
    }

    template< class T >
    //void saveResourceLink( const ResourceLink<T>* resource_link , std::ostream& os , Serializer* ser )/* const*/
    void saveResourceLink( const ResourceLinkT<T>* resource_link , egg::Path this_path
        , std::ostream& os , Serializer* ser )
    {
        //写引用类型
        if( resource_link )
        {
            ser->writeNumber(os,(int)resource_link->getLinkType());
        }
        else
        {
            ser->writeNumber(os,(int)RLT_Null); //写入无效类型 直接退出
            return;
        }

        //如果为无效 就放弃下面的操作
        if( resource_link->getLinkType() == RLT_Null) return;

        //计算引用资源的路径 如果res不存在就用path字段
        ResourceLinkT<T>* rl = const_cast<ResourceLinkT<T>*>(resource_link); //因为getPath不是const 所以强制转换
        egg::Path path = rl->getPath();
        if (resource_link->getLinkType() == egg::RLT_Parent_Relative
            || resource_link->getLinkType() == egg::RLT_Local_Relative )
        {
            path = egg::Path::relative(
                //this->uniform_path.getPathMel(false,true).c_str() 
                this_path.getPathMel(false,true).c_str() 
                , path.getPathMel(false,true).c_str()
                ,true );
        }
        ser->writeString(os,path.getPathMel());
    }


    //收集场景内资源
    class nv_collectionResourceObject
        :public NodeVisitor
    {
    public:
        nv_collectionResourceObject(std::map< egg::Path , egg::ref_ptr< Resource > >& r):result(r){}
        virtual ~nv_collectionResourceObject(){}
        virtual void apply(egg::Node& node) override{ traverse(node); }

        std::map< egg::Path , egg::ref_ptr< Resource > >& result;
    };

    //收集场景内资源链接
    class nv_collectionResourceLink
        :public NodeVisitor
    {
    public:
        nv_collectionResourceLink(std::vector< egg::ref_ptr< ResourceLink > >& r):result(r){}
        virtual ~nv_collectionResourceLink(){}
        virtual void apply(egg::Node& node) override{ traverse(node); }

        std::vector< egg::ref_ptr< ResourceLink > >& result;
    };


}//namespace egg

#endif//NIUBI_SG_RESOURCE_H
