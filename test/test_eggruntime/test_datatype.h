

//#include <eggRuntime/Runtime.h>

#if 0
TEST(Runtime, DataType)
{
    //定常类型
    EXPECT_EQ(sizeof(bool), 4);
    EXPECT_EQ(sizeof(char), 1);
    EXPECT_EQ(sizeof(unsigned char), 1);
    EXPECT_EQ(sizeof(short), 2);
    EXPECT_EQ(sizeof(unsigned short), 2);
    EXPECT_EQ(sizeof(int), 4);
    EXPECT_EQ(sizeof(unsigned int), 4);
    EXPECT_EQ(sizeof(long), 4);
    EXPECT_EQ(sizeof(unsigned long), 4);
    EXPECT_EQ(sizeof(long long), 4);
    EXPECT_EQ(sizeof(unsigned long long), 4);

    //变长类型
    EXPECT_EQ(sizeof(size_t), 4);
    EXPECT_EQ(sizeof(intptr_t), 4);


}
#endif

