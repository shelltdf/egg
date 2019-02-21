
#include <thread>
#include <chrono>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include <Eigen/Eigen>
#include <eggEngine/Matrix.h>

int main(int argc, char* argv[])
{
    //

    Eigen::Vector3f v0; //这里是不会初始化xyz的
    Eigen::Vector3f v1(1,2,3);
    Eigen::Vector3f v2;
    v2 << 3 , 2 , 1;


    //

    Eigen::Matrix4f m0; //这里是不会初始化的

    Eigen::Matrix4f m1;
    m1.setIdentity();

    //这种赋值方式是按照行主序方式，但是存储是列主序的。
    Eigen::Matrix4f m2;
    m2 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16;

    std::cout << m2(0, 0) << " ";
    std::cout << m2(0, 1) << " ";
    std::cout << m2(0, 2) << " ";
    std::cout << m2(0, 3) << " ";

    //从这个接口看来数据是行主序的排列，但是内存中是列。不知道为什么。


    //
    Eigen::Matrix4f m33;
    m33.setIdentity();

    m33(0, 0) = 0;
    m33(0, 1) = 1;
    m33(0, 2) = 2;
    m33(0, 3) = 3;
    m33(1, 0) = 4;
    m33(1, 1) = 5;
    m33(1, 2) = 6;
    m33(1, 3) = 7;
    m33(2, 0) = 8;
    m33(2, 1) = 9;
    m33(2, 2) = 10;
    m33(2, 3) = 11;
    m33(3, 0) = 12;
    m33(3, 1) = 13;
    m33(3, 2) = 14;
    m33(3, 3) = 15;

    //输出是行主序排列
    std::cout << "\n";
    std::cout << m33;
    std::cout << "\n";

    //列主序存储内容
    float* d = m33.data();
    for (size_t i = 0; i < 16; i++)
    {
        std::cout << d[i] << " ";
    }



    Eigen::Vector4f v8(1, 0, 0, 1);
    //Eigen::Vector4f v9 = v8 * m1; //这里会直接编译错误
    Eigen::Vector4f v10 = m1 * m1 * v8;

    //从结果看这里的顺序是 m * v


    //

    Eigen::AngleAxisf aa0;
    Eigen::AngleAxisf aa1(1.0,Eigen::Vector3f(1,2,3));

    Eigen::Quaternionf qq0;
    Eigen::Quaternionf qq1(1,2,3,4); //注意顺序是 w xyz


    //类型转换
    aa0 = qq1;
    qq0 = aa1;

    Eigen::Matrix3f m3; //旋转信息只能赋值给一个3x3矩阵
    m3 = qq0;
    m3 = aa1;

    qq0 = m3;
    aa1 = m3;


    //

    Eigen::Affine3f af;
    af.setIdentity();
    af.translate(Eigen::Vector3f(1, 1, 1));
    af.rotate(Eigen::AngleAxisf(90.0 / 180.0*M_PI, Eigen::Vector3f(0, 0, 1)));
    af.scale(0.5);
 
    //这里三个接口都是前乘
    //所以这里的顺序是 TRS

    Eigen::Matrix4f m_tx;
    m_tx = af.matrix();

    //这里打印的顺序是列主序的形式
    std::cout << "m_tx\n";
    std::cout << m_tx;
    std::cout << "\n";

    //这样打印的是行主序的结果
    float* dd = m_tx.data();
    for (size_t i = 0; i < 16; i++)
    {
        std::cout << dd[i] << " ";
    }



    //直接使用matrix计算空间变换
    Eigen::Matrix4f m_tx2 =
        Eigen::translate(1.0f, 1.0f, 1.0f)
        * Eigen::rotate(float(90.0 / 180.0*M_PI), 0.0f, 0.0f, 1.0f)
        * Eigen::scale(0.5f, 0.5f, 0.5f)
        ;

    std::cout << "m_tx2\n";
    std::cout << m_tx2;
    std::cout << "\n";
    
    //结论是计算过程是从后往前的顺序 也就是 M2 * M1 * M0 * V


    //转换类型
    Eigen::Affine3f af1(m_tx);
    Eigen::Matrix4f m_tx1(af.matrix());


    //动画曲线插值



    //投影矩阵计算



    //lookat计算



    //测试连续存储
    std::vector<Eigen::Vector3f> list;

    list.push_back(Eigen::Vector3f(1, 2, 3));
    list.push_back(Eigen::Vector3f(4, 5, 6));
    list.push_back(Eigen::Vector3f(7, 8, 9));

    float* ddd = list[0].data();
    for (size_t i = 0; i < 9; i++)
    {
        std::cout << ddd[i] << " ";
    }
    std::cout << "\n";

    //
    std::vector<Eigen::Matrix4f> list2;

    Eigen::Matrix4f m_0;
    m_0 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16;
    Eigen::Matrix4f m_1;
    m_1 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16;

    list2.push_back(m_0);
    list2.push_back(m_1);

    float* ddd2 = list2[0].data();
    for (size_t i = 0; i < 32; i++)
    {
        std::cout << ddd2[i] << " ";
    }
    std::cout << "\n";


    return 0;
}

