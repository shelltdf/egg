
//#include <eggEngine/VirtualFileSystem.h>
#include<Eigen/Core>
#include<Eigen/Geometry>

TEST(eggEngine, Matrix)
{
    Eigen::Matrix4f m;
    Eigen::Vector4f v;

    //v = v * m;
    v = m * m * v;


    //Eigen::Transform t;
    Eigen::Affine3f a;


}
