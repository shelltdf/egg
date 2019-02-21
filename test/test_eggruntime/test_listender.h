

#include <egg/Object.h>

class myObject
    :public egg::Object
{
public:

    myObject(std::string name = "object")
        :egg::Object(name)
    {
    }
    virtual ~myObject()
    {
    }

    virtual void onEvent(Object* from, Object* to, egg::Event* event)
    {
        num++;
    }

    int num = 0;
};

TEST(Runtime, Listender)
{
    myObject* sender = new myObject();

    myObject* listener0 = new myObject();
    myObject* listener1 = new myObject();

    sender->addListender(listener0);
    sender->addListender(listener1);

    sender->fireEvent(new egg::Event());

    EXPECT_EQ(listener0->num, 1);
    EXPECT_EQ(listener1->num, 1);

}

