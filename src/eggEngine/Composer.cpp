
#include <eggEngine/Composer.h>

using namespace egg;

Composer::Composer()
{
}

Composer::~Composer()
{
}

void Composer::save(Path filename, std::ostream& os, Serializer* ser) const
{
    //–¥»Îxml

}

void Composer::load(Path filename, std::istream& is, Serializer* ser, ObjectManager* om
    , Referenced* rmg, bool load_ext)
{
    //∂¡XML

    std::string element_name = "";

    if (element_name == "render_pass")
    {
    }
    if (element_name == "render_target")
    {
    }

}

void Composer::addStandardPass(std::string pass_name)
{
    mPasses[pass_name] = new StandardRenderPass();
}
void Composer::addImagePass(std::string pass_name)
{
    mPasses[pass_name] = new ImageRenderPass();
}

void Composer::addRengerTarget(std::string target_name)
{
}

void Composer::setInputSceneNode(std::string pass_name, std::string node_path)
{
}

void Composer::setInputCamera(std::string pass_name, std::string node_path)
{
}

void Composer::setRengerTarget(std::string pass_name, std::string target_name)
{
}


