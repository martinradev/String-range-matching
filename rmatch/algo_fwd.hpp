/*
 * Forward declarations of selected algorithm functions. This can be used to
 * reduce compilation times.
 */

#ifndef ALGO_FWD_HPP
#define ALGO_FWD_HPP

namespace rmatch {

template <typename string_type, typename output_iterator>
void kmp_match_less(
        const string_type& t,
        const string_type& p,
        output_iterator r);

template <typename string_type, typename output_iterator>
void kmp_match_range(
        const string_type& t,
        const string_type& l,
        const string_type& u,
        output_iterator r);

} // rmatch

#endif // ALGO_FWD_HPP
