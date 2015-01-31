#ifndef UTIL_HPP
#define UTIL_HPP

#include <vector>
#include <cstddef>
#include <boost/dynamic_bitset.hpp>

namespace str {
    /*!
        given two bit vectors \a lowBits and \a topBits it finds the
        bits for which we have ones only in either one of them
        It returns the positions of those bits.
    */
    std::vector<size_t> retrieveRangeIndices(const boost::dynamic_bitset<> & lowbits, const boost::dynamic_bitset<> & topbits);
}

#endif // UTIL_HPP
