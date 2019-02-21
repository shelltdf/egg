
#include <gtest/gtest.h>

#include "test_datatype.h"
#include "test_smart_pointer.h"
#include "test_path.h"
#include "test_serializer.h"
#include "test_module.h"
#include "test_node.h"
#include "test_listender.h"


/*
int Add(int x, int y) { return x + y; }
TEST(StringCmpTest, Demo)
{
    EXPECT_EQ(3, Add(1, 2));
}
*/

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

