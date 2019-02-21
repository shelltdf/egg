
#ifndef _CLR_ResultStack_H_
#define _CLR_ResultStack_H_

#include <egg/Export.h>

#include <stack>

#include <egg/Object.h>


namespace egg
{

    //选择堆栈
    //存储创建对象后存储临时变量
    class EGG_RUNTIME_EXPORT ResultStack
    {
    public:
        ResultStack() {}
        ~ResultStack() {}

        void setCurrent(Object* object) { mCurrentObject = object; }
        Object* getCurrent() { return mCurrentObject; }

        //获取不同通道的top值
        Object* top(int channel = 0)
        {
            return mStack.top();
        }

        //入栈当前值
        void push(int channel = 0)
        {
            mStack.push(mCurrentObject);
        }

        //出栈当前值
        void pop(int channel = 0)
        {
            mCurrentObject = mStack.top();
            mStack.pop();
        }

        //清理
        void clear()
        {
            //for (size_t i = 0; i < mStack.size(); i++)
            {
                while (mStack.size() > 0)
                {
                    mStack.pop();
                }
            }
        }

        void setCurrentAttr(Object* object, std::string name)
        {
            mSelectAttrObject = object;
            mSelectAttrName = name;
        }
        Object* getCurrentAttrObject() { return mSelectAttrObject; }
        std::string getSelectAttrName() { return mSelectAttrName; }

    private:

        //当前object对象
        Object* mCurrentObject = 0;

        //object堆栈
        //std::vector< std::stack<CLROBJECT*> > mStack;
        std::stack<Object*> mStack;

        //当前选择属性
        Object* mSelectAttrObject = 0;
        std::string mSelectAttrName;

    };

}//namespace egg

#endif // !_CLR_ResultStack_H_

