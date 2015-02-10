#ifndef KMP_MATCH_HPP
#define KMP_MATCH_HPP

#include <type_traits>
#include <vector>
#include <memory>
#include <algorithm>
#include <iterator>

namespace str {
namespace detail {

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

template <typename string_type, typename size_type>
struct kmp_match_iterator_context {
    typedef typename std::make_signed<size_type>::type index_type;
    const string_type& t;
    const string_type& p;
    const index_type n, m;
    std::vector<index_type> lcp;
    kmp_match_iterator_context(
            const string_type& t, size_type n,
            const string_type& p, size_type m):
        t(t), p(p), n(n), m(m), lcp(m)
    {
        kmp_precompute(p,m,lcp);
    }
};

} // detail

template <typename string_type, typename size_type>
struct kmp_match_iterator {

    typedef detail::kmp_match_iterator_context<string_type,size_type> context;
    typedef typename std::make_signed<size_type>::type index_type;

    typedef index_type difference_type;
    typedef size_type value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef std::input_iterator_tag iterator_category;

    std::shared_ptr<context> ctx;
    index_type i, j, k, l;
    size_type v;

    kmp_match_iterator(size_type i): i(i) {}
    kmp_match_iterator(
            const string_type& t, size_type n,
            const string_type& p, size_type m):
        ctx(std::make_shared<context>(t,n,p,m)),
        i(0), j(-1), k(-1) { next(); }

    bool operator!=(const kmp_match_iterator& o) const { return i != o.i; }
    bool operator==(const kmp_match_iterator& o) const { return i == o.i; }
    value_type operator*() const { return v; }
    void operator++(int) { next(); }
    kmp_match_iterator& operator++() { next(); return *this; }

    void next()
    {
        const index_type n = ctx->n;
        const index_type m = ctx->m;
        const string_type& t = ctx->t;
        const string_type& p = ctx->p;
        const std::vector<index_type>& lcp = ctx->lcp;

        while (i <= n) {
            // t[i,n) is the suffix being compared to p
            // t[j,k) is a previously found prefix of p with maximal k
            // Pre-condition: k−j = lcp(t[j,n),p)
            if (i > k) {
                k = i;
                l = 0;
            } else {
                l = lcp[i-j];
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
            // Post-condition: l = lcp(T[i,n),P)
            if (l != m && (i + l == n || t[i+l] < p[l])) {
                v = i++;
                return;
            }
            ++i;
        }
    }

    kmp_match_iterator end() const
    {
        return kmp_match_iterator(ctx->n+1);
    }
};

template <typename string_type, typename size_type, typename output_iterator>
void kmp_match(
        const string_type& t, size_type n,
        const string_type& b, size_type m1,
        const string_type& e, size_type m2,
        output_iterator out)
{
    using namespace std;
    auto bi = kmp_match_iterator<string_type, size_type>(t,n,b,m1);
    auto ei = kmp_match_iterator<string_type, size_type>(t,n,e,m2);
    std::set_difference(ei,ei.end(),bi,bi.end(),out);
}

template <typename string_type, typename output_iterator>
void kmp_match(
        const string_type& t,
        const string_type& b,
        const string_type& e,
        output_iterator out)
{
    kmp_match(t,t.size(),b,b.size(),e,e.size(),out);
}

} // str

#endif // KMP_MATCH_HPP
