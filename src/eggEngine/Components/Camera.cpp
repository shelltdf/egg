
#include <eggEngine/Components/Camera.h>

using namespace egg;

//轨道方式
//  在Y轴向上左右旋转
//  在X轴向上上下旋转

//围观模式
//  先抚养 再位移 再方位 无横滚

//FPS模式飞行模式
//  计算XYZ轴向
//  增量旋转 增量位移

//角色模式(场景节点混合模式)



void CameraHandler::onInput(nv_input* nv)
{
    SceneNode* sn = dynamic_cast<SceneNode*>(this->getNode());
    Camera* camera = sn->getComponent<Camera>();
    if (camera == 0) return;

    egg::WindowEvent we = nv->mWindowEvent;
    if (we.mWindowEventType == egg::WET_MOUSE)
    {
        if (we.mMouseType == egg::MT_Move)
        {
            mouse_last_x = mouse_x;
            mouse_last_y = mouse_y;
            mouse_x = we.x;
            mouse_y = we.y;
            mouse_move_x = mouse_x - mouse_last_x;
            mouse_move_y = mouse_y - mouse_last_y;
        }
        if (we.mMouseType == egg::MT_Click)
        {
            if (we.mMouseButton == egg::MB_Left)
            {
                mouse_pressed[0] = (we.mKeyState == egg::KS_DOWN);
            }
            if (we.mMouseButton == egg::MB_Middle)
            {
                mouse_pressed[1] = (we.mKeyState == egg::KS_DOWN);
            }
            if (we.mMouseButton == egg::MB_Right)
            {
                mouse_pressed[2] = (we.mKeyState == egg::KS_DOWN);
            }
        }
    }

    //左键旋转
    if (mouse_pressed[0])
    {
        Eigen::Matrix4f viewmatrix = camera->getViewMatrix();

        fangwei += mouse_move_x * 0.2;
        fuyang += mouse_move_y * 0.2;
    }

    //右键远近
    if (mouse_pressed[2])
    {
        juli += -mouse_move_y*0.1;
    }


    //
    Eigen::Affine3f af;
    af.setIdentity();
    
    af.rotate(Eigen::AngleAxisf(fuyang / 180.0*M_PI, Eigen::Vector3f(-1, 0, 0)));
    af.rotate(Eigen::AngleAxisf(fangwei / 180.0*M_PI, Eigen::Vector3f(0, -1, 0)));
    af.translate(Eigen::Vector3f(0, 0, juli));

    camera->setViewMatrix(af.matrix().inverse());

}


