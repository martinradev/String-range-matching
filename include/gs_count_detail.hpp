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

/* Auxiliary 'pred'-function in the original paper. Outputs tuple (b,c) in the
   list Sn with the maximum b for which b <= x. The list is assumed to be
   non-empty. */
template <typename index_type>
void pred(const typename s_n_t<index_type>::type& s_n, index_type x,
        index_type& b, index_type& c)
{
    typename s_n_t<index_type>::type::size_type i = 0;
    while (i+1 < s_n.size() && s_n[i+1].b <= x) ++i;
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
    for (const bec_t<index_type>& bec: s_p) {
        if (bec.e <= x) continue;
        if (bec.b > x) break;
        b = bec.b;
        e = bec.e;
        c = bec.c;
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
