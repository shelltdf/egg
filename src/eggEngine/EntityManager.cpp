
#include <eggEngine/EntityManager.h>


using namespace egg;

IMPLEMENT_SINGLETON(EntityFactory)

Entity* EntityFactory::createEntity(const std::string& type_name)
{
    auto it = mCrteatorMap.find(type_name);
    if (it != mCrteatorMap.end())
    {
        Entity* ent_new = dynamic_cast<Entity*>(it->second->create());
        return ent_new;
    }
    //else
    //{
    //    std::cout << "entity factory is not exsit" << std::endl;
    //}
}


void Entity::addChild(Entity* child)
{
    mChildren.push_back(child);
}

int Entity::getNumChildren()
{
    return mChildren.size();
}

Entity* Entity::getChild(int index)
{
    if (index< mChildren.size())
    {
        return mChildren[index].get();
    }
    return 0;
}

void Entity::removeChild(Entity* entity)
{
    //auto it = mChildren.find(entity);
    for (int i = 0; i < mChildren.size(); i++)
    {
        if (entity == mChildren[i].get())
        {
            mChildren.erase(mChildren.begin() + i);
            break;
        }
    }
}

void Entity::removeChild(int index)
{
    if (index < mChildren.size())
    {
        mChildren.erase(mChildren.begin() + index);
    }
}

