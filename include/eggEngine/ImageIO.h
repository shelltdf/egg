
#ifndef _EGG_ENGINE_IMAGE_IO_H_
#define _EGG_ENGINE_MATRIX_H_

#include <eggEngine/DataDefine.h>

extern "C"
{
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
}


static bool loadImage(egg::Path image_filename, std::vector<char>& buffer, int len
    , unsigned int& s, unsigned int& t, unsigned int& r
    , egg::PixelFormat& pf, egg::DataType& dt
    , std::vector<unsigned char>& out_data
)
{
    int x = 0;
    int y = 0;
    int comp = 0; //实际读取的通道数

    stbi_uc* data = stbi_load_from_memory((stbi_uc const *)&buffer[0], len, &x, &y, &comp, 4/*一般请求4个通道*/);
    if (data)
    {
        printf("load image file = %s\n", image_filename.getPathMel().c_str());

        s = x;
        t = y;
        t = 1;

        int pixel_in_byte = 0; //一个像素的字节数
        if (comp == 1)
        {
            pixel_in_byte = 1;
            pf = egg::PF_LUMINANCE;
            dt = egg::DT_UNSIGNED_BYTE;
        }
        if (comp == 3)
        {
            pixel_in_byte = 3;
            pf = egg::PF_RGB;
            dt = egg::DT_UNSIGNED_BYTE;
        }
        if (comp == 4)
        {
            pixel_in_byte = 4;
            pf = egg::PF_RGBA;
            dt = egg::DT_UNSIGNED_BYTE;
        }

        if (pixel_in_byte > 0)
        {
            out_data.resize(s*t*r*pixel_in_byte);
            memcpy(&out_data[0], data, out_data.size());

            //for (int i = 0; i < mData.size(); i+=pixel_in_byte)
            //{
            //    mData[i+2] = 0xff;//data[i];
            //}
        }

        free(data);
        data = 0;

        return true;
    }//if

    return false;
}



#endif // !_EGG_ENGINE_IMAGE_IO_H_
