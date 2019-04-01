
#include <egg/ref_ptr.h>
#include <egg/Object.h>

#include <vector>
#include <set>
#include <map>
#include <string>


static int count = 0;
class MyClass
    :public egg::Referenced
{
public:
    MyClass() { count++; }
    virtual ~MyClass() { count--; }
private:
};

class MyClassA
    :public MyClass
{
public:
    MyClassA() {  }
    virtual ~MyClassA() {  }
private:
};

class MyClassB
    :public MyClass
{
public:
    MyClassB() {  }
    virtual ~MyClassB() {  }
private:
};

class MyObject
    :public egg::Object
{
public:
    MyObject() { count++; }
    virtual ~MyObject() { count--; }
private:
};


TEST(smart_pointer, ref_ptr)
{
    egg::ref_ptr<MyClass> mc1 = new MyClass();
    EXPECT_EQ(1, mc1->getCount());

    egg::ref_ptr<MyClass> mc2 = mc1;
    EXPECT_EQ(2, mc1->getCount());

    mc2 = 0;
    EXPECT_EQ(1, mc1->getCount());

    mc1 = 0;
    EXPECT_EQ(0, count);
}

TEST(smart_pointer, observer_ptr)
{
    egg::ref_ptr<MyClass> mc1 = new MyClass();
    EXPECT_EQ(1, mc1->getCount());

    egg::observer_ptr<MyClass> mc2 = mc1;
    EXPECT_TRUE(mc2.get().get() > 0);

    mc1 = 0;
    EXPECT_TRUE(mc2.get().get() == 0);

    //释放
    mc1 = 0;
    mc2 = 0;
    EXPECT_EQ(0, count);
}

TEST(smart_pointer, observer_ptr2)
{
    egg::observer_ptr<MyClass> mc2 = new MyClass();
    egg::ref_ptr<MyClass> mc22 = mc2.get();
    EXPECT_TRUE(mc22.get() > 0);
    mc22.release();

    egg::ref_ptr<MyClass> mc1 = mc2.get();
    EXPECT_EQ(1, mc1->getCount());

    mc1 = 0;
    EXPECT_TRUE(mc2.get().get() == 0);


    //释放
    mc2 = 0;
    mc22 = 0;
    mc1 = 0;
    EXPECT_EQ(0, count);

}


TEST(smart_pointer, container)
{
    std::vector< egg::ref_ptr<MyClass> > mc1;
    std::set< egg::ref_ptr<MyClass> > mc2;
    std::map<std::string, egg::ref_ptr<MyClass> > mc3;

    std::vector< egg::observer_ptr<MyClass> > wc1;
    std::set< egg::observer_ptr<MyClass> > wc2;
    std::map<std::string, egg::observer_ptr<MyClass> > wc3;

    egg::ref_ptr<MyClass> p = new MyClass();

    mc1.push_back(p);
    mc2.insert(p);
    mc3["a"] = p;
    mc3.insert(std::pair<std::string, egg::ref_ptr<MyClass>>("b", p));

    wc1.push_back(p);
    wc2.insert(p);
    wc3["a"] = p;
    wc3.insert(std::pair<std::string, egg::observer_ptr<MyClass>>("b", p));


    //释放
    mc1.clear();
    mc2.clear();
    mc3.clear();
    wc1.clear();
    wc2.clear();
    wc3.clear();
    p = 0;
    EXPECT_EQ(0, count);

}

TEST(smart_pointer, assign)
{
    egg::ref_ptr<MyClass> p1 = new MyClass();
    egg::ref_ptr<MyClass> p2 = new MyClass();

    p1 = p2;
    p2 = p1;

    egg::observer_ptr<MyClass> w1 = new MyClass();
    egg::observer_ptr<MyClass> w2 = new MyClass();

    //这里交换会出现问题 没有任何强指针的对象 是无法被释放的
    //w1 = w2;

    p1 = w1.get();
    p2 = w2.get();


    //释放
    p1 = 0;
    p2 = 0;
    w1 = 0;
    w2 = 0;
    EXPECT_EQ(0, count);
}


TEST(smart_pointer, base)
{
    egg::ref_ptr<MyClassA> p1 = new MyClassA();
    egg::ref_ptr<MyClassB> p2 = new MyClassB();

    std::map<std::string, egg::ref_ptr<MyClass> > list;

    list["a"] = p1.get();
    list["b"] = p2.get();

    p1 = 0;
    p2 = 0;

    auto it = list.find("a");
    if (it != list.end())
    {
        list.erase(it);
    }
    EXPECT_EQ(1, count);

    //释放
    list.clear();
    EXPECT_EQ(0, count);
}


TEST(smart_pointer, object)
{
    egg::ref_ptr<MyObject> p1 = new MyObject();



}