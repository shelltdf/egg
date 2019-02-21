#ifndef _EGG_osgViewer_H_
#define _EGG_osgViewer_H_

//#define EGG_USE_OSG 1

#include <eggEngine/Component.h>

#if EGG_USE_OSG

#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/GraphicsContext>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StandardManipulator>
#include <osgGA/TrackballManipulator>

#include <osgViewer/api/Win32/GraphicsWindowWin32>

//#include <osgEarth/MapNode>
//#include <osgEarth/Registry>
//#include <osgEarth/Capabilities>


class MY_CameraManipulator
    :public osgGA::CameraManipulator
{
public:
    MY_CameraManipulator() {}
    virtual ~MY_CameraManipulator() {}

    virtual void setByMatrix(const osg::Matrixd& matrix)
    {
        mMatrixd = matrix;
    }
    virtual void setByInverseMatrix(const osg::Matrixd& matrix)
    {
        mMatrixd = osg::Matrix::inverse(matrix);
    }

    virtual osg::Matrixd getMatrix() const
    {
        return mMatrixd;
    }
    virtual osg::Matrixd getInverseMatrix() const
    {
        return osg::Matrix::inverse(mMatrixd);
    }
private:

    osg::Matrixd mMatrixd;
};

//父节点必须包含这个组件
class Component_osgViewer
    :public egg::ComponentBase
{
public:
    Component_osgViewer()
    {
        mRoot = new osg::Group();
    }
    virtual ~Component_osgViewer() {}
    virtual std::string ObjectClassName() override { return "osgViewer"; }

    virtual void onAttach()override
    {
        printf("onAttach\n");


    }
    virtual void onDetach()override
    {
        printf("onDetach\n");
        mViewer = 0;
    }

    virtual void onInput(egg::nv_input* nv) override
    {
        egg::WindowEvent we = nv->mWindowEvent;
        if (we.mWindowEventType == egg::WET_SIZE)
        {
#if 0
            osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>
                (mViewer->getCamera()->getGraphicsContext());
            if (gw)
            {
                //int x, y, w, h; 
                //gw->getWindowRectangle(x, y, w, h);

                mViewer->getEventQueue()->windowResize(we.x, we.y, we.z, we.w);
                mViewer->getCamera()->setViewport(0, 0, we.z, we.w);
            }
#endif
        }

        if (we.mWindowEventType == egg::WET_MOUSE)
        {
            if (we.mMouseType == egg::MT_Move)
            {
                mViewer->getEventQueue()->mouseMotion(we.x,we.y);
            }
            if (we.mMouseType == egg::MT_Click)
            {
                if (we.mMouseButton == egg::MB_Left)
                {
                    if (we.mKeyState == egg::KS_DOWN)
                    {
                        mViewer->getEventQueue()->mouseButtonPress(we.x, we.y,1);
                    }
                    else
                    {
                        mViewer->getEventQueue()->mouseButtonRelease(we.x, we.y, 1);
                    }
                }
                if (we.mMouseButton == egg::MB_Middle)
                {
                    if (we.mKeyState == egg::KS_DOWN)
                    {
                        mViewer->getEventQueue()->mouseButtonPress(we.x, we.y, 2);
                    }
                    else
                    {
                        mViewer->getEventQueue()->mouseButtonRelease(we.x, we.y, 2);
                    }
                }
                if (we.mMouseButton == egg::MB_Right)
                {
                    if (we.mKeyState == egg::KS_DOWN)
                    {
                        mViewer->getEventQueue()->mouseButtonPress(we.x, we.y, 3);
                    }
                    else
                    {
                        mViewer->getEventQueue()->mouseButtonRelease(we.x, we.y, 3);
                    }
                }
            }//MT_Click
        }//WET_MOUSE
    }
    virtual void onUpdate(egg::nv_update* nv) override {}
    virtual void onCull(egg::nv_cull* nv, Eigen::Matrix4f& vm) override 
    {
        if (mEGGtoOSG)
        {
            Eigen::Matrix4f view = nv->cameraViewMatrix;
            Eigen::Matrix4f proj = nv->projactiveMatrix;

            osg::Matrixd m;
            m.set(view(0), view(1), view(2), view(3)
                , view(4), view(5), view(6), view(7)
                , view(8), view(9), view(10), view(11)
                , view(12), view(13), view(14), view(15));

            osg::Matrixd pm;
            pm.set(proj(0), proj(1), proj(2), proj(3)
                , proj(4), proj(5), proj(6), proj(7)
                , proj(8), proj(9), proj(10), proj(11)
                , proj(12), proj(13), proj(14), proj(15));

            if (mCameraManipulator.valid())
            {
                mCameraManipulator->setByInverseMatrix(m);
            }

            if (mViewer.valid())
            {
                mViewer->getCamera()->setProjectionMatrix(pm);
            }
        }
    }

    virtual void onRender(egg::nv_render* nv, Eigen::Matrix4f& mv) override {}
    virtual void onGUI(egg::nv_gui* nv)override
    {
        //printf("g");

        //禁止渲染状态
        //egg::handle_program program;
        nv->renderer->unbindProgram();

        //初始化viewer
        if (!mViewer.valid())
        {
            //OE
            //if(osgEarth::Registry::capabilities().supportsGLSL())
            //{
            //    printf("aaaaaaaaaaaaaaaaaa");
            //}


            //osgViewer::Viewer viewer;

            mViewer = new osgViewer::Viewer();
            mViewer->setThreadingModel(osgViewer::Viewer::ThreadingModel::SingleThreaded);

            setEGGtoOSG(false);

#if 1
            osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> gwe
                = mViewer->setUpViewerAsEmbeddedInWindow(0, 0, nv->viewport[2], nv->viewport[3]);
#else
            osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
            traits->windowName = "newGraphicsContext";
            traits->windowDecoration = false;
            traits->x = 0;
            traits->y = 0;
            traits->width = 1;
            traits->height = 1;
            traits->doubleBuffer = true;
            traits->inheritedWindowData
                = new osgViewer::GraphicsWindowWin32::WindowData((HWND)nv->window->mWinID);

            mViewer->setThreadingModel(osgViewer::Viewer::ThreadingModel::SingleThreaded);
            osgViewer::GraphicsWindowEmbedded* gw = new osgViewer::GraphicsWindowEmbedded(traits);
            mViewer->getCamera()->setViewport(new osg::Viewport(0, 0, nv->viewport[2], nv->viewport[3]));
            mViewer->getCamera()->setProjectionMatrixAsPerspective(
                30.0f, static_cast<double>(nv->viewport[2]) / static_cast<double>(nv->viewport[3]), 1.0f, 10000.0f);
            mViewer->getCamera()->setGraphicsContext(gw);
#endif


            //mViewer = InitCameraConfig(nv);
            //mViewer->setThreadingModel(osgViewer::Viewer::ThreadingModel::SingleThreaded);

            //viewer.setSceneData(loadedModel.get());
            mViewer->setSceneData(mRoot.get());
            //mViewer->setCameraManipulator(new osgGA::TrackballManipulator);
            //mViewer->home();
            mViewer->addEventHandler(new osgViewer::StatsHandler);
            //mViewer->getCamera()->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            mViewer->getCamera()->setClearMask(0);
            //mViewer->getCamera()->setClearColor(osg::Vec4(1, 0, 1, 1));
            

            //重新建立GC对象避免嵌入窗口的冲突
            //reconfigure(mViewer.get(),nv);
            //const osg::GraphicsContext::Traits* traits =
            //    mViewer->getCamera()->getGraphicsContext()->getTraits();
            //traits->inheritedWindowData 
            //    = new osgViewer::GraphicsWindowWin32::WindowData((HWND)nv->window->mWinID);


            //mViewer->realize();
            mViewer->frame();

            //设置正确的视口比例
            osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>
                (mViewer->getCamera()->getGraphicsContext());
            if (gw)
            {
                mViewer->getEventQueue()->windowResize(
                    nv->viewport[0], nv->viewport[1], nv->viewport[2], nv->viewport[3]);
            }
        }

        //更新MVP
        mViewer->getCamera()->setViewport(0, 0, nv->viewport[2], nv->viewport[3]);
        if (mViewer->getCamera()->getComputeNearFarMode() != osg::Camera::DO_NOT_COMPUTE_NEAR_FAR)
        {
            //mViewer->getCamera()->setProjectionMatrixAsPerspective(30.0, nv->viewport[2] / nv->viewport[3], 0.1, 100);
        }

        //更新
        //if (!mViewer->isRealized()) { mViewer->realize(); }
        mViewer->frame();
    }
#if 0
    void reconfigure(osgViewer::Viewer* view, egg::nv_gui* nv)
    {
        osg::ref_ptr<osg::GraphicsContext> _gc;
        if (!_gc.valid())
        {
            // create the Qt graphics context if necessary; it will be shared across all views.
            osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
            osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits(ds);

            traits->readDISPLAY();
            if (traits->displayNum < 0) traits->displayNum = 0;

            traits->windowName = "newGraphicsContext";
            traits->windowDecoration = false;
            traits->x = 0;
            traits->y = 0;
            traits->width = 1;
            traits->height = 1;
            traits->doubleBuffer = true;

            traits->inheritedWindowData
                = new osgViewer::GraphicsWindowWin32::WindowData((HWND)nv->window->mWinID);

            _gc = osg::GraphicsContext::createGraphicsContext(traits.get());
        }

        // reconfigure this view's camera to use the Qt GC if necessary.
        osg::Camera* camera = view->getCamera();
        if (camera->getGraphicsContext() != _gc.get())
        {
            camera->setGraphicsContext(_gc.get());
            if (!camera->getViewport())
            {
                camera->setViewport(new osg::Viewport(0, 0, _gc->getTraits()->width, _gc->getTraits()->height));
            }
            if (mViewer->getCamera()->getComputeNearFarMode() != osg::Camera::DO_NOT_COMPUTE_NEAR_FAR)
            {
                camera->setProjectionMatrixAsPerspective(
                    30.0f, camera->getViewport()->width() / camera->getViewport()->height(), 1.0f, 10000.0f);
            }
        }
    }
#endif
    osgViewer::Viewer* InitCameraConfig(egg::nv_gui* nv) const
    {
        // Local Variable to hold window size data
        RECT rect;

        // Create the viewer for this window
        osgViewer::Viewer* viewer = new osgViewer::Viewer();

        // Add a Stats Handler to the viewer
        //mViewer->addEventHandler(new osgViewer::StatsHandler);

        // Get the current window size
        ::GetWindowRect((HWND)nv->window->mWinID, &rect);

        // Init the GraphicsContext Traits
        osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;

        // Init the Windata Variable that holds the handle for the Window to display OSG in.
        osg::ref_ptr<osg::Referenced> windata
            = new osgViewer::GraphicsWindowWin32::WindowData((HWND)nv->window->mWinID);

        // Setup the traits parameters
        traits->x = 0;
        traits->y = 0;
        traits->width = rect.right - rect.left;
        traits->height = rect.bottom - rect.top;
        traits->windowDecoration = false;
        traits->doubleBuffer = true;
        traits->sharedContext = 0;
        traits->setInheritedWindowPixelFormat = true;
        traits->inheritedWindowData = windata;

        // Create the Graphics Context
        osg::GraphicsContext* gc = osg::GraphicsContext::createGraphicsContext(traits.get());

        // Init Master Camera for this View
        osg::ref_ptr<osg::Camera> camera = viewer->getCamera();

        // Assign Graphics Context to the Camera
        camera->setGraphicsContext(gc);

        // Set the viewport for the Camera
        //camera->setViewport(new osg::Viewport(traits->x, traits->y, traits->width, traits->height));

        // Set projection matrix and camera attribtues
        //camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        //camera->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f));
        //camera->setProjectionMatrixAsPerspective(
        //    30.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0, 1000.0);

        // Add the Camera to the Viewer
        //mViewer->addSlave(camera.get());
        viewer->setCamera(camera.get());

        // Add the Camera Manipulator to the Viewer
        //mViewer->setCameraManipulator(keyswitchManipulator.get());

        // Set the Scene Data
        //mViewer->setSceneData(mRoot.get());

        // Realize the Viewer
        //mViewer->realize();

        // Correct aspect ratio
        /*double fovy,aspectRatio,z1,z2;
        mViewer->getCamera()->getProjectionMatrixAsPerspective(fovy,aspectRatio,z1,z2);
        aspectRatio=double(traits->width)/double(traits->height);
        mViewer->getCamera()->setProjectionMatrixAsPerspective(fovy,aspectRatio,z1,z2);*/

        return viewer;
    }

    void setEGGtoOSG(bool e)
    {
        if (mEGGtoOSG != e || !mCameraManipulator.valid())
        {
            if (e)
            {
                if(!mCameraManipulator.valid())mCameraManipulator = new MY_CameraManipulator();
                mViewer->setCameraManipulator(mCameraManipulator);
                mViewer->getCamera()->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
            }
            else
            {
                mViewer->setCameraManipulator(new osgGA::TrackballManipulator);
                mViewer->getCamera()->setComputeNearFarMode(osg::Camera::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);
            }
        }
    }

    bool mEGGtoOSG = true;

    osg::ref_ptr<osgViewer::Viewer> mViewer;
    osg::ref_ptr<MY_CameraManipulator> mCameraManipulator;
    osg::ref_ptr<osg::Group> mRoot;
    //osg::Group* root = new osg::Group();
};


/*
* 组件的类型：
*    Node组件
*       明确类型组件
*       抽象类型组件（主要用于未知类型）
*    Drawable组件
*       Drawable
*       Geometry
*    StateSet组件
*       NodeStateSet
*       GeometryStateSet
*
*   所有组件都通过一个指针指向被自己管理的osg对象
*/


//针对没有明确类型支持的节点
class osgNode
    :public egg::ComponentBase
{
public:
    osgNode() {}
    virtual ~osgNode() {}
    virtual std::string ObjectClassName() override { return "osgNode"; }

    virtual void onInput(egg::nv_input* nv) override {}
    virtual void onUpdate(egg::nv_update* nv) override {}
    virtual void onCull(egg::nv_cull* nv, Eigen::Matrix4f& vm) override {}
    virtual void onRender(egg::nv_render* nv, Eigen::Matrix4f& mv) override {}
    virtual void onGUI(egg::nv_gui* nv)override {}
};

//针对Group类型
class osgGroup
    :public egg::ComponentBase
{
public:
    osgGroup() {}
    virtual ~osgGroup() {}
    virtual std::string ObjectClassName() override { return "osgGroup"; }

    virtual void onInput(egg::nv_input* nv) override {}
    virtual void onUpdate(egg::nv_update* nv) override {}
    virtual void onCull(egg::nv_cull* nv, Eigen::Matrix4f& vm) override {}
    virtual void onRender(egg::nv_render* nv, Eigen::Matrix4f& mv) override {}
    virtual void onGUI(egg::nv_gui* nv)override {}
};

//针对无明确类型的Transform
class osgTransform
    :public egg::ComponentBase
{
public:
    osgTransform() {}
    virtual ~osgTransform() {}
    virtual std::string ObjectClassName() override { return "osgTransform"; }

    virtual void onInput(egg::nv_input* nv) override {}
    virtual void onUpdate(egg::nv_update* nv) override {}
    virtual void onCull(egg::nv_cull* nv, Eigen::Matrix4f& vm) override {}
    virtual void onRender(egg::nv_render* nv, Eigen::Matrix4f& mv) override {}
    virtual void onGUI(egg::nv_gui* nv)override {}
};

class osgMT
    :public egg::ComponentBase
{
public:
    osgMT() {}
    virtual ~osgMT() {}
    virtual std::string ObjectClassName() override { return "osgMT"; }

    virtual void onInput(egg::nv_input* nv) override {}
    virtual void onUpdate(egg::nv_update* nv) override {}
    virtual void onCull(egg::nv_cull* nv, Eigen::Matrix4f& vm) override {}
    virtual void onRender(egg::nv_render* nv, Eigen::Matrix4f& mv) override {}
    virtual void onGUI(egg::nv_gui* nv)override {}
};

class osgPAT
    :public egg::ComponentBase
{
public:
    osgPAT() {}
    virtual ~osgPAT() {}
    virtual std::string ObjectClassName() override { return "osgPAT"; }

    virtual void onInput(egg::nv_input* nv) override {}
    virtual void onUpdate(egg::nv_update* nv) override {}
    virtual void onCull(egg::nv_cull* nv, Eigen::Matrix4f& vm) override {}
    virtual void onRender(egg::nv_render* nv, Eigen::Matrix4f& mv) override {}
    virtual void onGUI(egg::nv_gui* nv)override {}
};

//geode作为最末端节点支持
class osgGeode
    :public egg::ComponentBase
{
public:
    osgGeode() {}
    virtual ~osgGeode() {}
    virtual std::string ObjectClassName() override { return "osgGeode"; }

    virtual void onInput(egg::nv_input* nv) override {}
    virtual void onUpdate(egg::nv_update* nv) override {}
    virtual void onCull(egg::nv_cull* nv, Eigen::Matrix4f& vm) override {}
    virtual void onRender(egg::nv_render* nv, Eigen::Matrix4f& mv) override {}
    virtual void onGUI(egg::nv_gui* nv)override {}
};

//Drawable 作为geode的组件
class osgDrawable
    :public egg::ComponentBase
{
public:
    osgDrawable() {}
    virtual ~osgDrawable() {}
    virtual std::string ObjectClassName() override { return "osgDrawable"; }

    virtual void onInput(egg::nv_input* nv) override {}
    virtual void onUpdate(egg::nv_update* nv) override {}
    virtual void onCull(egg::nv_cull* nv, Eigen::Matrix4f& vm) override {}
    virtual void onRender(egg::nv_render* nv, Eigen::Matrix4f& mv) override {}
    virtual void onGUI(egg::nv_gui* nv)override {}
};
//Geometry 作为geode的组件
class osgGeometry
    :public egg::ComponentBase
{
public:
    osgGeometry() {}
    virtual ~osgGeometry() {}
    virtual std::string ObjectClassName() override { return "osgGeometry"; }

    virtual void onInput(egg::nv_input* nv) override {}
    virtual void onUpdate(egg::nv_update* nv) override {}
    virtual void onCull(egg::nv_cull* nv, Eigen::Matrix4f& vm) override {}
    virtual void onRender(egg::nv_render* nv, Eigen::Matrix4f& mv) override {}
    virtual void onGUI(egg::nv_gui* nv)override {}
};

//StateSet 两种用途
// 1 作为SN的组件
// 2 作为根据geometry的关系添加到geometry的SN上 作为一个特例
// 所以这里要集成两种类型的StateSet
class osgNodeStateSet
    :public egg::ComponentBase
{
public:
    osgNodeStateSet() {}
    virtual ~osgNodeStateSet() {}
    virtual std::string ObjectClassName() override { return "osgNodeStateSet"; }

    virtual void onInput(egg::nv_input* nv) override {}
    virtual void onUpdate(egg::nv_update* nv) override {}
    virtual void onCull(egg::nv_cull* nv, Eigen::Matrix4f& vm) override {}
    virtual void onRender(egg::nv_render* nv, Eigen::Matrix4f& mv) override {}
    virtual void onGUI(egg::nv_gui* nv)override {}
};
class osgGeometryStateSet
    :public egg::ComponentBase
{
public:
    osgGeometryStateSet() {}
    virtual ~osgGeometryStateSet() {}
    virtual std::string ObjectClassName() override { return "osgGeometryStateSet"; }

    virtual void onInput(egg::nv_input* nv) override {}
    virtual void onUpdate(egg::nv_update* nv) override {}
    virtual void onCull(egg::nv_cull* nv, Eigen::Matrix4f& vm) override {}
    virtual void onRender(egg::nv_render* nv, Eigen::Matrix4f& mv) override {}
    virtual void onGUI(egg::nv_gui* nv)override {}
};



//更新node
//void updateOSGNode(egg::SceneNode* sn, osg::Node* node)
//{
//}

//访问器
class updateSceneNode
    :public osg::NodeVisitor
{
public:
    updateSceneNode() {}
    virtual ~updateSceneNode() {}
    egg::SceneNode* getSceneNode() { return sn; }

private:
    egg::SceneNode* sn = 0;
};


//绑定新的node
egg::SceneNode* bindOSGNode(osg::Node* node)
{
    if (node == 0) return 0;

    osg::ref_ptr<updateSceneNode> nv = new updateSceneNode();
    node->accept(*nv.get());
    return nv->getSceneNode();
}


void testOSG(egg::SceneNode* parent)
{
    //osgViewer
    egg::SceneNode* sn_viewer = new egg::SceneNode("sn_viewer");
    Component_osgViewer* viewer = new Component_osgViewer();
    sn_viewer->addComponent(viewer);

    //建立node
    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("../osgData/cow.osg");
    //osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("D:/_work_bt/_aliyun_svn/myproject/project_bluesky/data/hainan.ive");
    if (node)
    {
        //osg::Group* root = new osg::Group();
        //viewer->mViewer->setSceneData(root);

        //root->addChild(node);

        osg::MatrixTransform* mt = new osg::MatrixTransform();
        mt->addChild(node);
        viewer->mRoot->addChild(mt);

        //mt->setMatrix(osg::Matrix::translate(0, 0, -10));

        //viewer->mViewer->home();

        //egg::SceneNode* sn_node = bindOSGNode(node);
        //if (sn_node)
        //{
        //    sn_viewer->addChild(sn_node);
        //}
    }

    //
    parent->addChild(sn_viewer);
}

#endif //EGG_USE_OSG
#endif //_EGG_osgViewer_H_

