
#include <eggEngine/Texture.h>

using namespace egg;


void Texture::save(egg::Path filename, std::ostream& os, Serializer* ser) const
{
    Resource::save(filename, os, ser);

    saveResourceLink<Image>(mImage.get(), this->uniform_path, os, ser);
}

void Texture::load(egg::Path filename, std::istream& is, Serializer* ser, ObjectManager* om
    , egg::Referenced* rmg, bool load_ext)
{
    Resource::load(filename, is, ser, om, rmg, load_ext);

    mImage = new ResourceLinkT< Image >();
    loadResourceLink<Image>(filename, mImage.get(), is, ser);
}

//收集已经加载的资源链接
void Texture::collectionResourceObject(std::map< egg::Path, egg::ref_ptr< Resource > >& result)
{
    if (mImage.valid() && mImage->isLoaded())
    {
        result[mImage->getResource()->getUniformPath()] = mImage->getResource();
    }
}

//收集尚未加载的资源路径
void Texture::collectionResourceLink(std::vector< egg::ref_ptr< ResourceLink > >& result)
{
    if (mImage.valid() && !mImage->isLoaded())
    {
        result.push_back(mImage.get());
    }
}

