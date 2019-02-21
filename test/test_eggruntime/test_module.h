

#include <egg/Runtime.h>

#if 0
TEST(Runtime, Module)
{
    egg::ref_ptr<egg::Runtime> runtime = egg::Runtime::getInstance(true);

    egg::Module* module = runtime->requestModule("Platform", "win32");
    EXPECT_TRUE(module);

}
#endif

