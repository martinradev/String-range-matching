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
    method m;
    size_t k;
    bool silent;
    bool time;
    bool count;
    int ret;
    bool extend;
    bool debug;

    input():
        m(NAIVE),
        k(3),
        silent(false),
        time(false),
        count(false),
        ret(0),
        extend(false),
        debug(false)
    {}

    // parse pattern parameter
    mstring pattern(const char *p) const;
};

struct query {
    bool ranged;
    mstring p1;
    mstring p2;
    query(): ranged(false) {}
    query(const mstring& p1):
        ranged(false), p1(p1) {}
    query(const mstring& p1, const mstring& p2):
        ranged(true), p1(p1), p2(p2) {}
};

struct qstream {
    virtual ~qstream() {}
    virtual bool next(query& q) = 0;
};

bool init(int argc, char *const argv[], input& in);

#endif // RMATCH_HPP
