#ifndef _CLR_History_H_
#define _CLR_History_H_

#include <egg/Export.h>

//#include <fstream>
//#include <string>
//#include <vector>
//#include <map>
//
//#include <egg/Reflection/ValueHandle.h>
//#include <egg/Reflection/ObjectHandle.h>

#include <egg/Command/Command.h>

namespace egg
{
 
    //ÀúÊ·¼ÇÂ¼
    class EGG_RUNTIME_EXPORT History
    {
    public:
        History() :mCommand(0) {}
        ~History() {}

        Command* mCommand;
        std::vector<std::string> mParameter;
        std::string mResult;
    };



}//namespace egg

#endif // !_CLR_History_H_
