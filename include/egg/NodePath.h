#ifndef _NSG_CORE_CAPABILITY_H_
#define _NSG_CORE_CAPABILITY_H_

#include <egg/Node.h>

#include <vector>

namespace egg
{

    //½ÚµãÂ·¾¶
    class NodePath
    {
    public:
        NodePath(){}
        ~NodePath(){}

        std::string toString()
        {
            std::string ret = "";
            for (size_t i = 0; i < path.size(); i++)
            {
                ret += "|";
                ret += path[i]->getName();
            }
            return ret;
        }

        std::vector< Node* > path;
    };

}//namespace nbSG
   
#endif //_NSG_CORE_CAPABILITY_H_
