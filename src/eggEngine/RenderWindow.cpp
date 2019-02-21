
#include <eggEngine/RenderWindow.h>

#include <eggEngine/Components/Transfrom3D.h>


using namespace egg;



RenderWindow::RenderWindow(Window* win, GraphicsContext* gc, Renderer* rd)
    :mWindow(win), mGraphicsContext(gc), mRenderer(rd)
{
    init();
}

RenderWindow::~RenderWindow()
{
}

bool RenderWindow::init()
{
    //create root node
    mRoot = new SceneNode();
    mRoot->addComponent(new egg::Camera());
    mRoot->addComponent(new egg::CameraHandler());

    //create default camera
    //mDefaultCamera = new SceneNode();
    //mDefaultCamera->addComponent(new egg::Camera());

    //create default composer
    mComposer = new Composer();
    mComposer->addStandardPass("default");

    return true;
}

bool RenderWindow::shutdown()
{

    return true;
}

bool RenderWindow::startInputThread()
{
    return true;
}

bool RenderWindow::startRenderThread()
{
    return true;
}

bool RenderWindow::runStep(double dt_second)
{
    if (mWindow.valid())
    {
        if (mThreadMode == 0)
        {
            processInput(dt_second);
            processRender(dt_second);
        }

        if (mThreadMode == 1)
        {
            //如果线程没启动就启动线程
            if (true)
            {

            }
            if (true)
            {

            }
        }

        return mWindow->isShow();
    }
    return false;
}


void RenderWindow::processInput(double dt_second)
{
    //更新消息 
    //  1. 收集全部消息放入消息队列
    mWindow->frame(dt_second);

    //  2. 对场景提出一次遍历，转接到compoents的onInput接口
    std::queue<WindowEvent> weq = mWindow->getWindowEventQueue();
    while (!weq.empty())
    {
        //每个WindowEvent发起一次遍历
        auto we = weq.front();

        ref_ptr<nv_input> nv = new nv_input();
        nv->mWindowEvent = we;

        std::map<std::string, ref_ptr< RenderPass > > rps = mComposer->getPasses();
        for (auto it = rps.begin(); it != rps.end(); it++)
        {
            //通知摄像机更新视口
            egg::Camera* camera = it->second->getCamera();
            if (camera == 0)
            {
                //第一次更新的时候 camera的关联是无效的 所以使用mDefaultCamera
                //camera = mDefaultCamera->getComponent<Camera>();

                //直接从ROOT取摄像机
                camera = mRoot->getComponent<Camera>();
            }
            camera->onInput(nv.get());


            if (it->second->getRenderScene())
            {
                //第一次更新的时候  pass的SceneNode有可能是没有的 等待渲染的适合弥补
                it->second->getRenderScene()->accept(*nv);
            }
        }

        weq.pop();
    }

    // 3. 清理消息
    mWindow->cleanWindowEventQueue();
}

void RenderWindow::processRender(double dt_second)
{

    //STEP 1. 建立统一时间

    //STEP 2. 更新场景(更新全部场景图)
    ref_ptr<nv_update> nv = new nv_update();
    nv->dt = dt_second;
    //nv->currentViewMatrix = this->viewMatrix;//这里应该是单位矩阵
    mRoot->accept(*nv);


    //STEP 3. CULL场景(遍历指定的pass的camera关联的场景图)
    //搜索全部可用的camera强制执行onCull()
    std::map<std::string, ref_ptr< RenderPass > > passes = mComposer->getPasses();
    for (auto it = passes.begin(); it != passes.end(); it++)
    {
        RenderPass* pass = it->second.get();
        StandardRenderPass* spass = dynamic_cast<StandardRenderPass*>(pass);
        if (spass)
        {
            //处理默认值问题
            egg::Camera* camera = spass->getCamera();
            if (camera == 0)
            {
                //spass->setCamera(mDefaultCamera->getComponent<Camera>());
                spass->setCamera(mRoot->getComponent<Camera>()); //使用默认摄像机作为输入
            }
            egg::SceneNode* sn = spass->getRenderScene();
            if (sn == 0)
            {
                spass->setRenderScene(mRoot.get()); //使用默认的场景作为渲染内容
            }

            //每次cull之前都重建结果对象
            spass->resetRenderNode();

            //单独执行camera的onCull操作
            //   当前的mRenderNode就是对应camera的渲染状态
            //   仅仅记录camera的pass到rendernode
            ref_ptr<nv_cull> cull = new nv_cull();
            cull->mSceneNode = spass->getRenderScene(); //场景输入
            cull->mRenderNode = spass->getRenderNode(); //渲染图输出

            //设置默认的mv矩阵
            //  这个矩阵是世界坐标系
            //  scenenode记录的是相对于世界中心的坐标。所以这里的矩阵必须是实际矩阵。而不能是摄像机的观察矩阵。
            //  在添加到renderleaf的时候等于 cam->vm * sn->vm 
            Eigen::Matrix4f world_matrix;
            world_matrix.setIdentity();

            //在这个处理过程中指定摄像机的投影矩阵
            spass->getCamera()->onCull(cull.get(), world_matrix);


            //遍历摄像机对应的场景图
            //  建立场景渲染图
            //spass->getRenderScene()->accept(*cull);
        }
    }


    //STEP 4. 执行渲染(遍历渲染图)
    //std::map<std::string, ref_ptr< RenderPass > > passes = mComposer->getPasses();
    for (auto it = passes.begin(); it != passes.end(); it++)
    {
        RenderPass* pass = it->second.get();

        ref_ptr<nv_render> nv = new nv_render();
        nv->render = mRenderer.get();
        //nv->context = mGraphicsContext->m_gcID;
        nv->gc = mGraphicsContext.get();

        //设定摄像机关系 清理缓冲区
        Eigen::Matrix4f n;
        pass->getCamera()->onRender(nv.get(), n);//这里的第二个参数没有任何作用

        //绘制渲染树
        pass->getRenderNode()->accept(*nv);
    }

    //STEP 5.执行GUI渲染
    for (auto it = passes.begin(); it != passes.end(); it++)
    {
        RenderPass* pass = it->second.get();

        ref_ptr<nv_gui> nv = new nv_gui();
        nv->renderer = mRenderer.get();
        nv->gc = mGraphicsContext.get();
        nv->window = mWindow.get();
        nv->viewport = pass->getCamera()->getViewPort();

        //mRoot->accept(*nv);
        pass->getRenderScene()->accept(*nv);
    }

    //STEP 6.交换缓冲区
    mGraphicsContext->swapBuffers();
}

#if 0
void RenderWindowEventCallback::onKey(int keycode, int keystate, int x, int y)
{
}

void RenderWindowEventCallback::onMouse(int type, int moustbutton, int moustbuttonstate, int x, int y)
{
}

void RenderWindowEventCallback::onTouch(unsigned int message, unsigned int cursor_id, float x, float y)
{
}

void RenderWindowEventCallback::onSize(int w, int h)
{
}

//void RenderWindowEventCallback::onFrame(float dt)
//{
//}
//
//void RenderWindowEventCallback::onDraw(float dt)
//{
//}
#endif

