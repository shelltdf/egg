
#include <eggEngine/Animation.h>

using namespace egg;

void Animation::save(Path filename, std::ostream& os, Serializer* ser) const
{
    Resource::save(filename, os, ser);

    //写入所有mIndex
    ser->writeNumber(os, mIndex.size()); //通道数
    for (auto it = mIndex.begin(); it != mIndex.end(); it++)
    {
        ser->writeString(os, it->first);   //通道名称

        ser->writeNumber(os, it->second);  //ID
    }

    //写入所有mData
    ser->writeNumber(os, mData.size()); //通道数
    for (auto it = mData.begin(); it != mData.end(); it++)
    {
        ser->writeNumber(os, (*it).size()); //通道的矩阵数量

        for (unsigned int i = 0; i < (*it).size(); i++)
        {
            for ( int mi = 0; mi < 16; mi++) //写入矩阵
            {
                ser->writeNumber(os, ((*it)[i])(mi)); //当前数字
            }
        }
    }

}

void Animation::load(Path filename, std::istream& is, Serializer* ser
    , ObjectManager* om, Referenced* rmg, bool load_ext)
{
    Resource::load(filename, is, ser, om, rmg, load_ext);

    //读所有mIndex
    int index_count = 0;
    ser->readNumber(is, index_count); //通道数
    for (int i = 0; i < index_count; i++)
    {
        std::string index_name;
        ser->readString(is, index_name); //通道名称

        int index_id = 0;
        ser->readNumber(is, index_id);  //ID

        mIndex[index_name] = index_id;
    }

    //读所有mData
    int data_count = 0;
    ser->readNumber(is, data_count); //通道数
    for (int i = 0; i < data_count; i++)
    {
        int data_len = 0;
        ser->readNumber(is, data_len); //当前通道元素数量

        mData.push_back(std::vector<Eigen::Matrix4f>(data_len));
        for (int j = 0; j < data_len; j++)
        {
            for (unsigned int mi = 0; mi < 16; mi++) //读取矩阵
            {
                ser->readNumber(is, mData[i][j](mi)); //当前数字
            }
        }
    }

}


unsigned int Animation::getFrameNum()
{
    unsigned int ret = 0;
    for (unsigned int i = 0; i < mData.size(); i++)
    {
        if (ret < mData[i].size()) ret = mData[i].size();
    }
    return ret;// / 16;
}

Eigen::Matrix4f Animation::interpolation(std::string name, float frame)
{
    unsigned int a = (unsigned int)(frame);       //插值A点
    unsigned int b = (unsigned int)(frame) + 1;   //插值B点
    float i = frame - a; //插值比例

    //printf("    %d %d\n",a,b);
    //printf("    %f\n",i);

    auto it = mIndex.find(name);
    if (it != mIndex.end())
    {
#if 0
        if (it->second < mData.size() && b * 16 < mData[it->second].size())
        {

            vmml::mat4f mat_a;
            vmml::mat4f mat_b;
            mat_a.set(&mData[it->second][a * 16], &mData[it->second][a * 16 + 15], false);
            mat_b.set(&mData[it->second][b * 16], &mData[it->second][b * 16 + 15], false);

            //线性插值
            vmml::mat4f mat_i = mat_a + (mat_b - mat_a) * i;
            //vmml::mat4f mat_i = mat_a;
            return mat_i;
        }
#else

        if (it->second < mData.size() && b < mData[it->second].size())
        {
            return mData[it->second][a]; //目前先不实现插值
        }
#endif
        
    }

    //无效名称
    return Eigen::Matrix4f::Identity();
}


