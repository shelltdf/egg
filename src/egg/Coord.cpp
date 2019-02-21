
#include<egg/Coord.h>

using namespace egg;

#if 0
void Coord2DNode::getMeAbsLTRB(float& l, float& t, float& r, float& b, float& cx, float& cy)
{
    //osgCore::BehSprite2D sp(this->getSceneNode());

    //得到Parent的绝对值
    float abs_x; float abs_y;
    float abs_w; float abs_h;
    float abs_cx; float abs_cy;
    this->getParentAbsXYWH(abs_x, abs_y, abs_w, abs_h, abs_cx, abs_cy);

    //修正坐标到屏幕比例
    //abs_x /= 2.0;
    //abs_y /= 2.0;

    //得到自己的绝对值
    float me_x = mX.getCoord(abs_w / 2.0);
    float me_y = mY.getCoord(abs_h / 2.0);
    float me_w = mW.getCoord(abs_w);
    float me_h = mH.getCoord(abs_h);
    float me_cx = mCX.getCoord(me_w / 2.0);
    float me_cy = mCY.getCoord(me_h / 2.0);

    //      父节点物理中心  + me x - me cx -宽度的一半（因为是L所有是减去）
    l = (abs_x - abs_cx) + me_x - me_cx - me_w / 2.0;
    b = (abs_y - abs_cy) + me_y - me_cy - me_h / 2.0; //同上
    //b = (abs_y - abs_h/2.0 - abs_cy) - me_cy - me_h/2.0; //同上
    r = l + (this->mRectShape[2] - this->mRectShape[0]); //x+w
    t = b + (this->mRectShape[1] - this->mRectShape[3]); //y+h 左下角原点

    //cx = me_cx;
    //cy = me_cy;

    //绝对值
    cx = l + (r - l) / 2.0 + me_cx;
    cy = b + (t - b) / 2.0 + me_cy;
}


void Coord2DNode::getParentAbsXYWH(float& x, float& y, float& w, float& h, float& cx, float& cy)
{
    SN_CLASS_TYPE* sn_parent = FnSceneNode(this->getSceneNode()).getParentSceneNode();
    //Scene* scene = dynamic_cast<Scene*>(parent);
    FnScene fnscene(sn_parent);

    //sprite
    if (sn_parent && (!fnscene.isScene()))
    {
        Behavior2DSprite* sprite = FnSceneNode(sn_parent).findBehvaoir<Behavior2DSprite>(0);
        if (sprite)
        {
            //计算绝对值
            float parent_x; float parent_y;
            float parent_w; float parent_h;
            sprite->getParentAbsXYWH(parent_x, parent_y, parent_w, parent_h, cx, cy);

#if 1           
            //
            //20140903 试验性修复GUI核心BUG
            //
            //建立如下结构：
            //Scene
            //|--sprite_0   [ px(500.0, 0.0) py(500.0, 0.0) w(0.0, 1.0) h(0.0, 1.0) ]
            //  |--sprite_1     [ px(0.0, 0.0) py(0.0, 0.0) w(0.0, 1.0) h(0.0, 1.0) ]
            //    |--pushButton    [ px(0.0, 0.0) py(0.0, 0.0) w(0.0, 1.0) h(0.0, 1.0) ] 
            //则pushButton的LBRT明显为错误值
            //因为原计算方法没有考虑父节点的pos值
            //

            x = getCoord(sprite->getX(), parent_w / 2.0) + parent_x - cx;
            y = getCoord(sprite->getY(), parent_h / 2.0) + parent_y - cy;

#else

            x = getCoord(sprite->getX(), parent_w / 2.0);
            y = getCoord(sprite->getY(), parent_h / 2.0);

#endif
            w = getCoord(sprite->getW(), parent_w);
            h = getCoord(sprite->getH(), parent_h);
            cx = getCoord(sprite->getCenterX(), w / 2.0);
            cy = getCoord(sprite->getCenterY(), h / 2.0);
        }
    }

    //stage
    else if (fnscene.isScene() || sn_parent == 0)
    {
        //parent = parent->SN_getParentSceneNode();
        //sn = dynamic_cast<SceneNode*>(parent);
        osg::Camera* cam = 0;
        if (sn_parent)
        {
            cam = dynamic_cast<osg::Camera*>(sn_parent->getParent(0));
        }
        else
        {
            cam = dynamic_cast<osg::Camera*>(this->getSceneNode()->getParent(0));
        }

        if (cam)
        {
            //Behavior2DStage*  stage  = sn->SN_findBehvaoir<Behavior2DStage>(0);
            //if( stage )
            //{
            double left;
            double right;
            double bottom;
            double top;
            double znear;
            double zfar;

            //osg::Camera* cam = dynamic_cast<osg::Camera*>(stage->getRenderView()->RV_getOSGNode());
            cam->getProjectionMatrixAsOrtho(left, right, bottom, top, znear, zfar);

            x = 0.0;
            y = 0.0;
            w = (right - left);///2.0;
            h = (top - bottom);///2.0;
            cx = 0.0;
            cy = 0.0;
            //}
        }
    }
    else
    {
    }
}

void Coord2DNode::reshape()
{
    reShape(mLastReShape[0], mLastReShape[1], mLastReShape[2], mLastReShape[3]);

    //便利子节点
    for (unsigned int i = 0; i<this->getSceneNode()->getNumChildren(); i++)
    {
        osg::Node* node = this->getSceneNode()->getChild(i);
        SN_CLASS_TYPE* sn = dynamic_cast<SN_CLASS_TYPE*>(node);
        if (sn)
        {
            osgCore::BehSprite2D spr(sn);
            if (spr.get())
            {
                spr->reShape();
            }
        }
    }
}

void Coord2DNode::reShape(float x, float y, float w, float h)
{
    //printf("Behavior2DSprite::reShape\n");

    FnSceneNode fnsn(this->getSceneNode());
    fnsn.setShapeDirty(true);

    mLastReShape[0] = x;
    mLastReShape[1] = y;
    mLastReShape[2] = w;
    mLastReShape[3] = h;


    //计算父节点绝对size
    float parent_x; float parent_y;
    float parent_w; float parent_h;
    float parent_cx; float parent_cy;
    getParentAbsXYWH(parent_x, parent_y, parent_w, parent_h, parent_cx, parent_cy);


    //osg::Group* sn = this->getSceneNode();
    //Behavior2DSprite* sprite = fnsn.findBehvaoir<Behavior2DSprite>(0);

    float new_w = getCoord( /*sprite->*/ getW(), parent_w);
    float new_h = getCoord( /*sprite->*/ getH(), parent_h);
    float new_cx = getCoord( /*sprite->*/ getCenterX(), new_w / 2.0);
    float new_cy = getCoord( /*sprite->*/ getCenterY(), new_h / 2.0);
    float new_x = getCoord( /*sprite->*/ getX(), parent_w / 2.0);// + (new_w/2.0 - new_cx);// + parent_x;
    float new_y = getCoord( /*sprite->*/ getY(), parent_h / 2.0);// + parent_y;

    //printf("name:%s\n",sprite->getName().c_str());
    //printf("     pp %f %f\n",parent_x,parent_y);
    //printf("     pr %f %f\n",parent_w,parent_h);
    //printf("     pc %f %f\n",parent_cx,parent_cy);
    //printf("     p %f %f\n",new_x,new_y);
    //printf("     r %f %f\n",new_w,new_h);
    //printf("     c %f %f\n",new_cx,new_cy);

    if (/*sprite->*/getMember())
    {
        //sprite->getMember()->reShape(
        //	new_x		    - new_cx - (new_w/2.0)
        //	,new_y + new_h	- new_cy - (new_h/2.0)
        //	,new_x + new_w  - new_cx - (new_w/2.0)
        //	,new_y          - new_cy - (new_h/2.0)
        //	);

        /*sprite->*/getMember()->reShape(
            -new_cx - (new_w / 2.0)
            , new_h - new_cy - (new_h / 2.0)
            , new_w - new_cx - (new_w / 2.0)
            , -new_cy - (new_h / 2.0)
        );



        mRectShape[0] = -new_cx - (new_w / 2.0);
        mRectShape[1] = new_h - new_cy - (new_h / 2.0);
        mRectShape[2] = new_w - new_cx - (new_w / 2.0);
        mRectShape[3] = -new_cy - (new_h / 2.0);

    }

    osg::Matrix mat =
        osg::Matrix::rotate(osg::DegreesToRadians(mRotate), osg::Vec3(0, 0, 1))
        * osg::Matrix::translate(new_x - parent_cx, new_y - parent_cy, 0)
        ;


    if (fnsn.getOSGNode() && fnsn.getOSGNode()->asTransform() && fnsn.getOSGNode()->asTransform()->asMatrixTransform())
    {
        fnsn.getOSGNode()->asTransform()->asMatrixTransform()->setMatrix(mat);
    }


    mRectShape[0] += new_x - parent_cx;
    mRectShape[1] += new_y - parent_cy;
    mRectShape[2] += new_x - parent_cx;
    mRectShape[3] += new_y - parent_cy;

    mRectPivots[0] = new_x - parent_cx;
    mRectPivots[1] = new_y - parent_cy;

    //检查该SN是否是ClipNode
    osg::ClipNode* clipNode = dynamic_cast<osg::ClipNode*>(this->getSceneNode());
    if (clipNode)
    {
        float l, t, r, b, cx, cy;
        this->getMeAbsLTRB(l, t, r, b, cx, cy);

        //std::cout << "l: " << l << std::endl;
        //std::cout << "t: " << t << std::endl;
        //std::cout << "r: " << r << std::endl;
        //std::cout << "b: " << b << std::endl;

        clipNode->getClipPlaneList().clear();

        clipNode->addClipPlane(new osg::ClipPlane(0, osg::Plane(0.0, 1.0, 0.0, -b)));
        clipNode->addClipPlane(new osg::ClipPlane(1, osg::Plane(0.0, -1.0, 0.0, t)));
        clipNode->addClipPlane(new osg::ClipPlane(2, osg::Plane(1.0, 0.0, 0.0, -l)));
        clipNode->addClipPlane(new osg::ClipPlane(3, osg::Plane(-1.0, 0.0, 0.0, r)));
    }
}
#endif

