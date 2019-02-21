
#include <eggEngine/Geometry.h>

//#include <nbSG/Material.h>
#include <eggEngine/RenderVisitor.h>
//#include <eggEngine/Pass.h>

using namespace egg;

void Geometry::draw(nv_render* nv, int index)
{
    nv->render->bindGeometry(nv->gc, this);
    nv->render->drawGeometry(nv->gc, this, index);
    nv->render->unbindGeometry(nv->gc, this);
}

#if 0
//渲染指定段
void Geometry::renderSubGeometry(nbPlugins::Renderer* render, Material::Pass* p, unsigned int id)
{
    //检查ID
    if (id >= mSubGeometry.size()) return;

    //如果vbo对象为空 就建立他们
    for (auto it_va = mVectexAttribute.begin(); it_va != mVectexAttribute.end(); it_va++)
    {
        auto it_buf = mBuffers.find(it_va->second->buffer_name);
        if(it_buf != mBuffers.end())
        {
            if(mVBOs[it_buf->first] > 0) continue;

#if 1
            //兼容性 把int类型强制转换成float
            //       主要是因为本地模拟器并不支持 ivec4 类型
            if( it_buf->second->getTypeID() == typeid(int).name())
            {
                nbSG::ArrayT<float>* a = new nbSG::ArrayT<float>(it_buf->second->getSize());
                int*   b = (int*)it_buf->second->getData();
                for (unsigned int i = 0; i < a->getSize(); i++)
                {
                    (*a)[i] = (float)b[i];
                }
                mBuffers[it_buf->first] = a;
            }
#endif

            mVBOs[it_buf->first] = render->createGeometry(
                nbPlugins::Renderer::RAW
                , it_buf->second->getSize() * it_buf->second->getSizeOf()
                , it_buf->second->getData()
                );
        }
    }

    //绑定vbo对象
    for (auto it = this->mVBOs.begin(); it != this->mVBOs.end(); it++)
    {
        auto it_va = mVectexAttribute.find(it->first);
        if(it_va == mVectexAttribute.end()) continue;

        VectexAttribute* attr = it_va->second.get();
        Program* prog = p->getProgram();
        if(prog) 
        {
            auto it_prog = prog->mVANameMapping.find(it->first);
            if( it_prog!= prog->mVANameMapping.end() )
            {
                render->bindGeometry(mVBOs[it->first], it_prog->second, attr->noc, attr->type,false);
            }
        }//if(prog) 
    }



    //绘制指定subgeometry中的全部图元集合
    for (size_t set_id = 0; set_id < mSubGeometry[id]->PrimitiveSets.size(); set_id++)//当前primitive set的id
    {
        PrimitiveSet ps = mSubGeometry[id]->PrimitiveSets[set_id];

        unsigned int s1 = mSubGeometry.size();
        unsigned int s2 = mSubGeometry[id]->PrimitiveSets.size();
        unsigned int b = ps.mBegin;
        unsigned int e = ps.mCount;

        render->drawGeometryArray(nbPlugins::Renderer::TRIANGLES, b, e);
    }//for set_id

}
#endif 
