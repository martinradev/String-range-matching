#ifndef Z_ALGORITHM_HPP
#define Z_ALGORITHM_HPP

#include "Util.hpp"

#include <vector>
#include <string>
#include <boost/dynamic_bitset.hpp>

namespace str
{

/*!
    Finds the suffixes of \a text which are smaller than pattern
    It returns a bit vector where if the i-th bit is turned on, then
    this means that text[i..) < pattern.
    The function uses the Z algorithm to find the suffixes.
*/
template<typename string_type>
boost::dynamic_bitset<> lowerBoundZ(const string_type & text, const string_type & pattern)
{
    boost::dynamic_bitset<> bits = boost::dynamic_bitset<>(text.length());
    string_type total = pattern + text;
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
    return std::move(bits);
}

template <typename string_type, typename output_container>
void stringRangeMatchZ(const string_type & text, const string_type & low, const string_type & top, output_container& positions)
{
    boost::dynamic_bitset<> lowbits = lowerBoundZ(text,low);
    boost::dynamic_bitset<> topbits = lowerBoundZ(text,top);
    retrieveRangeIndices(lowbits,topbits,positions);
}

/*!
    Finds all of the suffixes of \a text which are bigger than \a low and smaller than \a top.
    The positions are returned.
*/
template<typename string_type>
std::vector<size_t> stringRangeMatchZ(const string_type & text, const string_type & low, const string_type & top)
{
    std::vector<size_t> positions;
    stringRangeMatchZ(text,low,top,positions);
    return std::move(positions);
}
}

#endif // Z_ALGORITHM_HPP
