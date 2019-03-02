#ifndef _EGG_EDITOR_H_
#define _EGG_EDITOR_H_

#include <eggEditor/Export.h>

#include <egg/Singleton.h>
#include <egg/Object.h>

#include <eggEditor/Panel.h>


namespace egg
{

    class EGG_EDITOR_EXPORT EventCallBack
        :public Object
    {
    public:

        EventCallBack() {}
        virtual ~EventCallBack() {}

        virtual void start() = 0;
        virtual bool step(double dt_second) = 0;
        virtual void exit() = 0;

        virtual void showPanel(const std::string& type, Panel* panel) = 0;
        virtual void hidePanel(const std::string& type, Panel* panel) = 0;
    };

    class EGG_EDITOR_EXPORT Editor
        :public Object
    {
    public:

        Editor();
        virtual ~Editor();

        DECLARE_SINGLETON(Editor)

        void start();
        bool step(double dt_second);
        void exit();

        void showPanel(const std::string& type);
        void hidePanel(const std::string& type);

        void setEventCallBack(EventCallBack* cb) { mEventCallBack = cb; }

    private:

        ref_ptr<EventCallBack> mEventCallBack;
    };
}

#endif // !_EGG_EDITOR_H_

