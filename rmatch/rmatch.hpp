/*
 * Common definitions.
 *
 * Copyright (c) 2015 Jarno Leppänen
 */

#ifndef RMATCH_HPP
#define RMATCH_HPP

#include "mallocate.hpp"
#include <string>
#include <memory>

/* Different algorithm types. */
enum method {
    NAIVE,
    GS,
    C,
    Z,
    SA,
    KMP
};

/* The default string type for input using a custom allocator that allows
   ignoring input text allocations. */
typedef std::basic_string<char,std::char_traits<char>,mallocator<char>> mstring;

struct qstream;

/* Input data for algorithms. */
struct input {
    mstring t;
    std::shared_ptr<qstream> queries;
    method m    = NAIVE;
    size_t k    = 3;
    bool silent = false;
    bool time   = false;
    bool count  = false;
    int ret     = 0;
    bool extend = false;
    bool debug  = false;

    // parse pattern parameter
    mstring pattern(const char *p) const;
};

struct query {
    bool ranged;
    mstring p1;
    mstring p2;
};

struct qstream {
    virtual ~qstream() {}
    virtual bool next(query& q) = 0;
};

bool init(int argc, char *const argv[], input& in);

#endif // RMATCH_HPP
