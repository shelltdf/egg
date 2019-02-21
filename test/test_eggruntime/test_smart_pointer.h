
#include <egg/ref_ptr.h>

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
    ~MyClass() { count--; }
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

}

TEST(smart_pointer, assign)
{
    egg::ref_ptr<MyClass> p1 = new MyClass();
    egg::ref_ptr<MyClass> p2 = new MyClass();

    p1 = p2;
    p2 = p1;

    egg::observer_ptr<MyClass> w1 = new MyClass();
    egg::observer_ptr<MyClass> w2 = new MyClass();

    w1 = w2;

    p1 = w1.get();
    p2 = w2.get();

}


