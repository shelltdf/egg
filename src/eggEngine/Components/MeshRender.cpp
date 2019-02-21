
#include <eggEngine/Components/MeshRender.h>
#include <eggEngine/RenderVisitor.h>
#include <eggEngine/RenderNode.h>
#include <eggEngine/SceneNode.h>

using namespace egg;


void MeshRender::traverse(NodeVisitor& nv)
{
    if (nv.as<nv_collectionResourceObject>())
    {
        nv_collectionResourceObject* cro = nv.as<nv_collectionResourceObject>();

        //Geometry
        if (mGeometry->isLoaded())
        {
            cro->result[mGeometry->getPath()] = mGeometry->getResource();
        }

        //Material
        for (auto it = mMaterial.begin(); it != mMaterial.end(); it++)
        {
            if ((*it)->isLoaded())
            {
                Path p = (*it)->getPath();
                if (p.size() > 0)
                {
                    cro->result[p] = (*it)->getResource();
                }
                else
                {
                    printf("Material path is null\n");
                }
            }
        }
    }
    else if (nv.as<nv_collectionResourceLink>())
    {
        nv_collectionResourceLink* crl = nv.as<nv_collectionResourceLink>();

        //Geometry
        if (!mGeometry->isLoaded())
        {
            crl->result.push_back(mGeometry.get());
        }

        //Material
        for (auto it = mMaterial.begin(); it != mMaterial.end(); it++)
        {
            if (!(*it)->isLoaded())
            {
                crl->result.push_back((*it).get());
            }
        }
    }

    ComponentBase::traverse(nv);
}

Material* MeshRender::fixMaterial(unsigned int index)
{
    //查询对应的材质  如果渲染段数量大于材质数量就沿用最后一个
    Material* m = 0;
    if (getNumMaterial() == 0)       //如果没有材质 m=0
    {
        //m = mMaterialList[0].get();
    }
    else if (index < getNumMaterial())   //存在正确的material m=id
    {
        m = getMaterial(index);
    }
    else                                 //如果超过了material m=最后一个材质
    {
        m = getMaterial(getNumMaterial() - 1);
    }

    //如果没有找到任何材质 渲染是无法进行的 可以考虑设置默认材质
    //if (m == 0)
    //{
    //    //提示错误
    //    printf("no material\n");
    //    return;
    //}
    return m;
}

void MeshRender::onCull(nv_cull* nv, Eigen::Matrix4f& vm)
{
    //如果没有mesh就完全无效了
    if (!mGeometry->isLoaded()) return;

    //nv_cull* cull = nv.as<nv_cull>();
    RenderNode* rendernode = nv->mRenderNode;

    //遍历全部subgeometry
    unsigned int sg_size = mGeometry->getResource()->getNumSubGeometry();
    for (size_t sg_index = 0; sg_index < sg_size; sg_index++)
    {
        //确定当前material对象
        Material* material = fixMaterial(sg_index);

        //遍历全部pass
        unsigned int pass_num = material->getTech(material->getCurrentTech())->mPasses.size();
        for (size_t pass_index = 0; pass_index < pass_num; pass_index++)
        {
            //当前pass
            Pass* pass = material->getTech(material->getCurrentTech())->mPasses[pass_index].get();

            //查询当前RN和RF
            RenderNode* child_rn = rendernode->getOrCreateChild(pass);
            child_rn->setPass(pass);

            //把所有subgeomtery添加到leaf
            RenderLeaf* rl = child_rn->getOrCreateRenderLeaf();
            rl->setViewMatrix(nv->cameraViewMatrix * vm);
            rl->setProjectionMatrix(nv->projactiveMatrix);

            RenderLeaf::DrawableSet sgs;
            sgs.drawable = mGeometry->getResource();
            sgs.subgeometry_index = sg_index;

            rl->addDrawable(sgs);
        }
    }
}

void MeshRender::onRender(nv_render* nv, Eigen::Matrix4f& mv)
{
#if 0
    //for (int i = 0; i < mMeshList.size(); i++)
    //{
    //    mMeshList[i]->render(nv->render,nv->projactiveMatrix,mv);
    //}

    if (!mGeometry->isLoaded()) return;

    //计算subgeometry的渲染段数量
    unsigned int num_subgeometry = mGeometry->getResource()->getNumSubGeometry();

    //循环所有geometry渲染端
    for (size_t i = 0; i < num_subgeometry; i++)
    {
        //查询对应的材质  如果渲染段数量大于材质数量就沿用最后一个
        Material* m = 0;
        if (getNumMaterial() == 0)       //如果没有材质 m=0
        {
            //m = mMaterialList[0].get();
        }
        else if (i < getNumMaterial())   //存在正确的material m=id
        {
            m = getMaterial(i);
        }
        else                                 //如果超过了material m=最后一个材质
        {
            m = getMaterial(getNumMaterial() - 1);
        }

        //如果没有找到任何材质 渲染是无法进行的 可以考虑设置默认材质
        if (m == 0)
        {
            //提示错误
            printf("no material\n");
            return;
        }

        //循环所有pass 渲染几何数据
        unsigned int pass_num = m->getTech(m->getCurrentTech())->mPasses.size();
        for (size_t ipass = 0; ipass < pass_num; ipass++)
        {
            //启用材质
            Material::Pass* p = m->getTech(m->getCurrentTech())->mPasses[ipass].get();

            //预处理自动变量
            //特殊属性处理  以后该进程一个标准接口  明确提示属性的存在
            auto it = mGeometry->getResource()->mVectexAttribute.find(NFX_VectexAttribute_bone_indices);
            if (it != mGeometry->getResource()->mVectexAttribute.end())
            {
                int skinning = 1;
                p->setUniform<int>("skinning", UniformBase::UniformDataType::UDT_INT, &skinning, 1, "");
            }
            else
            {
                int skinning = 0;
                p->setUniform<int>("skinning", UniformBase::UniformDataType::UDT_INT, &skinning, 1, "");
            }

            //bind pass
            p->bind(nv->render, mv, nv->projactiveMatrix);

            //渲染subgeometry
            mGeometry->getResource()->renderSubGeometry(nv->render, p, i);
        }

    }//for
#endif //0
}

void MeshRender::save(Path filename, std::ostream& os, Serializer* ser) const
{
    ComponentBase::save(filename, os, ser);

    //Geometry
    saveResourceLink<Geometry>(mGeometry.get(), Path(filename), os, ser);

    //Material
    unsigned int tex_num = getNumMaterial();
    ser->writeNumber(os, tex_num);  //Texture数量
    for (unsigned int j = 0; j < tex_num; j++)
    {
        saveResourceLink<Material>(mMaterial[j].get(), Path(filename), os, ser);
    }
}

void MeshRender::load(Path filename, std::istream& is
    , Serializer* ser, ObjectManager* om
    , Referenced* rmg, bool load_ext)
{
    ComponentBase::load(filename, is, ser, om, rmg, load_ext);

    //Geometry
    mGeometry = new ResourceLinkT< Geometry >();
    loadResourceLink<Geometry>(filename, mGeometry.get(), is, ser);

    //Material
    unsigned int mtl_num = 0;
    ser->readNumber(is, mtl_num);  //Texture数量
    for (unsigned int t = 0; t < mtl_num; t++)
    {
        ResourceLinkT< Material >* material = new ResourceLinkT< Material >();
        loadResourceLink<Material>(filename, material, is, ser);
        mMaterial.push_back(material);
    }
}

