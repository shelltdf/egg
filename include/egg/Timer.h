
#ifndef _TIMER_H_
#define _TIMER_H_

#if _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <time.h>

namespace egg
{

    /*
        //run
        Timer t;
        while( (!viewer.done()) && (!app->isDone()))
        {
        //计算更新时间 ms
        double dt = t.GetCounter();
        t.StartCounter();

        app->onPreFrame( &viewer, dt/1000.0 );
        viewer.frame();
        app->onPostFrame(&viewer, dt/1000.0 );
        }

        */
    class Timer
    {
    public:

        Timer()
            :CounterStart(0)
#if _WIN32
            , PCFreq(0.0)
#endif
        {
            StartCounter();
        }

        void StartCounter()
        {

#if _WIN32
            LARGE_INTEGER li;

            if (!QueryPerformanceFrequency(&li))
            {
                std::cout << "QueryPerformanceFrequency failed!\n";
            }
            PCFreq = double(li.QuadPart) / 1000.0;

            QueryPerformanceCounter(&li);
            CounterStart = li.QuadPart;
#endif//_WIN32

#if __GNUG__ || ADNROID

            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC, &ts);
            CounterStart = ts.tv_sec * 1000 + ts.tv_nsec / (1000 * 1000);

#endif //__CYGWIN__

        }

        // ms
        double GetCounter()
        {

#if _WIN32
            LARGE_INTEGER li;

            QueryPerformanceCounter(&li);
            return double(li.QuadPart - CounterStart) / PCFreq;
#endif//_WIN32

#if __GNUG__ || ADNROID

            timespec ts;
            clock_gettime(CLOCK_MONOTONIC, &ts);
            long long c = ts.tv_sec * 1000 + ts.tv_nsec / (1000 * 1000);
            return double(c - CounterStart);

#endif //__CYGWIN__

            return 1000.0 / 60.0; //如果系统不支持计时器
        }

    private:

#if _WIN32
        double PCFreq;
#endif

        long long CounterStart; //开始的时间点 毫秒
    };

}

#endif //_TIMER_H_

