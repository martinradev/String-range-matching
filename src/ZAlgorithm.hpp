#ifndef Z_ALGORITHM_HPP
#define Z_ALGORITHM_HPP

#include "Util.hpp"

#include <vector>
#include <string>
#include <boost/dynamic_bitset.hpp>

namespace str
{

/*!

*/
template<typename T>
boost::dynamic_bitset<> lowerBoundZ(const std::basic_string<T> & text, const std::basic_string<T> & pattern)
{
    boost::dynamic_bitset<> bits = boost::dynamic_bitset<>(text.length());
    std::basic_string<T> total = pattern + text;
    std::vector<size_t> prefixes(pattern.length() + text.length());
    size_t l = 0, r = 0;
    prefixes[0] = total.length();
    size_t i = 1;
    while (i<total.length())
    {
        if (i > r)
        {
            /*
                we just have to extend since we have reached the end of the window
            */
            l = r = i;
            while (r < total.length() && total[r-l] == total[r]) ++r;
            prefixes[i] = r-l;
            --r;
        }
        else
        {
            size_t k = i-l;
            /*
                prefixes[k] is already precomputed
                q = prefixes[k]
                =>
                total[0...q) = total[k...k+q)
                note that if q < r-i+1, then for sure
                total[k...k+q) is a prefix of total[i...r)
                meaning that prefix[i] = q
            */
            if (prefixes[k] < r-i+1)
            {

                prefixes[i] = prefixes[k];
            }
            else
            {
                l = i;
                /*
                    otherwise the prefix mau have a bigger length, so we
                    must extend
                */
                while (r < total.length() && total[r-l] == total[r]) ++r;
                prefixes[i] = r-l;
                --r;
            }
        }
        if (i >= pattern.length())
        {
            /*
                we are checking the text now
            */
            size_t pLen = prefixes[i];
            size_t textI = i-pattern.length();
            if (pLen < pattern.length() && pattern[pLen] > text[textI+pLen])
            {
                /*
                    check the character where we have a difference
                */
                bits[textI] = 1;
            }
        }
        ++i;
    }
    return bits;
}

/*!

*/
template<typename T>
std::vector<size_t> stringRangeMatchZ(const std::basic_string<T> & text, const std::basic_string<T> & low, const std::basic_string<T> & top)
{
    boost::dynamic_bitset<> lowbits = lowerBoundZ<T>(text,low);
    boost::dynamic_bitset<> topbits = lowerBoundZ<T>(text,top);

    std::vector<size_t> positions = retrieveRangeIndices(lowbits, topbits);

    return std::move(positions);
}
};

#endif // Z_ALGORITHM_HPP
