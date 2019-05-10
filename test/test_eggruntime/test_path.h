
#include <egg/path.h>
#include <egg/Platform.h>
#include <egg/Runtime.h>

#include <egg/FileDirUtil.h>

TEST(path, AbsolutePath)
{
    egg::Path path1("c:/aaaaa"); //windows 
    egg::Path path2("/aaaaa");   //unix
    egg::Path path3("|aaaaa");   //mel
    EXPECT_TRUE(path1.isAbsolutePath());
    EXPECT_TRUE(path2.isAbsolutePath());
    EXPECT_TRUE(path3.isAbsolutePath());


    egg::Path path11("aaaaa");
    egg::Path path22("aaaaa");
    egg::Path path33("aaaaa");
    egg::Path path111("./aaaaa");
    egg::Path path222("./aaaaa");
    egg::Path path333(".|aaaaa");
    egg::Path path1111("../aaaaa");
    egg::Path path2222("../aaaaa");
    egg::Path path3333("..|aaaaa");
    EXPECT_FALSE(path11.isAbsolutePath());
    EXPECT_FALSE(path22.isAbsolutePath());
    EXPECT_FALSE(path33.isAbsolutePath());
    EXPECT_FALSE(path111.isAbsolutePath());
    EXPECT_FALSE(path222.isAbsolutePath());
    EXPECT_FALSE(path333.isAbsolutePath());
    EXPECT_FALSE(path1111.isAbsolutePath());
    EXPECT_FALSE(path2222.isAbsolutePath());
    EXPECT_FALSE(path3333.isAbsolutePath());


    path11.setAbsolutePath(true);
    path22.setAbsolutePath(true);
    path33.setAbsolutePath(true);
    EXPECT_TRUE(path11.isAbsolutePath());
    EXPECT_TRUE(path22.isAbsolutePath());
    EXPECT_TRUE(path33.isAbsolutePath());

}

TEST(path, size)
{
    egg::Path path1("aaaaa");
    EXPECT_EQ(1, path1.size());

    path1.set("aaa/bbb/ccc");
    EXPECT_EQ(3, path1.size());

    path1.set("/");
    EXPECT_EQ(0, path1.size());

}


TEST(path, push_pop)
{
    egg::Path path1("aaa");
    path1.push("bbb");
    path1.push("ccc");
    EXPECT_EQ(3, path1.size());

    path1.pop();
    EXPECT_EQ(2, path1.size());

    path1.pop();
    EXPECT_EQ(1, path1.size());

    path1.pop();
    EXPECT_EQ(0, path1.size());

}


TEST(smart_pointer, string)
{
    egg::Path path1("c:/aaa");
    EXPECT_STRCASEEQ("c:\\aaa", path1.getPathWindows().c_str());
    EXPECT_STRCASEEQ("/c/aaa", path1.getPathUnix().c_str());
    EXPECT_STRCASEEQ("|c|aaa", path1.getPathMel().c_str());

}

TEST(smart_pointer, relative)
{


}

//移植过来的测试
//void testPath()
TEST(path, testPath)
{
    egg::Path p1("|aaaa");
    egg::Path p2("|bbbb");
    egg::Path p3("|aa|bb|cc");
    egg::Path p4("|aa|ee|qq|bb");

    //计算相对路径
    egg::Path p = egg::Path::relative(p3, p4,false);
    std::string str = p.getPathMel();

    p4.pop();
    p4.push("../"); //有bug
    p4.push("../"); //有bug
    p4.push("../"); //有bug
    p4.push("../"); //有bug
    str = p4.getPathMel();
}

//移植过来的测试
//测试路径计算
void test_path()
{
    egg::ref_ptr<egg::Runtime> runtime = egg::Runtime::getInstance(true);
    egg::Module* module = runtime->requestModule("Platform", "win32");
    egg::Platform* platform = egg::Platform::getInstance();

    //路径存在
    printf("%d\n", platform->isExist("testfs"));
    printf("%d\n", platform->isExist("testfs2"));

    //文件存在
    printf("%d\n", platform->isExist("testfs/aa.txt"));
    printf("%d\n", platform->isExist("testfs/aa2.txt"));

    //列举文件
    std::vector<std::string> l = platform->listFile("testfs", "ddr", true);
    for (unsigned int i = 0; i < l.size(); i++)
    {
        printf("%s\n", l[i].c_str());
    }


    //计算相对路径
    egg::Path p1("|aaaa");
    egg::Path p2("|bbbb");
    egg::Path p3("|aa|bb");
    egg::Path p4("|aa|ee");

    std::string str;

    str = egg::Path::relative(p1, p2,true).getPathMel();
    str = egg::Path::relative(p2, p3, true).getPathMel();
    str = egg::Path::relative(p3, p4, false).getPathMel();

    str = egg::Path::relative(p1, p3, true).getPathMel();
    str = egg::Path::relative(p1, p4, true).getPathMel();

    str = egg::Path::relative(p2, p4, true).getPathMel();


    str = egg::Path::relative(p2, p1, true).getPathMel();
    str = egg::Path::relative(p3, p2, false).getPathMel();
    str = egg::Path::relative(p4, p3, false).getPathMel();

    str = egg::Path::relative(p3, p1, false).getPathMel();
    str = egg::Path::relative(p4, p1, false).getPathMel();

    str = egg::Path::relative(p4, p2, false).getPathMel();


    //测试本地转换
    str = egg::Path("|").getPathMel();
    str = egg::Path("|").getPathWindows();
    str = egg::Path("|").getPathUnix();


    //测试mel接口
    str = egg::Path("|aaaaaaaaa").getPathMel(true, true);

}

TEST(path, filedirutil)
{
	std::vector<std::string> files = getFilesInDir("d:/");


}