#ifndef TIMER_H
#define TIMER_H

#include <chrono>

using std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::duration;
using std::chrono::milliseconds;

class Timer
{
    private:
        const static double PERIOD;
        time_point<steady_clock> m_start;
    public:
        void start() { m_start = steady_clock::now(); }
        double elapsed() { return (steady_clock::now() - m_start).count() * PERIOD; }
        static std::string units() { return "ms"; }
};

// Seconds
// const double Timer::PERIOD = 0.000000001;
// Ms
const double Timer::PERIOD = 0.000001;

#endif