/*
 * A command line program for string range matching using different algorithms.
 * Features memory consumption and running time evalutions.
 *
 * Copyright (c) 2015 Jarno Leppänen
 */

#include "rmatch.hpp"
#include "algo_fwd.hpp"
#include "Crochemore.hpp"
#include "ZAlgorithm.hpp"
#include "SuffixArray.hpp"
#include "gs_count.hpp"
#include "naive_match.hpp"
#include "mallocate.hpp"
#include "timer.hpp"
#include "count_iterator.hpp"
#include "algo_wrap.hpp"

using namespace std;
using namespace rmatch;

#define RUN_ALGO(algo) \
if (q.ranged) { \
    if (in.count) { \
        algo##_match_range(in.t,q.p1,q.p2,counter(out)); \
    } else { \
        algo##_match_range(in.t,q.p1,q.p2,back_inserter(out)); \
    } \
} else { \
    if (in.count) { \
        algo##_match_less(in.t,q.p1,counter(out)); \
    } else { \
        algo##_match_less(in.t,q.p1,back_inserter(out)); \
    } \
}

void run_query(const input& in, const query& q)
{
    vector<size_t,mallocator<size_t>> out;
    if (in.debug) cerr << q.p1 << ' ' << q.p2 << endl;
    timer t(in.time);
    switch (in.m) {
        case C:     RUN_ALGO(crochemore);   break;
        case Z:     RUN_ALGO(z);            break;
        case SA:    RUN_ALGO(sa);           break;
        case NAIVE: RUN_ALGO(naive);        break;
        case KMP:   RUN_ALGO(kmp);          break;
        case GS:
            if (q.ranged)
                gs_count_range(in.t,q.p1,q.p2,in.k,back_inserter(out));
            else
                gs_count_less(in.t,q.p1,in.k,back_inserter(out));
            break;
    }
    t.stop();
    if (in.silent) return;
    for (auto v: out) printf("%ld\n",v);
}

int main(int argc, char *const argv[])
{
    input in;
    if (!init(argc, argv, in)) return in.ret;
    query q;
    while (in.queries->next(q)) {
        run_query(in,q);
    }
    return 0;
}
