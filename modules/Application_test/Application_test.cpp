
//#include <eggRuntime/Platform.h>
//#include <eggRuntime/Module.h>
//#include <eggRuntime/Runtime.h>
//#include <eggRuntime/Path.h>

#include <eggApplication/Application.h>

#include <eggEngine/Components/Transfrom3D.h>
#include <eggEngine/Components/MeshRender.h>
#include <eggEngine/ResourceManager.h>




class Application_test
    :public egg::Application
{
public:

    Application_test()
    {
    }

    ~Application_test()
    {
    }

    bool init(const egg::ApplicationOption& option)
    {
        Application::init(option);

        //create scene node
        egg::SceneNode* sn = buildSceneNode();
        this->getRenderWindow()->getRoot()->addChild(sn);
        
        //rm->readResourceGroup()


        //over
        
        return true;
    }

    bool step(double dt_second)
    {
        return Application::step(dt_second);
    }

    bool shutdown()
    {
        return Application::shutdown();
    }



    egg::SceneNode* buildSceneNode()
    {
        //root
        egg::SceneNode* sn_root = new egg::SceneNode("root");
        egg::Transfrom3D* t3d = new egg::Transfrom3D();
        sn_root->addComponent(t3d);

        //当前需要建立meshrender的sn
        egg::SceneNode* sn_child_test = 0;

        //子节点
        {
            egg::SceneNode* sn_child = new egg::SceneNode("c01");
            sn_root->addChild(sn_child);
            egg::Transfrom3D* t3d_child = new egg::Transfrom3D();
            sn_child->addComponent(t3d_child);

            //
            //vmml::mat4f mat_child;
            //vmml::identity(mat_child);
            //mat_child.rotate_z(1.0);
            //mat_child.set_translation(0, 2, 0);

            Eigen::Matrix4f mat_child;
            mat_child.setIdentity();

            Eigen::Affine3f af;
            af.setIdentity();
            af.translate(Eigen::Vector3f(0, 0, -10));
            mat_child = af.matrix();

            t3d_child->setLocalMatrix(mat_child);

            sn_child_test = sn_child;
        }

        //子节点
        {
            egg::SceneNode* sn_child = new egg::SceneNode("c02");
            sn_root->addChild(sn_child);
            egg::Transfrom3D* t3d_child = new egg::Transfrom3D();
            sn_child->addComponent(t3d_child);

            //
            //vmml::mat4f mat_child;
            //vmml::identity(mat_child);
            ////mat_child.rotate_z(1.0);
            //mat_child.set_translation(2, 0, 0);

            Eigen::Matrix4f mat_child;
            mat_child.setIdentity();

            t3d_child->setLocalMatrix(mat_child);
        }


        //new MeshRender
        egg::MeshRender* meshrender = new egg::MeshRender();
        sn_child_test->addComponent(meshrender);

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

            (*pos)[0] = Eigen::Vector4f(-1.0f, -1.0f, 0.0f, 1.0f);
            (*pos)[1] = Eigen::Vector4f(-1.0f, 1.0f, 0.0f, 1.0f);
            (*pos)[2] = Eigen::Vector4f(1.0f, 1.0f, 0.0f, 1.0f);
            (*pos)[3] = Eigen::Vector4f(1.0f, 1.0f, 0.0f, 1.0f);
            (*pos)[4] = Eigen::Vector4f(1.0f, -1.0f, 0.0f, 1.0f);
            (*pos)[5] = Eigen::Vector4f(-1.0f, -1.0f, 0.0f, 1.0f);

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


        return sn_root;
    }


};


INSTALL_MODULE(Application_test)
{
    egg::ApplicationFactory::getInstance(true)
        ->registerCrteator("Application_test"
            , new egg::Crteator<Application_test>());
    return true;
}

UNINSTALL_MODULE(Application_test)
{
    egg::ApplicationFactory::getInstance(true)
        ->unregisterCrteator("Application_test");
    return true;
}
