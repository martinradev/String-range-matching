/*
 * Timer class implementation
 *
 * Copyright (c) 2015 Jarno Leppänen
 */

#include "timer.hpp"
#include <cstdio>

timer::timer():
    start(std::chrono::high_resolution_clock::now()),
    stopped(false),
    print(true)
{
}

timer::timer(bool print):
    start(std::chrono::high_resolution_clock::now()),
    stopped(false),
    print(print)
{
}

timer::~timer()
{
    stop();
    if (!print) return;
    printf("%f\n",span);
    fflush(stdout);
}

void timer::stop()
{
    using namespace std::chrono;
    auto stop = high_resolution_clock::now();
    if (stopped) return;
    span = duration_cast<duration<double>>(stop-start).count();
    stopped = true;
}
