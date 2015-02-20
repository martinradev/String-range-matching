/*
 * An implementation of a string range matching algorithm in O(n+m) time and
 * O(m) extra space based on Knuth-Morris-Pratt exact string matching algorithm.
 * Originally described on page 253 in:
 *
 * J. Kärkkäinen: Fast BWT in small space by blockwise suffix sorting.
 * Theoretical Computer Science 387, pp. 249–257, 2007.
 * http://dx.doi.org/10.1016/j.tcs.2007.07.018
 *
 * Copyright (c) 2015 Jarno Leppänen
 */

#ifndef KMP_MATCH_HPP
#define KMP_MATCH_HPP

#include <type_traits>
#include <vector>
#include <memory>
#include <algorithm>
#include <iterator>

namespace rmatch {
namespace detail {

/* Compute lcp(p,p[i..m)) for pattern p for all i ∈ [1,m]. This is the modified
   'SmallerSuffixes'-algorithm used as a precomputation step in the original
   paper. */
template <typename string_type, typename size_type, typename lcp_type>
void kmp_precompute(const string_type& p, size_type pm, lcp_type& lcp)
{
    typedef typename std::make_signed<size_type>::type index_type;
    index_type i = 0, j = -1, k = -1, l, m = pm, n = m-1;
    while (i <= n) {
        if (i > k) {
            k = i;
            l = 0;
        } else {
            l = lcp[i-j-1];
        }
        if (i+l == k) {
            while (l < m && k < n && p[l] == p[k+1]) {
                ++k;
                ++l;
            }
            j = i;
        } else if (i+l > k) {
            l = k-i;
            j = i;
        }
        lcp[i] = l;
        ++i;
    }
}

/* The common context for algorithm iterators sharing the same text and upper
   bound pattern. Using a pointer to this context avoids overhead when iterator
   is being copied. */
template <typename string_type, typename size_type>
struct kmp_match_less_iterator_context {
    typedef typename std::make_signed<size_type>::type index_type;
    const string_type t;
    const string_type p;
    const index_type n, m;
    std::vector<index_type> lcp;
    kmp_match_less_iterator_context(
            string_type t, size_type n,
            string_type p, size_type m):
        t(t), p(p), n(n), m(m), lcp(m)
    {
        kmp_precompute(p,m,lcp);
    }
};

} // detail

/**
 * @brief Input iterator class that outputs indices i of suffixes t[i..n)
 * that are lexicographically smaller than a given pattern.
 *
 * The class computes matching suffixes lazily index-by-index and can be used to
 * avoid having to save large intermediate index sequences in memory in case
 * only some of the results are going to be used. Moreover, the class adheres
 * to input iterator semantics and enables the use of generic STL algorithms
 * to filter and process the output further. Common requirements for input
 * iterators apply.
 *
 * The iterator is initialized with a text string and a pattern. On
 * initialization, an array containing lcp(p,p[i..m)) for all i ∈ [1,m] and the
 * first index of the matching suffix is computed. A subsequent index is
 * computed every time the iterator is incremented. The value of the computed
 * index is retrieved by dereferncing the iterator. The final position to
 * compare the iterator with is created by calling the end() method of an
 * initialized iterator.
 *
 * The underlying algorithm is based on Knuth-Morris-Pratt exact string matching
 * algorithm and iterates over all matching indices in O(n+m) time using O(m)
 * extra space.
 */

template <typename string_type, typename size_type>
class kmp_match_less_iterator {
public:
    typedef detail::kmp_match_less_iterator_context<string_type,size_type>
        context;
    typedef typename std::make_signed<size_type>::type index_type;

    /* typedefs required for stl iterators. */
    typedef index_type difference_type;
    typedef size_type value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef std::input_iterator_tag iterator_category;

    /**
     * Construct a string range matching algorithm iterator with the given text
     * t and a pattern p.
     *
     * @param t Input text. (random access iterator)
     * @param n Size of the input text.
     * @param p Input pattern. (random access iterator)
     * @param m Size of the input pattern.
     */
    kmp_match_less_iterator(
            string_type t, size_type n,
            string_type p, size_type m):
        ctx(std::make_shared<context>(t,n,p,m)),
        i(0), j(-1), k(-1) { next(); }

    /**
     * Inequality comparison between operators. Compared iterators are assumed
     * to share context (i.e. point to the same text and pattern). Calling
     * this operator with iterators not sharing common context results in
     * undefined behavior.
     *
     * @param o Iterator that this iterator is to be compared with.
     * @return True, if iterators are in different positions in the text.
     */
    bool operator!=(const kmp_match_less_iterator& o) const { return i != o.i; }

    /**
     * Equality comparison between operators. Functionally equivalent to calling
     * !(this != o).
     *
     * @param o Iterator that this iterator is to be compared with.
     * @return True, if iterators are at the same position in the text.
     */
    bool operator==(const kmp_match_less_iterator& o) const { return i == o.i; }

    /**
     * Retrieve the previously computed index of the suffix t[i..n) in the text
     * for which t[i..n) is lexicographically smaller the pattern p given
     * on iterator construction.
     *
     * @return Index of the previously computed matching suffix index.
     */
    value_type operator*() const { return v; }

    /**
     * Compute the next position i in the text t for which the suffix t[i..n)
     * is lexicographically smaller than the pattern p given on iterator
     * construction. If such a position is not found, the index will be set
     * at a position one past the length of the text given on iterator
     * construction.
     */
    kmp_match_less_iterator& operator++() { next(); return *this; }

    /**
     * Compute the next position i in the text t for which the suffix t[i..n)
     * is lexicographically smaller than the pattern p given on iterator
     * construction. If such a position is not found, the index will be set
     * at a position one past the length of the text given on iterator
     * construction.
     *
     * This is a post-increment version which does not return a reference to
     * this iterator to avoid copying overhead.
     */
    void operator++(int) { next(); }

    /**
     * Create an end position iterator that can be used to determine if this
     * iterator has reched text end.
     *
     * @return Created end position iterator for this iterator.
     */
    kmp_match_less_iterator end() const
    {
        return kmp_match_less_iterator(ctx->n+1);
    }

private:
    /**
     * Compute the next position i in the text t for which the suffix t[i..n)
     * is lexicographically smaller than the pattern p given on iterator
     * construction. If such a position is not found, the index will be set
     * at a position one past the length of the text given on iterator
     * construction.
     */
    void next()
    {
        const index_type n = ctx->n;
        const index_type m = ctx->m;
        const string_type t = ctx->t;
        const string_type p = ctx->p;
        const std::vector<index_type>& lcp = ctx->lcp;

        using namespace std;

        while (i <= n) {
            // t[i,n) is the suffix being compared to p
            // t[j,k) is a previously found prefix of p with maximal k
            // Pre-condition: k−j = lcp(t[j,n),p)
            if (i > k) {
                k = i;
                l = 0;
            } else {
                // lcp[i] = lcp(p,p[i+1,m))
                l = lcp[i-j-1];
            }
            if (i+l == k) {
                while (l < m && k < n && p[l] == t[k]) {
                    ++k;
                    ++l;
                }
                j = i;
            } else if (i+l > k) {
                l = k-i;
                j = i;
            }
            // Post-condition: l = lcp(t[i,n),p)
            if (l != m && (i + l == n || t[i+l] < p[l])) {
                v = i++;
                return;
            }
            ++i;
        }
    }

    /**
     * Construct a dummy iterator with the index value i. This can be used to
     * construct an end position iterator by giving the total length of the text
     * as a parameter.
     *
     * Iterator created with this constructor should only be used for
     * comparison. Calling other methods will result in undefined behavior.
     *
     * @param i Length of the text of the iterator this iterator is to be
     * compared with.
     */
    kmp_match_less_iterator(size_type i): i(i) {}

    const std::shared_ptr<const context> ctx;
    index_type i, j, k, l;
    size_type v;
};

/**
 * Calculate indices i of suffixes t[i..n) of text t that are lexicographically
 * smaller than pattern p.
 *
 * @param t Input text. (random access iterator)
 * @param n Size of the input text.
 * @param p Input pattern. (random access iterator)
 * @param m Size of the input pattern.
 * @param r Destination index sequence. (output iterator)
 */
template <typename string_type, typename size_type, typename output_iterator>
void kmp_match_less(
        string_type t, size_type n,
        string_type p, size_type m,
        output_iterator r)
{
    auto i = kmp_match_less_iterator<string_type, size_type>(t,n,p,m);
    std::copy(i,i.end(),r);
}

/**
 * Calculate indices i of suffixes t[i..n) of text t that are lexicographically
 * smaller than pattern p.
 *
 * @param t Input text. (random access container)
 * @param p Input pattern. (random access iterator)
 * @param r Destination index sequence. (output iterator)
 */
template <typename string_type, typename output_iterator>
void kmp_match_less(
        const string_type& t,
        const string_type& p,
        output_iterator r)
{
    kmp_match_less(t.begin(),t.size(),p.begin(),p.size(),r);
}

/**
 * Calculate indices i of suffixes t[i..n) of text t that are lexicographically
 * larger or equal to pattern l and smaller than pattern u; i.e. l <= t < u.
 *
 * @param t Input text. (random access iterator)
 * @param n Size of the input text.
 * @param l Lower bound pattern. (random access iterator)
 * @param lm Size of the lower bound pattern.
 * @param u Upper bound pattern. (random access iterator)
 * @param um Size of the upper bound pattern.
 * @param r Destination index sequence. (output iterator)
 */
template <typename string_type, typename size_type, typename output_iterator>
void kmp_match_range(
        string_type t, size_type n,
        string_type l, size_type lm,
        string_type u, size_type um,
        output_iterator r)
{
    auto li = kmp_match_less_iterator<string_type, size_type>(t,n,l,lm);
    auto ui = kmp_match_less_iterator<string_type, size_type>(t,n,u,um);
    std::set_difference(ui,ui.end(),li,li.end(),r);
}

/**
 * Calculate indices i of suffixes t[i..n) of text t that are lexicographically
 * larger or equal to pattern l and smaller than pattern u; i.e. l <= t < u.
 *
 * @param t Input text. (random access container)
 * @param l Lower bound pattern. (random access container)
 * @param u Upper bound pattern. (random access container)
 * @param r Destination index sequence. (output iterator)
 */
template <typename string_type, typename output_iterator>
void kmp_match_range(
        const string_type& t,
        const string_type& l,
        const string_type& u,
        output_iterator r)
{
    kmp_match_range(
            t.begin(),t.size(),
            l.begin(),l.size(),
            u.begin(),u.size(),
            r);
}

} // rmatch

#endif // KMP_MATCH_HPP
