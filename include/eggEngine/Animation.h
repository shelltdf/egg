#ifndef NIUBI_SG_ANIMATION_H
#define NIUBI_SG_ANIMATION_H

#include <eggEngine/Resource.h>

#include <egg/Math.h>
#include <eggEngine/Matrix.h>

namespace egg
{

    /*
    * Animation
    */

    class EGG_ENGINE_EXPORT Animation
        :public Resource
    {
    public:

        Animation() {}
        virtual ~Animation() {}

        virtual std::string ObjectClassName() override { return "animation"; }

        //读写函数
        virtual void save(Path filename, std::ostream& os, Serializer* ser) const override;
        virtual void load(Path filename, std::istream& is, Serializer* ser, ObjectManager* om, Referenced* rmg, bool load_ext) override;

        //查询数据内容
        //unsigned int getSize(){ return mIndex.size(); }
        std::map< std::string, unsigned int >& getIndex() { return mIndex; }
        std::vector< std::vector< Eigen::Matrix4f > > &   getData() { return mData; }

        //获取可能的frame数量
        //  按照道理应该是等长的，但是也存在特殊情况
        unsigned int getFrameNum();

        //查询插值结果
        Eigen::Matrix4f interpolation(std::string name, float frame);

    private:

        //名称对应ID表
        //   ID是从0开始顺序的表
        std::map< std::string, unsigned int > mIndex;

        //ID对应的动画序列
        //   暂时先存储矩阵信息 mat44 以后再考虑压缩成SRT数据
        std::vector< std::vector< Eigen::Matrix4f > > mData;
    };


}//namespace egg

#endif//NIUBI_SG_ANIMATION_H
