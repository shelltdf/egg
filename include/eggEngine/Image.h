#ifndef NIUBI_SG_IMAGE_H
#define NIUBI_SG_IMAGE_H

#include <eggEngine/Export.h>

#include <eggEngine/Resource.h>
#include <eggEngine/DataDefine.h>


namespace egg
{

    /*
    * image
    */

    class EGG_ENGINE_EXPORT Image
        :public Resource
    {
    public:

        Image()
            :mS(0), mT(0), mR(0)
            , mPixelFormat(egg::PF_RGBA), mDataType(egg::DT_UNSIGNED_BYTE)
        {}
        virtual ~Image() {}

        //virtual std::string getExt(){return "image";}
        virtual std::string ObjectClassName() override { return "image"; }

        virtual void save(egg::Path filename, std::ostream& os, Serializer* ser) const override;

        virtual void load(egg::Path filename, std::istream& is, Serializer* ser
            , ObjectManager* om, egg::Referenced* rmg, bool load_ext) override;


        //分配和销毁
        bool alloc(unsigned int s, unsigned int t, unsigned int r
            , egg::PixelFormat pf, egg::DataType dt, unsigned int size);
        bool deloc();

        unsigned int getS() { return mS; }
        void setS(unsigned int s) { mS = s; }

        unsigned int getR() { return mR; }
        void setR(unsigned int r) { mR = r; }

        unsigned int getT() { return mT; }
        void setT(unsigned int t) { mT = t; }

        egg::PixelFormat getPixelFormat() { return mPixelFormat; }
        void setPixelFormat(egg::PixelFormat pf) { mPixelFormat = pf; }

        egg::DataType getDataType() { return mDataType; }
        void setDataType(egg::DataType dt) { mDataType = dt; }

        unsigned char* getData();

        std::string getExternalPath() { return mExternalPath; }
        void setExternalPath(std::string str) { mExternalPath = str; }

    private:

        //外部链接文件
        //    比如 jpg dds
        //    这个属性如果存在有效字符 其他属性就等同于无效了
        std::string mExternalPath;

        unsigned int mS; //维度0 宽度
        unsigned int mT; //维度1 高度
        unsigned int mR; //维度2 z深度

        egg::PixelFormat mPixelFormat;      //分量数量  RGBA=4 RBG=3
        egg::DataType mDataType;            //每个通道的存储byte数  

        std::vector<unsigned char> mData;//存储对象
    };


}//namespace nbSG

#endif//NIUBI_SG_IMAGE_H
