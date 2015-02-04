#ifndef NAIVE_RANGE_MATCH_HPP
#define NAIVE_RANGE_MATCH_HPP

#include <algorithm>
#include <string>
#include <iterator>
#include <type_traits>

namespace str {

template <typename input_iterator, typename output_iterator>
void naive_range_match(
        input_iterator ti, input_iterator te,
        input_iterator bi, input_iterator be,
        input_iterator ei, input_iterator ee,
        output_iterator r)
{
    using namespace std;
    typedef typename iterator_traits<input_iterator>::difference_type diff;
    typedef typename make_unsigned<diff>::type size_type;
    size_type i = 0;
    for (; ti != te; ++ti, ++i, ++r) {
        if (
                 lexicographical_compare(ti,te,ei,ee) &&
                !lexicographical_compare(ti,te,bi,be)) *r = i;
    }
}

template <typename string_type, typename output_iterator>
void naive_range_match(
        const string_type& t,
        const string_type& b,
        const string_type& e,
        output_iterator r)
{
    naive_range_match(
            t.begin(), t.end(),
            b.begin(), b.end(),
            e.begin(), e.end(), r);
}

} // str

#endif
