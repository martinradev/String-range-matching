#ifndef NAIVE_STRING_RANGE_REPORTING_HPP
#define NAIVE_STRING_RANGE_REPORTING_HPP

#include <algorithm>

namespace str {

template <typename input_iterator, typename output_iterator>
void naive_range_match(
        input_iterator xi, input_iterator xe
        input_iterator yi, input_iterator ye,
        output_iterator r)
{
    using namespace std;
    typename iterator_traits<output_iterator>::value_type i = 0;
    for (size_t i = 0; xi != xe; ++i, ++r) {
        if (lexicographic_compare(xi,xe,yi,ye)) *r = i;
    }
}

} // str
