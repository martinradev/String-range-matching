/*
 * Explicitly instantiate selected algorithm functions to reduce compilation
 * time.
 */

#include "rmatch.hpp"
#include "algo_fwd.hpp"
#include "kmp_match.hpp"
#include "count_iterator.hpp"
#include <vector>
#include <iterator>

using namespace std;

typedef vector<size_t,mallocator<size_t>> mvector;

typedef count_iterator<mvector> count_it;
typedef back_insert_iterator<mvector> report_it;

template void rmatch::kmp_match_less<mstring, count_it>(
        const mstring& t,
        const mstring& p,
        count_it r);

template void rmatch::kmp_match_less<mstring, report_it>(
        const mstring& t,
        const mstring& p,
        report_it r);

template void rmatch::kmp_match_range<mstring, count_it>(
        const mstring& t,
        const mstring& l,
        const mstring& u,
        count_it r);

template void rmatch::kmp_match_range<mstring, report_it>(
        const mstring& t,
        const mstring& l,
        const mstring& u,
        report_it r);
