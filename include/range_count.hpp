#ifndef RANGE_COUNT_HPP
#define RANGE_COUNT_HPP

#include "range_count_detail.hpp"

namespace str {
namespace detail {

template <typename string_type, typename index_type>
s_t<index_type> precompute(const string_type& y, index_type m, index_type k)
{
    using namespace std;
    s_t<index_type> s;
    add(s.n,index_type(1),index_type(0));
    index_type i = 1, last = 1, l = 0, count = 0;
    while (i < m) { // Invariant: count = |y_[0..i) ∩ [ɛ,y)|
        while (i+l < m && y[i+l] == y[l]) ++l;
        index_type b, e, c;
        contains(s.p,l,&b,&e,&c);
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
            pred(s.n,l/k+1,&b,&c);
            count += c;
            i += b;
            l = 0;
        }
    }
    return s;
}

} // detail

template <typename string_type, typename index_type>
index_type one_sided_range_count(
        const string_type& x, index_type n,
        const string_type& y, index_type m,
        index_type k)
{
    using namespace std;
    using namespace str::detail;
    s_t<index_type> s = precompute(y,m,k);
    index_type count = 0, i = 0, l = 0;
    while (i < n) { // Invariant: count = |x_[0..i) ∩ [ɛ,y)|
        while (i+l < n && l < m && x[i+l] == y[l]) ++l;
        index_type b, e, c;
        contains(s.p,l,&b,&e,&c);
        if (l < m && (i+l == n || x[i+l] < y[l])) ++count;
        if (b != 0) { // per(y[0..l)) = b/2
            // c = |Y_[1..b/2) ∩ [ɛ,y)| = |x_[i+1..i+b/2) ∩ [ɛ,y)|
            count += c;
            i += b/2;
            l -= b/2;
        } else { // per(y[0..l)) > l/k
            pred(s.n,l/k+1,&b,&c); // (⌊l/k⌋+1)/4 < b
            // c = |Y_[1..b) ∩ [ɛ,y)| = |x_[i+1..i+b) ∩ [ɛ,y)|
            count += c;
            i += b;
            l = 0;
        }
    }
    return count;
}

template <typename string_type, typename index_type>
index_type range_count(
        const string_type& t, index_type n,
        const string_type& b, index_type m1,
        const string_type& e, index_type m2,
        index_type k)
{
    return
        one_sided_range_count(t,n,e,m2,k)-
        one_sided_range_count(t,n,b,m1,k);
}

template <typename string_type>
typename string_type::size_type range_count(
        const string_type& t,
        const string_type& b,
        const string_type& e,
        typename string_type::size_type k)
{
    return range_count(t,t.size(),b,b.size(),e,e.size(),k);
}

} // str

#endif
