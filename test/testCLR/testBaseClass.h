#ifndef _TEST_BASE_CLASS_H_
#define _TEST_BASE_CLASS_H_

#include <set>
#include <vector>
#include <map>


enum OBJECTTYPE
{
    OC_AAAA = 0
    , OC_BBBB
    , OC_CCCC
};

class BBBB;

class AAAA
    :public egg::Object
{
public:
    AAAA() 
    {
        type = OC_AAAA;
    
        //ref = new AAAA();
    }
    ~AAAA() {}

    //值类型
    int id = 0;
    std::string name;

    //枚举类型
    OBJECTTYPE type;

    //索引容器
    std::vector<int> nums_vector;
    std::map<int, int> nums_map;
    std::vector<AAAA*> children;
    //std::map<std::string, CLRPTR<AAAA> > library;
    std::map<std::string,BBBB*> library;


    //无索引容器
    std::set<int> nums_set;

    //对象引用类型
    AAAA* ref = 0;


    //函数
    void exec()
    {
    }

private:

};

class BBBB
    :public AAAA
{
public:
    BBBB() { type = OC_BBBB; }
    ~BBBB() {}

    std::string nameB;


    void exec()
    {
    }
private:

};

class CCCC
    :public BBBB
{
public:
    CCCC() { type = OC_CCCC; }
    ~CCCC() {}

    std::string nameC;


private:
    void exec()
    {
    }
};


#endif // !_TEST_BASE_CLASS_H_

