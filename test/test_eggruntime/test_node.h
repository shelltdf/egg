

//#include <egg/Runtime.h>
#include <egg/Coord.h>
#include <egg/Node.h>
#include <egg/NodeVisitor.h>


class TestNode
    :public egg::Node
{
public:

    TestNode(const std::string& name = "testnode")
        :egg::Node(name) {}
    virtual ~TestNode() {}

    int num = 0;
};

class TestNodeVisitor
    :public egg::NodeVisitor
{
public:
    TestNodeVisitor() {}
    virtual ~TestNodeVisitor() {}

    virtual void apply(egg::Node& node)
    {
        TestNode* testnode = dynamic_cast<TestNode*>(&node);
        if (testnode)
        {
            testnode->num++;
        }

        traverse(node);
    }
};


TEST(Runtime, Node)
{
    TestNode* node_p0 = new TestNode("node_p0");
    TestNode* node_p1 = new TestNode("node_p1");

    TestNode* node_c0 = new TestNode("node_c0");
    TestNode* node_c1 = new TestNode("node_c1");

    node_p0->addChild(node_c0);
    node_p1->addChild(node_c1);

    TestNodeVisitor* vistor = new TestNodeVisitor();
    node_p0->accept(*vistor);
    node_p1->accept(*vistor);
    EXPECT_EQ(node_p0->num, 1);
    EXPECT_EQ(node_p1->num, 1);
    EXPECT_EQ(node_c0->num, 1);
    EXPECT_EQ(node_c1->num, 1);

    node_c0->accept(*vistor);
    node_c1->accept(*vistor);
    EXPECT_EQ(node_c0->num, 2);
    EXPECT_EQ(node_c1->num, 2);

}

TEST(Runtime, Coord)
{
    egg::Coord coord(10, 1);

    EXPECT_EQ(coord.getAbs(), 10);
    EXPECT_EQ(coord.getRel(), 1);

    EXPECT_EQ(coord.getCoord(100), 110);
}



class TestCoord2DNode
    :public egg::Node
    ,public egg::Coord2DInterface
{
public:

    TestCoord2DNode(const std::string& name = "TestCoord2DNode")
        :egg::Node(name) {}
    virtual ~TestCoord2DNode() {}

    virtual void onShape(float l, float t, float r, float b, float rot) override
    {
        ml = l;
        mt = t;
        mr = r;
        mb = b;
        mrot = rot;
    }

    float ml = 0;
    float mt = 0;
    float mr = 0;
    float mb = 0;
    float mrot = 0;
};


TEST(Runtime, Coord2DNode)
{
    TestCoord2DNode* node_p0 = new TestCoord2DNode("node_p0");
    node_p0->getCoord2D().mX = egg::Coord(0, 0);
    node_p0->getCoord2D().mY = egg::Coord(0, 0);
    node_p0->getCoord2D().mW = egg::Coord(100, 0);
    node_p0->getCoord2D().mH = egg::Coord(100, 0);
    node_p0->getCoord2D().mCX = egg::Coord(0, 0);
    node_p0->getCoord2D().mCY = egg::Coord(0, 0);


    TestCoord2DNode* node_c0 = new TestCoord2DNode("node_c0");
    TestCoord2DNode* node_c1 = new TestCoord2DNode("node_c1");
    TestCoord2DNode* node_c2 = new TestCoord2DNode("node_c2");
    TestCoord2DNode* node_c3 = new TestCoord2DNode("node_c3");
    node_p0->addChild(node_c0);
    node_p0->addChild(node_c1);
    node_p0->addChild(node_c2);
    node_p0->addChild(node_c3);


    //左上
    node_c0->getCoord2D().mX = egg::Coord(0, 0);
    node_c0->getCoord2D().mY = egg::Coord(0, 0);
    node_c0->getCoord2D().mW = egg::Coord(10, 0);
    node_c0->getCoord2D().mH = egg::Coord(10, 0);
    node_c0->getCoord2D().mCX = egg::Coord(0, 0);
    node_c0->getCoord2D().mCY = egg::Coord(0, 0);

    //右上
    node_c1->getCoord2D().mX = egg::Coord(0, 0);
    node_c1->getCoord2D().mY = egg::Coord(0, 0);
    node_c1->getCoord2D().mW = egg::Coord(10, 0);
    node_c1->getCoord2D().mH = egg::Coord(10, 0);
    node_c1->getCoord2D().mCX = egg::Coord(0, 0);
    node_c1->getCoord2D().mCY = egg::Coord(0, 0);

    //左下
    node_c2->getCoord2D().mX = egg::Coord(0, 0);
    node_c2->getCoord2D().mY = egg::Coord(0, 0);
    node_c2->getCoord2D().mW = egg::Coord(10, 0);
    node_c2->getCoord2D().mH = egg::Coord(10, 0);
    node_c2->getCoord2D().mCX = egg::Coord(0, 0);
    node_c2->getCoord2D().mCY = egg::Coord(0, 0);

    //右下
    node_c3->getCoord2D().mX = egg::Coord(0, 0);
    node_c3->getCoord2D().mY = egg::Coord(0, 0);
    node_c3->getCoord2D().mW = egg::Coord(10, 0);
    node_c3->getCoord2D().mH = egg::Coord(10, 0);
    node_c3->getCoord2D().mCX = egg::Coord(0, 0);
    node_c3->getCoord2D().mCY = egg::Coord(0, 0);


    //遍历
    egg::Coord2DVisitor* visitor = new egg::Coord2DVisitor();
    node_p0->accept(*visitor);


    //
    EXPECT_EQ(node_c0->ml, 5);
    EXPECT_EQ(node_c0->mt, 5);
    EXPECT_EQ(node_c0->mr, 5);
    EXPECT_EQ(node_c0->mb, 5);
    EXPECT_EQ(node_c0->mrot, 0);
    
    EXPECT_EQ(node_c1->ml, 5);
    EXPECT_EQ(node_c1->mt, 5);
    EXPECT_EQ(node_c1->mr, 5);
    EXPECT_EQ(node_c1->mb, 5);
    EXPECT_EQ(node_c1->mrot, 0);

    EXPECT_EQ(node_c2->ml, 5);
    EXPECT_EQ(node_c2->mt, 5);
    EXPECT_EQ(node_c2->mr, 5);
    EXPECT_EQ(node_c2->mb, 5);
    EXPECT_EQ(node_c2->mrot, 0);

    EXPECT_EQ(node_c3->ml, 5);
    EXPECT_EQ(node_c3->mt, 5);
    EXPECT_EQ(node_c3->mr, 5);
    EXPECT_EQ(node_c3->mb, 5);
    EXPECT_EQ(node_c3->mrot, 0);

}

