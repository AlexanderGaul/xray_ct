#pragma once

#include <iostream>
#include <string>
#include <chrono>

class Timer
{
private:
    typedef std::chrono::system_clock Clock;
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::time_point<std::chrono::system_clock,std::chrono::system_clock::duration> TimePoint;

    std::string _label;
    TimePoint _begin;

public:
    Timer(std::string label)
        : _label(label)
    {
        start();
    }

    void start()
    {
        _begin = Clock::now();
    }

    /**
      * This method measures the time since the
      * timer was started.
      */
    void end()
    {
        TimePoint end = Clock::now();

        std::cout << _label << " \t"
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - _begin).count()
                  << " ms" << std::endl;
    }

    void setLabel(std::string label)
    {
        _label = label;
    }
};
