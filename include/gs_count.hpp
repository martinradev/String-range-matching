/*
 * An implementation of a string range match counting algorithm in O(n+m) time
 * and O(log(m)) extra space based on an exact string matching algorithm by
 * Galil and Seiferas. Originally published in:
 *
 * J. Kärkkäinen, D. Kempa, S. Puglisi: String Range Matching.
 * Proceedings of the 25th Symposium on Combinatorial Pattern
 * Matching (CPM), pp. 232-241, 2014.
 * http://dx.doi.org/10.1007/978-3-319-07566-2_24
 *
 * Copyright (c) 2015 Jarno Leppänen
 */

#ifndef GS_COUNT_HPP
#define GS_COUNT_HPP

#include "gs_count_detail.hpp"

namespace rmatch {
namespace detail {

/* Find O(log(m)) scopes of the k-hrps of a string. This is the
   'PreCompute'-algorithm listed in Fig. 2 of the original paper. */
template <typename string_type, typename index_type>
s_t<index_type> gs_precompute(string_type y, index_type m, index_type k)
{
    using namespace std;
    s_t<index_type> s;
    add(s.n,index_type(1),index_type(0));
    index_type i = 1, last = 1, l = 0, count = 0;
    while (i < m) { // Invariant: count = |y_[0..i) ∩ [ɛ,y)|
        while (i+l < m && y[i+l] == y[l]) ++l;
        index_type b, e, c;
        contains(s.p,l,b,e,c);
        if (k*i <= i+l && b == 0) {
            b = 2*i; e = i+l+1; c = count;
            add(s.p,b,e,c); // y[0..i) is a new k-hrp
        }
        if (2*last <= i) {
            add(s.n,i,count);
            last = i;
        }
        if (i+l == m || y[i+l] < y[l]) ++count;
        if (b != 0) {
            count += c;
            i += b/2;
            l -= b/2;
        } else {
            pred(s.n,l/k+1,b,c);
            count += c;
            i += b;
            l = 0;
        }
    }
    return s;
}

} // detail

/**
 * Calculate the number of suffixes in a text that are lexicographically smaller
 * than a given pattern in linear time and O(log(m)) extra space.
 *
 * @param x Input text. (random access iterator)
 * @param n Length of the input text.
 * @param y Input pattern. (random access iterator)
 * @param m Length of the input pattern.
 * @param k Constant k used in calculating the k-hrps of the pattern. This
 * should be larger or equal to 3.
 * @return Number of matching suffixes in the text.
 */
template <typename string_type, typename index_type>
index_type gs_count_less(
        string_type x, index_type n,
        string_type y, index_type m,
        index_type k)
{
    using namespace std;
    using namespace rmatch::detail;
    s_t<index_type> s = gs_precompute(y,m,k);
    index_type count = 0, i = 0, l = 0;
    while (i < n) { // Invariant: count = |x_[0..i) ∩ [ɛ,y)|
        while (i+l < n && l < m && x[i+l] == y[l]) ++l;
        index_type b, e, c;
        contains(s.p,l,b,e,c);
        if (l < m && (i+l == n || x[i+l] < y[l])) ++count;
        if (b != 0) { // per(y[0..l)) = b/2
            // c = |Y_[1..b/2) ∩ [ɛ,y)| = |x_[i+1..i+b/2) ∩ [ɛ,y)|
            count += c;
            i += b/2;
            l -= b/2;
        } else { // per(y[0..l)) > l/k
            pred(s.n,l/k+1,b,c); // (⌊l/k⌋+1)/4 < b
            // c = |Y_[1..b) ∩ [ɛ,y)| = |x_[i+1..i+b) ∩ [ɛ,y)|
            count += c;
            i += b;
            l = 0;
        }
    }
    return count;
}

/**
 * Calculate the number of suffixes in a text x that are lexicographically
 * larger or equal to pattern b and smaller than pattern e; i.e. b <= x < e.
 *
 * @param x Input text. (random access iterator)
 * @param n Size of the input text.
 * @param b Lower bound pattern. (random access iterator)
 * @param m1 Size of the lower bound pattern.
 * @param e Upper bound pattern. (random access iterator)
 * @param m2 Size of the upper bound pattern.
 * @param k Constant k used in calculating the k-hrps of the patterns. This
 * should be larger or equal to 3.
 * @return Number of matching suffixes in the text.
 */
template <typename string_type, typename index_type>
index_type gs_count_range(
        string_type x, index_type n,
        string_type b, index_type m1,
        string_type e, index_type m2,
        index_type k)
{
    return
        gs_count_less(x,n,e,m2,k)-
        gs_count_less(x,n,b,m1,k);
}

/**
 * Calculate the number of suffixes in a text x that are lexicographically
 * larger or equal to pattern b and smaller than pattern e; i.e. b <= x < e.
 *
 * @param x Input text. (random access container)
 * @param b The lower bound pattern. (random access container)
 * @param e The upper bound pattern. (random access container)
 * @param k Constant k used in calculating the k-hrps of the patterns. This
 * should be larger or equal to 3.
 * @return Number of matching suffixes in the text.
 */
template <typename string_type>
typename string_type::size_type gs_count_range(
        const string_type& x,
        const string_type& b,
        const string_type& e,
        typename string_type::size_type k)
{
    return gs_count_range(
            x.begin(),x.size(),
            b.begin(),b.size(),
            e.begin(),e.size(),
            k);
}

} // rmatch

#endif // GS_COUNT_HPP
