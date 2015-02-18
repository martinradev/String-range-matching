#ifndef UTIL_HPP
#define UTIL_HPP

#include <vector>
#include <iterator>
#include <cstddef>
#include <boost/dynamic_bitset.hpp>
#include <iostream>

namespace rmatch {
    /*!
        given two bit vectors \a lowBits and \a topBits it finds the
        bits for which we have ones only in either one of them
        It returns the positions of those bits.
    */
    template <typename output_iterator>
    void retrieveRangeIndices(
            const boost::dynamic_bitset<> & lowbits,
            const boost::dynamic_bitset<> & topbits,
            output_iterator positions)
    {
        /*
            we have the bitsets for the lower and the upper bound
            obviously the lower bound is a subset of the upper bound
            namely if we have 1 for some index in the lower bound, the we have it in the upper bound
            we have to set those bits to 0
            we just have to do an XOR
        */
        const boost::dynamic_bitset<> XOR = lowbits ^ topbits;

        size_t i = XOR.find_first();

        /*
            find the next bit's position and store the index
            note that if one decides to stop using the bitset and use multiple memory blocks to work with
            the best way to find the next bit set is to use the expression
            (val & -val) which is equivalen to (val & (~val + 1)) due to 2-complement of how integers are stored in memory
        */
        while (i != XOR.npos)
        {
            *positions++ = i;
            i = XOR.find_next(i);
        }
    }

    inline std::vector<size_t> retrieveRangeIndices(
            const boost::dynamic_bitset<> & lowbits,
            const boost::dynamic_bitset<> & topbits)
    {
        std::vector<size_t> positions;
        retrieveRangeIndices(lowbits,topbits,std::back_inserter(positions));
        return std::move(positions);
    }
}

#endif // UTIL_HPP
