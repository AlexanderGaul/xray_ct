#pragma once

#include <iostream>
#include <string>
#include <chrono>

class Timer
{
private:
    typedef std::chrono::high_resolution_clock Clock;

    std::string _label;
    std::chrono::system_clock::time_point _begin;

public:
    Timer(std::string label)
        : _label(label)
    {
        _begin = Clock::now();
    }

    /**
      * The destructor of timer measures the time since the
      * timer was created.
      */
    ~Timer()
    {
        std::chrono::system_clock::time_point end = Clock::now();

        std::cout << _label << "\t"
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - _begin).count()
                  << " ms" << std::endl;
    }
};
