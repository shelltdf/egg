
#include <eggEngine/Image.h>

#include <eggEngine/ResourceManager.h> //for load
#include <eggEngine/ImageIO.h>


using namespace egg;

void Image::save(egg::Path filename, std::ostream& os, Serializer* ser) const
{
    Resource::save(filename, os, ser);
    ser->writeNumber(os, mS);
    ser->writeNumber(os, mT);
    ser->writeNumber(os, mR);
    ser->writeNumber(os, mPixelFormat);
    ser->writeNumber(os, mDataType);
    ser->writeBinary(os, mData);
    ser->writeString(os, mExternalPath);
}

void Image::load(egg::Path filename, std::istream& is, Serializer* ser, ObjectManager* om
    , egg::Referenced* rmg, bool load_ext)
{
    Resource::load(filename, is, ser, om, rmg, load_ext);
    ser->readNumber(is, mS);
    ser->readNumber(is, mT);
    ser->readNumber(is, mR);
    ser->readNumber(is, (int&)mPixelFormat);
    ser->readNumber(is, (int&)mDataType);
    ser->readBinary(is, mData);
    ser->readString(is, mExternalPath);


    if (!load_ext) return;


    std::string n = filename.getNativePath();
    if (mExternalPath.size() > 0 && mData.size() == 0)
    {
        egg::Path image_filename = filename;
        image_filename.pop();
        image_filename.push(mExternalPath);
        //printf("image file = %s\n",image_filename.getNativePath().c_str());

        ResourceManager* resmanager = dynamic_cast<ResourceManager*>(rmg);
        if (resmanager)
        {
            std::vector<char> buffer;
            unsigned int len = 0;
            resmanager->readFile(image_filename, buffer, len);

            if (len == 0)
            {
                return;
            }

            bool b = loadImage(image_filename, buffer, len
                , mS, mT, mR
                , mPixelFormat, mDataType
                , mData);

        }//if(resmanager)
    }//if(mExternalPath.size()>0 && mData.size() == 0)
}


bool Image::alloc(unsigned int s, unsigned int t, unsigned int r
    , egg::PixelFormat pf, egg::DataType dt, unsigned int size)
{
    mS = s; mT = t; mR = r;
    mPixelFormat = pf;
    mDataType = dt;
    mData.resize(size);
    return true;
}

bool Image::deloc()
{
    mS = 0; mT = 0; mR = 0;
    //mPixelFormat = NB_RGBA;
    //mDataType = NB_UNSIGNED_BYTE;
    mData.clear();
    return true;
}

unsigned char* Image::getData()
{
    if (mData.size() > 0) return &mData[0];
    return 0;
}

