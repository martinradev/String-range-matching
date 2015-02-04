#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

struct timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    double span;
    bool stopped;
    bool print;

    timer();
    timer(bool print);
    ~timer();
    void stop();
};

#endif
