/*
 * A naive implementation of a string range matching algorithm in O(nm) time.
 *
 * Copyright (c) 2015 Jarno Leppänen
 */

#ifndef NAIVE_MATCH_HPP
#define NAIVE_MATCH_HPP

#include <algorithm>
#include <string>
#include <iterator>
#include <type_traits>

namespace rmatch {

/**
 * Calculate indices i of suffixes t[i..n) of text t that are lexicographically
 * larger or equal to pattern l and smaller than pattern u; i.e. l <= t < u.
 *
 * @param t Input text. (input iterator)
 * @param te End position of input text. (input iterator)
 * @param l Lower bound pattern. (input iterator)
 * @param le End position of the lower bound pattern. (input iterator)
 * @param u Upper bound pattern. (input iterator)
 * @param le End position of the upper bound pattern. (input iterator)
 * @param r Destination index sequence. (output iterator)
 */
template <typename input_iterator, typename output_iterator>
void naive_match_range(
        input_iterator t, input_iterator te,
        input_iterator l, input_iterator le,
        input_iterator u, input_iterator ue,
        output_iterator r)
{
    using namespace std;
    typedef typename iterator_traits<input_iterator>::difference_type diff;
    typedef typename make_unsigned<diff>::type size_type;
    /* Iterate over all suffixes and compare to patterns lexicographically */
    for (size_type i = 0; t != te; ++t, ++i, ++r) {
        if (
                 lexicographical_compare(t,te,u,ue) &&
                !lexicographical_compare(t,te,l,le)) *r = i;
    }
}

/**
 * Calculate indices i of suffixes t[i..n) of text t that are lexicographically
 * larger or equal to pattern l and smaller than pattern u; i.e. l <= t < u.
 *
 * @param t Input text. (container)
 * @param l Lower bound pattern. (container)
 * @param u Upper bound pattern. (container)
 * @param r Destination index sequence. (output iterator)
 */
template <typename string_type, typename output_iterator>
void naive_match_range(
        const string_type& t,
        const string_type& b,
        const string_type& e,
        output_iterator r)
{
    naive_match_range(
            t.begin(), t.end(),
            b.begin(), b.end(),
            e.begin(), e.end(),
            r);
}

} // rmatch

#endif
