#ifndef RANGE_COUNT_DETAIL_HPP
#define RANGE_COUNT_DETAIL_HPP

#include <vector>

namespace str {
namespace detail {

template <typename index_type>
struct bec_t {
    index_type b;
    index_type e;
    index_type c;
    bec_t(index_type b, index_type e, index_type c): b(b), e(e), c(c) {}
};

template <typename index_type>
struct bc_t {
    index_type b;
    index_type c;
    bc_t(index_type b, index_type c): b(b), c(c) {}
};

template <typename index_type> using s_p_t = std::vector<bec_t<index_type>>;
template <typename index_type> using s_n_t = std::vector<bc_t<index_type>>;

template <typename index_type>
void add(s_p_t<index_type>& s_p, index_type b, index_type e, index_type c)
{
    s_p.emplace_back(b,e,c);
}

template <typename index_type>
void add(s_n_t<index_type>& s_n, index_type b, index_type c)
{
    s_n.emplace_back(b,c);
}

template <typename list_type, typename index_type>
typename list_type::size_type bin_search_b(const list_type& l, index_type x)
{
    typedef typename list_type::size_type size_type;
    size_type n = l.size(), i = 0;
    for (size_type b = n/2; b >= 1; b /= 2) {
        while (i+b < n && l[i+b].b <= x) i += b;
    }
    return i;
}

template <typename index_type>
void pred(const s_n_t<index_type>& s_n, index_type x,
        index_type *b, index_type *c)
{
    auto i = bin_search_b(s_n,x);
    *b = s_n[i].b;
    *c = s_n[i].c;
}

template <typename index_type>
void contains(const s_p_t<index_type>& s_p, index_type x,
        index_type *b, index_type *e, index_type *c)
{
    if (s_p.empty()) {
        *b = 0;
        return;
    }
    auto i = bin_search_b(s_p,x);
    if (s_p[i].b <= x && x < s_p[i].e) {
        *b = s_p[i].b;
        *e = s_p[i].e;
        *c = s_p[i].c;
        return;
    }
    *b = 0;
}

template <typename index_type>
struct s_t {
    s_p_t<index_type> p;
    s_n_t<index_type> n;
};

} // detail
} // str

#endif
