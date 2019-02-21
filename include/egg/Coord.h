#ifndef _EGG_Coord_H_
#define _EGG_Coord_H_

#include<egg/Export.h>
#include<egg/Node.h>
#include<egg/NodeVisitor.h>

namespace egg
{

    //坐标变量Coordinate
    //	使用使用相对值为原点，绝对值为偏移量，来表达一个坐标的完整信息
    class Coord
    {
    public:
        Coord(float absolute = 0.0f, float relative = 0.0f)
            :mAbsolute(absolute), mRelative(relative) {}

        float getAbs() { return mAbsolute; }

        float getRel() { return mRelative; }

        float getCoord(float ref)
        {
            return ref * mRelative + mAbsolute;
        }

    private:

        float mAbsolute;
        float mRelative;
    };

#if 0
    class Coord2DNode
        :public egg::Node
    {
    public:

        Coord2DNode(const std::string& name = "node")
            :egg::Node(name) {}
        virtual ~Coord2DNode() {}

        //获取当前对象的上下左右绝对值
        void getMeAbsLTRB(float& l, float& t, float& r, float& b, float& cx, float& cy);

        //提示更新
        void reshape();

    protected:

        //坐标值
        Coord mX;
        Coord mY;
        Coord mW;
        Coord mH;
        Coord mCX;
        Coord mCY;
        float mRotate;

        //当前的LTRB  相对于父节点的绝对像素
        float mRectShape[4];  //本节点的LTRB到父节点中心的偏移
        float mRectPivots[2]; //本节点的中心点到父节点中心的偏移

    private:

        //获取父节点的上下左右绝对值
        void getParentAbsXYWH(float& x, float& y, float& w, float& h, float& cx, float& cy);

        //更新
        void reShape(float x, float y, float w, float h);
        float mLastReShape[4];//最后一次设置

    };
#endif


    class Coord2D
        //:public egg::Node
    {
    public:

        Coord2D()
        {
        }
        virtual ~Coord2D() {}

        //坐标值
        Coord mX;
        Coord mY;
        Coord mW;
        Coord mH;
        Coord mCX;
        Coord mCY;
        float mRotate;



    };


    class Coord2DInterface
    {
    public:
        Coord2DInterface()
        {
            mRectShape[0] = 0;
            mRectShape[1] = 0;
            mRectShape[2] = 0;
            mRectShape[3] = 0;

            mRectPivots[0] = 0;
            mRectPivots[1] = 0;
            mRectPivots[2] = 0;
            mRectPivots[3] = 0;

            mLastReShape[0] = 0;
            mLastReShape[1] = 0;
            mLastReShape[2] = 0;
            mLastReShape[3] = 0;
        }
        virtual ~Coord2DInterface() {}

        Coord2D& getCoord2D() { return mCoord2D; }

        virtual void onShape(float l, float t, float r, float b, float rot) = 0;

    protected:

        Coord2D mCoord2D;

    public:
        //当前的LTRB  相对于父节点的绝对像素

        //本节点的LTRB到父节点中心的偏移
        float mRectShape[4];

        //本节点的中心点到父节点中心的偏移
        float mRectPivots[2];

        //最后一次设置
        float mLastReShape[4];

    };

    class Bound
    {
    public:
        Bound(float left = 0, float top = 0, float right = 0, float botton = 0)
            :Left(left), Right(right), Top(top), Botton(botton)
        {}
        float Left;
        float Right;
        float Top;
        float Botton;
    };

    class Coord2DVisitor
        :public egg::NodeVisitor
    {
    public:
        Coord2DVisitor() {}
        virtual ~Coord2DVisitor() {}

        Coord2DInterface* getParent()
        {
            if (np.path.size() > 1)
            {
                Coord2DInterface* parent = dynamic_cast<Coord2DInterface*>(np.path[np.path.size() - 1]);
                if (parent)
                {
                    return parent;
                }
            }
            return 0;
        }

        //坐标转换为边界
        Bound Coord2Bound(
            Coord2D x, Coord2D y
            , Coord2D w, Coord2D h
            , Coord2D cx, Coord2D cy
        ) const
        {
            Bound bound;

            return bound;
        }

        //边界转换为坐标
        void Coord2Bound(Bound& bound
            , Coord2D& x, Coord2D& y
            , Coord2D& w, Coord2D& h
            , Coord2D& cx, Coord2D& cy
        ) const
        {
        }

        virtual void apply(egg::Node& node)
        {
            Coord2DInterface* coord2d = dynamic_cast<Coord2DInterface*>(&node);
            if (coord2d)
            {
                //testnode->num++;
                Coord2DInterface* parent = getParent();
                if (parent)
                {
                    //这里获取父节点的 LTRB 区域

                    //计算当前的坐标

                    
                }
                else
                {
                    //没有父节点就直接计算自己的坐标

                    float new_w = coord2d->getCoord2D().mW.getCoord(0);
                    float new_h = coord2d->getCoord2D().mH.getCoord(0);
                    float new_cx = coord2d->getCoord2D().mCX.getCoord(0);
                    float new_cy = coord2d->getCoord2D().mCY.getCoord(0);
                    float new_x = coord2d->getCoord2D().mX.getCoord(0);
                    float new_y = coord2d->getCoord2D().mY.getCoord(0);



                }


                //这里认为已经计算好了自己的坐标


                //转换坐标为 LTRB

                //发送给虚函数


            }
        }
    };



}

#endif //_EGG_Coord_H_

