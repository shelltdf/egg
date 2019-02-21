#ifndef _CLR_Command_H_
#define _CLR_Command_H_

#include <egg/Export.h>

//#include <fstream>
//#include <string>
#include <vector>
//#include <map>

//#include <egg/Reflection/ValueHandle.h>
//#include <egg/Reflection/ObjectHandle.h>


namespace egg
{
    class Context;

    typedef std::vector<std::string> ParameterList;
    typedef std::vector<std::string> CommandData;
    typedef std::vector<CommandData> CommandDataList;

    //√¸¡Ó
    class EGG_RUNTIME_EXPORT Command
    {
        friend class Context;

    public:
        Command(const char* name)
            :nName(name)
        {
        }

        virtual ~Command() {}

        std::string getName() { return nName; }

        //void pushParameter(std::string parameter)
        //{
        //    mParameter.push_back(parameter);
        //}

    protected:

        virtual std::string exec(Context* context, ParameterList pl) = 0;

        //private:

        std::string nName;
        //std::vector<std::string> mParameter;
    };


}//namespace egg

#endif // !_CLR_Command_H_
