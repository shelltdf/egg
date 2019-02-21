
#include <thread>
#include <chrono>

#include <eggApplication/Application.h>

#include <eggEngine/Components/Transfrom3D.h>
#include <eggEngine/Components/MeshRender.h>
#include <eggEngine/Components/GUI.h>

#include "osgViewer.h"
#include "osgEarth.h"


void testScene(egg::SceneNode* parent)
{
    //root
    egg::SceneNode* sn_root = new egg::SceneNode("root");
    egg::Transfrom3D* t3d = new egg::Transfrom3D();
    sn_root->addComponent(t3d);

    //子节点
    egg::SceneNode* sn_child = new egg::SceneNode("c01");
    sn_root->addChild(sn_child);
    egg::Transfrom3D* t3d_child = new egg::Transfrom3D();
    sn_child->addComponent(t3d_child);

    //设置矩阵
    Eigen::Matrix4f mat_child;
    mat_child.setIdentity();

    Eigen::Affine3f af;
    af.setIdentity();
    af.translate(Eigen::Vector3f(0, 0, 0));
    mat_child = af.matrix();

    t3d_child->setLocalMatrix(mat_child);


    //new MeshRender
    egg::MeshRender* meshrender = new egg::MeshRender();
    sn_child->addComponent(meshrender);

    //new SubGeometry
    egg::SubGeometry* sg = new egg::SubGeometry();
    sg->addPrimitiveSet(egg::PrimitiveSet::Mode::TRIANGLES, 0, 6);

    //new Geometry
    egg::Geometry* geo = new egg::Geometry();
    {
        auto pos = new egg::BufferVec4f(6);
        auto normal = new egg::BufferVec3f(6);
        auto color = new egg::BufferVec4f(6);
        auto uv0 = new egg::BufferVec2f(6);
        
        float s = 10.0f;

        (*pos)[0] = Eigen::Vector4f(-s, -s, 0.0f, 1.0f);
        (*pos)[1] = Eigen::Vector4f(-s, s, 0.0f, 1.0f);
        (*pos)[2] = Eigen::Vector4f(s, s, 0.0f, 1.0f);
        (*pos)[3] = Eigen::Vector4f(s, s, 0.0f, 1.0f);
        (*pos)[4] = Eigen::Vector4f(s, -s, 0.0f, 1.0f);
        (*pos)[5] = Eigen::Vector4f(-s, -s, 0.0f, 1.0f);

        (*normal)[0] = Eigen::Vector3f(0.0f, 0.0f, 1.0f);
        (*normal)[1] = Eigen::Vector3f(0.0f, 0.0f, 1.0f);
        (*normal)[2] = Eigen::Vector3f(0.0f, 0.0f, 1.0f);
        (*normal)[3] = Eigen::Vector3f(0.0f, 0.0f, 1.0f);
        (*normal)[4] = Eigen::Vector3f(0.0f, 0.0f, 1.0f);
        (*normal)[5] = Eigen::Vector3f(0.0f, 0.0f, 1.0f);

        (*color)[0] = Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
        (*color)[1] = Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
        (*color)[2] = Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
        (*color)[3] = Eigen::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);
        (*color)[4] = Eigen::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);
        (*color)[5] = Eigen::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);

        (*uv0)[0] = Eigen::Vector2f(0.0f, 0.0f);
        (*uv0)[0] = Eigen::Vector2f(0.0f, 1.0f);
        (*uv0)[0] = Eigen::Vector2f(1.0f, 1.0f);
        (*uv0)[0] = Eigen::Vector2f(1.0f, 1.0f);
        (*uv0)[0] = Eigen::Vector2f(1.0f, 0.0f);
        (*uv0)[0] = Eigen::Vector2f(0.0f, 0.0f);


        //set buffers
        geo->mBuffers[egg_va_position] = pos;
        geo->mBuffers[egg_va_normal] = normal;
        geo->mBuffers[egg_va_color0] = color;
        geo->mBuffers[egg_va_uv0] = uv0;

        //set VA
        geo->mVectexAttribute[egg_va_position]
            = new egg::VectexAttribute(egg_va_position, 4, egg::DT_FLOAT, false, 0, egg_va_position);
        geo->mVectexAttribute[egg_va_normal]
            = new egg::VectexAttribute(egg_va_normal, 3, egg::DT_FLOAT, false, 0, egg_va_normal);
        geo->mVectexAttribute[egg_va_color0]
            = new egg::VectexAttribute(egg_va_color0, 4, egg::DT_FLOAT, false, 0, egg_va_color0);
        geo->mVectexAttribute[egg_va_uv0]
            = new egg::VectexAttribute(egg_va_uv0, 2, egg::DT_FLOAT, false, 0, egg_va_uv0);

    }
    geo->mSubGeometry.push_back(sg);
    meshrender->setGeometry(geo);

    //new Material
    egg::Material* mtl = new egg::Material();
    meshrender->addMaterial(mtl);

    //
    parent->addChild(sn_root);
}

void testGUI(egg::SceneNode* parent)
{
    //root
    egg::SceneNode* sn_root = new egg::SceneNode("root");
    egg::GUI* gui = new egg::GUI();
    sn_root->addComponent(gui);

    //
    parent->addChild(sn_root);
}

int main(int argc, char* argv[])
{
    egg::ref_ptr<egg::Application> app = new egg::Application();

    egg::ApplicationOption option;
    app->init(option);

    unsigned int context_id = app->getRenderWindow()->getContextID();

    //操作器
    //app->getRenderWindow()->getRoot()->addComponent(new egg::CameraHandler());

    testScene(app->getRenderWindow()->getRoot());

#if EGG_USE_OSG
    //testOSG(app->getRenderWindow()->getRoot());
#endif
#if EGG_USE_OSGEARTH
    //testOSGEarth(app->getRenderWindow()->getRoot(), context_id);
#endif

    //
    testGUI(app->getRenderWindow()->getRoot());


#if 0
    while (!app->isDone())
    {
        app->step(1.0 / 60.0);
    }
    app->shutdown();
#else
    app->run();
#endif
    app = 0;

    return 0;
}

