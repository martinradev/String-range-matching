/*
 * Auxiliary functions for Galil-Seiferas based string range match counting
 * algorithm.
 *
 * Copyright (c) 2015 Jarno Leppänen
 */

#ifndef GS_COUNT_DETAIL_HPP
#define GS_COUNT_DETAIL_HPP

#include <vector>

namespace rmatch {
namespace detail {

/* A tuple holding values (b,e,c) */
template <typename index_type>
struct bec_t {
    index_type b;
    index_type e;
    index_type c;
    bec_t(index_type b, index_type e, index_type c): b(b), e(e), c(c) {}
};

/* A tuple holding values (b,c) */
template <typename index_type>
struct bc_t {
    index_type b;
    index_type c;
    bc_t(index_type b, index_type c): b(b), c(c) {}
};

/* List Sp consisting of tuples (b,e,c) */
template <typename index_type>
struct s_p_t {
    typedef std::vector<bec_t<index_type>> type;
};

/* List Sn consisting of tuples (b,c) */
template <typename index_type>
struct s_n_t {
    typedef std::vector<bc_t<index_type>> type;
};

/* Add tuple (b,e,c) to list Sp */
template <typename index_type>
void add(typename s_p_t<index_type>::type& s_p, index_type b, index_type e,
        index_type c)
{
    s_p.emplace_back(b,e,c);
}

/* Add tuple (b,c) to list Sn */
template <typename index_type>
void add(typename s_n_t<index_type>::type& s_n, index_type b, index_type c)
{
    s_n.emplace_back(b,c);
}

/* Binary search for finding a tuple (b,..) in the list l with the maximum b so
   that b <= x. */
template <typename list_type, typename index_type>
typename list_type::size_type bin_search_b(const list_type& l, index_type x)
{
    typedef typename list_type::size_type size_type;
    size_type i = 0, n = l.size();
    for (size_type b = n/2; b >= 1; b /= 2) {
        while (i+b < n && l[i+b].b <= x) i += b;
    }
    return i;
}

/* Auxiliary 'pred'-function in the original paper. Outputs tuple (b,c) in the
   list Sn with the maximum b so that b <= x. The tuple is always assumed
   to be found. */
template <typename index_type>
void pred(const typename s_n_t<index_type>::type& s_n, index_type x,
        index_type& b, index_type& c)
{
    auto i = bin_search_b(s_n,x);
    b = s_n[i].b;
    c = s_n[i].c;
}

/* Auxiliary 'contains'-function in the original paper. Outputs tuple (b,e,c)
   in the list Sp with b <= x < e or (0,..) if such a tuple is not found.
   Tuples are assumed to be non-overlapping. */
template <typename index_type>
void contains(const typename s_p_t<index_type>::type& s_p, index_type x,
        index_type& b, index_type& e, index_type& c)
{
    if (s_p.empty()) {
        b = 0;
        return;
    }
    auto i = bin_search_b(s_p,x);
    if (x >= s_p[i].b && x < s_p[i].e) {
        b = s_p[i].b;
        e = s_p[i].e;
        c = s_p[i].c;
        return;
    }
    b = 0;
}

/* A tuple holding lists Sp and Sn. */
template <typename index_type>
struct s_t {
    typename s_p_t<index_type>::type p;
    typename s_n_t<index_type>::type n;
};

} // detail
} // rmatch

#endif // GS_COUNT_DETAIL_HPP
