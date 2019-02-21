
#include <eggEditor/Editor.h>
#include <eggRuntime/Module.h>

#include <QApplication>
#include <QFrame>
#include <QMainWindow>
#include <QStatusBar>


class Editor_qt_EventCallBack
    :public egg::EventCallBack
{
public:

    Editor_qt_EventCallBack::Editor_qt_EventCallBack()
    {
    }

    virtual Editor_qt_EventCallBack::~Editor_qt_EventCallBack()
    {
    }

    virtual void start()
    {
        std::cout << "start qt" << std::endl;

        int argc = 0;
        app = new QApplication(argc, 0);

        //QFrame* frame = new QFrame();
        //frame->show();

        /*QMainWindow**/ win = new QMainWindow();
        win->setStatusBar(new QStatusBar());
        win->setMinimumSize(800, 600);
        win->show();

        //ÇëÇó½¨Á¢panel
        egg::Editor* editor = egg::Editor::getInstance();
        editor->showPanel("menu");

    }

    virtual bool step(double dt_second)
    {
        //std::cout << "step qt" << std::endl;
        app->processEvents();

        //bool all_closed = true;
        //QWidgetList list = QApplication::allWidgets();
        //for (size_t i = 0; i < list.size(); i++)
        //{
        //    if (!list[i]->isHidden()) all_closed = false;
        //}
        //QWidgetList list2 = QApplication::topLevelWidgets();
        //for (size_t i = 0; i < list2.size(); i++)
        //{
        //    if (!list2[i]->isHidden()) all_closed = false;
        //}
        //if (all_closed) { return false; }

        if (win) return !win->isHidden();
        return true;
    }

    virtual void exit()
    {
        std::cout << "exit qt" << std::endl;

        if (app) delete app;
        app = 0;
    }

    virtual void showPanel(const std::string& type, egg::Panel* panel)
    {
        if (type == "menu")
        {

        }
    }

    virtual void hidePanel(const std::string& type, egg::Panel* panel)
    {
    }

    QApplication* app = 0;
    QMainWindow* win = 0;
};


INSTALL_MODULE(Editor_qt)
{
    egg::Editor::getInstance(true)
        ->setEventCallBack(new Editor_qt_EventCallBack());
    return true;
}

UNINSTALL_MODULE(Editor_qt)
{
    egg::Editor::getInstance(true)->setEventCallBack(0);
    return true;
}
