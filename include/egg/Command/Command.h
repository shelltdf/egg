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
        Command(const char* name, const char* category = 0)
            :mName(name)
        {
            mCategory = category;
            if (mCategory.size() == 0)
            {
                mCategory = name;
            }
        }

        virtual ~Command() {}

        std::string getName() { return mName; }

        std::string getCategory() { return mCategory; }

        //void pushParameter(std::string parameter)
        //{
        //    mParameter.push_back(parameter);
        //}

    protected:

        virtual std::string exec(Context* context, ParameterList pl) = 0;

        //private:

        std::string mName;
        std::string mCategory;
        //std::vector<std::string> mParameter;
    };


}//namespace egg

#endif // !_CLR_Command_H_
