
#include <thread>
#include <chrono>
#include <iostream>

//#define _USE_MATH_DEFINES
//#include <math.h>
//
//#include <Eigen/Eigen>
//#include <eggEngine/Matrix.h>

int main(int argc, char* argv[])
{
    char aaaa[256];
    std::cin >> aaaa;

    char* str = 0;

    for (size_t i = 0; i < 60*30; i++)
    {
        str = new char[10];

        //_sleep(1000);
        //std::this_thread::sleep_for(std::chrono::duration(1s));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("a");
    }

    return 0;
}

