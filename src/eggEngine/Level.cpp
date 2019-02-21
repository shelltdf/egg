
#include <eggEngine/Level.h>

using namespace egg;

void Level::save(Path filename, std::ostream& os, Serializer* ser) const
{
    Resource::save(filename, os, ser);

}

void Level::load(Path filename, std::istream& is, Serializer* ser
    , ObjectManager* om, Referenced* rmg, bool load_ext)
{
    Resource::load(filename, is, ser, om, rmg, load_ext);

}

