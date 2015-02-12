/*
 * A class enabling accurate timing using c++11 chrono features.
 *
 * Copyright (c) 2015 Jarno Leppänen
 */

#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

/**
 * Timer class enables accurate time measurement. Timing starts on object
 * construction stops when stop() is called or when the object is destroyed.
 */

class timer {
public:

    /**
     * Contruct a timer. The timer will print the measured time span when it is
     * destroyed.
     */
    timer();

    /**
     * Construct a timer. Timer printing the measured time span can be
     * controlled with the parameter print.
     *
     * @param print If print is true, the measured time span is printed when the
     * object is destroyed.
     */
    timer(bool print);

    /**
     * Destroyes the timer object and prints the measured time span if the
     * object was configured to do so.
     */
    ~timer();

    /**
     * Explicitly stops time measurement before object destruction.
     */
    void stop();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    double span;
    bool stopped;
    bool print;
};

#endif
