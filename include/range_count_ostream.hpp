#ifndef RANGE_COUNT_OSTREAM_HPP
#define RANGE_COUNT_OSTREAM_HPP

#include "range_count_detail.hpp"
#include <ostream>

namespace str {
namespace detail {

template <typename index_type>
std::ostream& operator<<(std::ostream& o, const bec_t<index_type>& bec)
{
    return o <<
        "(b=" << bec.b <<
        ",e=" << bec.e <<
        ",c=" << bec.c << ")";
}

template <typename index_type>
std::ostream& operator<<(std::ostream& o, const bc_t<index_type>& bc)
{
    using namespace std;
    return o <<
        "(b=" << bc.b <<
        ",c=" << bc.c << ")";
}

template <typename value_type>
std::ostream& operator<<(std::ostream& o, const std::vector<value_type>& c)
{
    auto it = c.cbegin();
    if (it == c.cend())
        return o << "[]";
    o << "[" << *it;
    for (++it; it != c.cend(); ++it) {
        o << "," << *it;
    }
    return o << "]";
}

template <typename index_type>
std::ostream& operator<<(std::ostream& o, const s_t<index_type>& s)
{
    return o << "(s_p=" << s.p << ",s_n=" << s.n << ")";
}

} // detail
} // str

#endif
