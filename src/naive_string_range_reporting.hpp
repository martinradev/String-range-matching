#ifndef NAIVE_STRING_RANGE_REPORTING_HPP
#define NAIVE_STRING_RANGE_REPORTING_HPP

#include <algorithm>
#include <string>
#include <iterator>

namespace str {

template <typename input_iterator, typename output_iterator>
void naive_range_match(
        input_iterator ti, input_iterator te,
        input_iterator bi, input_iterator be,
        input_iterator ei, input_iterator ee,
        output_iterator r)
{
    using namespace std;
    typename iterator_traits<output_iterator>::value_type i = 0;
    for (; ti != te; ++ti, ++i, ++r) {
        if (
                 lexicographical_compare(ti,te,ei,ee) &&
                !lexicographical_compare(ti,te,bi,be)) *r = i;
    }
}

template <typename char_type, typename output_iterator>
void naive_range_match(
        const std::basic_string<char_type>& t,
        const std::basic_string<char_type>& b,
        const std::basic_string<char_type>& e,
        output_iterator r)
{
    naive_range_match(
            t.begin(), t.end(),
            b.begin(), b.end(),
            e.begin(), e.end(), r);
}

} // str

#endif
