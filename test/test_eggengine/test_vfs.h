
#include <eggEngine/VirtualFileSystem.h>

TEST(eggEngine, VirtualFileSystem)
{


}

//移植过来的测试
//测试建立过程
void test_fs_native(const char* archive_filename, const char* archive_ext)
{
    //文件系统
    egg::FileSystem fs;

    //注册一个包
    egg::Archive* a = fs.newArchive(archive_ext);

    //打开包
    a->open(archive_filename);
    if (a->isOpen())
    {
        printf("archive is open\n");
    }
    else
    {
        printf("archive is close\n");
    }


    //新建目录
    a->newEntryAsDir("ddr");
    a->newEntryAsDir("ddr2");
    a->newEntryAsDir("a|b|c|d|e");

    //新建文件
    a->setEntryAsFile("qq.txt", "1122334455", 10, true);
    a->setEntryAsFile("qq2.txt", "1122334455", 10, true);
    a->setEntryAsFile("a|b|c|d|e|qq3.txt", "1122334455", 10, true);

    //删除目录
    //a->delEntry("ddr");
    a->delEntry("ddr2");

    //删除文件
    //a->delEntry("qq.txt");
    a->delEntry("qq2.txt");


    //打开对象
    egg::Entry* ent = a->getEntry("aa.txt");
    if (ent)
    {
        //读文件信息
        if (ent->isFile())
        {
            printf("is file\n");
        }
        if (ent->isDir())
        {
            printf("is dir\n");
        }
        printf("name %s\n", ent->getName().c_str());
        printf("len  %d\n", ent->getLen());

        //读文件数据
        std::string str;
        str.resize(ent->getLen() + 1);
        str[ent->getLen()] = '\0';
        ent->read(&str[0]);
        printf("%s\n", str.c_str());
    }
    else
    {
        printf("file not exist\n");
    }



    //关闭
    a->close();

}

//移植过来的测试
//测试查询功能
void test_fs_native2(const char* archive_filename, const char* archive_ext)
{
    //文件系统
    egg::FileSystem fs;

    //注册一个包
    egg::Archive* a = fs.newArchive(archive_ext);

    //打开包
    a->open(archive_filename);
    if (a->isOpen())
    {
        printf("archive is open\n");
    }
    else
    {
        printf("archive is close\n");
    }

    //打印文件列表
    std::vector<std::string> l = a->listFile("ddr", true);
    for (unsigned int i = 0; i < l.size(); i++)
    {
        printf("%s\n", l[i].c_str());
    }

}
