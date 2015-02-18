#ifndef ALGO_WRAP_HPP
#define ALGO_WRAP_HPP

#include "Crochemore.hpp"
#include "ZAlgorithm.hpp"
#include "SuffixArray.hpp"
#include <boost/dynamic_bitset.hpp>

namespace rmatch {

template <typename output_iterator>
void copy_indices(const boost::dynamic_bitset<>& b, output_iterator r)
{
    for (auto i = b.find_first(); i != b.npos; i = b.find_next(i)) *r++ = i;
}

template <typename string_type, typename output_iterator>
void crochemore_match_range(
        const string_type& t,
        const string_type& l,
        const string_type& u,
        output_iterator r)
{
    stringRangeMatch(t,l,u,r);
}

template <typename string_type, typename output_iterator>
void crochemore_match_less(
        const string_type& t,
        const string_type& u,
        output_iterator r)
{
    copy_indices(lowerBound(t,u),r);
}

template <typename string_type, typename output_iterator>
void z_match_range(
        const string_type& t,
        const string_type& l,
        const string_type& u,
        output_iterator r)
{
    stringRangeMatchZ(t,l,u,r);
}

template <typename string_type, typename output_iterator>
void z_match_less(
        const string_type& t,
        const string_type& u,
        output_iterator r)
{
    copy_indices(lowerBoundZ(t,u),r);
}

template <typename string_type, typename output_iterator>
void sa_match_range(
        const string_type& t,
        const string_type& l,
        const string_type& u,
        output_iterator r)
{
    rangeQuery(t,l,u,r);
}

template <typename string_type, typename output_iterator>
void sa_match_less(
        const string_type& t,
        const string_type& u,
        output_iterator r)
{
    SuffixArray<string_type> sa(t);
    size_t e = sa.lowerBound(u);
    for (size_t i = 0; i < e; ++i) *r++ = sa.m_array[i];
}

} // rmatch

#endif // ALGO_WRAP_HPP
