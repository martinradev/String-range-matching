#ifndef UTIL_HPP
#define UTIL_HPP

#include <vector>
#include <cstddef>
#include <boost/dynamic_bitset.hpp>

namespace str {
    std::vector<size_t> retrieveRangeIndices(const boost::dynamic_bitset<> & lowbits, const boost::dynamic_bitset<> & topbits);
};

#endif // UTIL_HPP
