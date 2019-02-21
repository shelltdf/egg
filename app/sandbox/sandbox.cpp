
#include <eggEditor/Editor.h>
#include <eggRuntime/Module.h>
#include <eggRuntime/Runtime.h>



int main(int argc, char* argv[])
{
    std::string app_name = "qt";
    if (argc > 1) { app_name = argv[1]; }
    std::string app_name2 = "Editor_" + app_name;

    egg::ref_ptr<egg::Runtime> runtime = egg::Runtime::getInstance(true);
    egg::ref_ptr<egg::Module> module = runtime->requestModule("Editor", app_name);

    egg::ref_ptr<egg::Editor> editor = egg::Editor::getInstance();
    editor->start();

    bool isRun = true;
    while (isRun)
    {
        isRun = editor->step(1.0 / 60.0);
    }

    editor->exit();

    module = 0;
    editor = 0;
    runtime = 0;

    return 0;
}

