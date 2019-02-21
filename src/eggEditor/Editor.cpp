
#include <eggEditor/Editor.h>

using namespace egg;

IMPLEMENT_SINGLETON(Editor)

Editor::Editor()
{
}

Editor::~Editor()
{
}

void Editor::start()
{
    if (mEventCallBack.valid()) mEventCallBack->start();
}

bool Editor::step(double dt_second)
{
    bool s = true;
    if (mEventCallBack.valid()) s = mEventCallBack->step(dt_second);


    return s;
}

void Editor::exit()
{
    if (mEventCallBack.valid()) mEventCallBack->exit();

}


void Editor::showPanel(const std::string& type)
{

    //建立panel
    Panel* panel = new Panel();

    //执行回调
    if (mEventCallBack.valid()) mEventCallBack->showPanel(type, panel);
}

void Editor::hidePanel(const std::string& type)
{
    Panel* panel = 0;

    if (mEventCallBack.valid()) mEventCallBack->hidePanel(type, panel);
}
