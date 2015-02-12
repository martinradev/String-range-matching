#ifndef CROCHEMORE_HPP
#define CROCHEMORE_HPP

#include "Util.hpp"

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <boost/dynamic_bitset.hpp>

namespace rmatch
{

/*!
    computes the next maximum suffix (MS) given the previous one. Also computes the period of it.
    \a l -> end index of pattern
    \a s -> starting position of the maximum suffix
    \a p -> per(MS(pattern[0...l]))
    Function is needed later in determining the jump to make by checking whether
    per(pattern[0..l]) == per(MS(pattern[0..l]))
*/
template<typename string_type>
void updateMS(const string_type & pattern, size_t & l, size_t & s, size_t & p)
{
    if (l == 0)
    {
        /*
            maximum suffix is pattern[s..l)
            setting p to 1 mean that no characters really match
        */
        l = 1;
        s = 0;
        p = 1;
        return;
    }
    size_t i = l;
    while (i < l+1)
    {
        if (pattern[i-p] > pattern[i])
        {
            /*
                set the maximum suffix to be recomputed
            */
            i = i - (i-s) % p;
            s = i;
            p = 1;
        }
        else if (pattern[i-p] < pattern[i])
        {
            /*
                increase maximum suffix
                set the period be the length of the maximum suffix
                this yields that no characters really match
                (no border)
            */
            p = i-s+1;
        }
        /*
            the characters are equal
            the maximum suffix is extended
            the perioud is the same
            namely:
            we have that
            pattern[0...i] = A.A.A.A.A.B where B is a prefix of A
            so, it is so that the period holds, but we just continue to have equal characters
        */
        i++;
    }
    l+=1;
}

/*!
    returns a \a boost::dynamic_bitset of size \a text.length() where
    bit[i] = 1 then text[i...) < pattern
    bit[i] = 0 else
*/
template<typename string_type>
boost::dynamic_bitset<> lowerBound(const string_type & text, const string_type & pattern)
{
    boost::dynamic_bitset<> bits = boost::dynamic_bitset<>(text.length());
    size_t i=0, l = 0, p = 0, s = 0, j = 0;
    size_t imax = 0, lmax = 0, pmax = 0, smax = 0, h = 0;
    while (i < text.length())
    {
        while (i+l < text.length() && l < pattern.length() && text[i+l]==pattern[l])
        {
            /*
                update the maximum suffix and the maximum pariod of it
            */
            updateMS(pattern, l, s, p);
        }
        if (l < pattern.length() && (i+l==text.length() || text[i+l] < pattern[l]))
        {
            /*
                we do not have a match
                we check whether the last compared character in the text is smaller than the last compared in pattern
                if we have reached the length of the text, then we "compare" with the empty string which is always smaller
                => we set the bit to 1
            */
            bits[i] = 1;
        }
        j = imax;
        if (l > lmax)
        {
            std::swap(l,lmax);
            std::swap(s,smax);
            std::swap(p,pmax);
            imax = i;
        }
        if ((0 < p && p <= l/3) && std::equal(pattern.begin(), pattern.begin()+s, pattern.begin()+p))
        {
            /*
                we make a skip but we must copy the bit set in the range
                we are skipping a period
                we just have to copy what we have set initially.
            */
            for (int f = 1; f < p; ++f) bits[i+f] = bits[j+f];
            i = i+p;
            l = l-p;
        }
        else
        {
            /*
                we make a skip but we must copy the bit set in the range
            */
            h = l/3 + 1;
            for (int f = 1; f < h; ++f) bits[i+f] = bits[j+f];
            i += h;
            l = 0, s = 0, p = 0;
        }
    }
    return std::move(bits);
}

/*!
    puts the starting positions of all suffixes in \a text which are lexicographically
    in the range [low,top) in \a positions
*/
template<typename string_type, typename output_container>
void stringRangeMatch(const string_type & text, const string_type & low, const string_type & top, output_container& positions)
{
    using namespace std;
    boost::dynamic_bitset<> lowbits = lowerBound(text,low);
    boost::dynamic_bitset<> topbits = lowerBound(text,top);
    str::retrieveRangeIndices(lowbits,topbits,positions);
}

/*!
    returns the starting positions of all suffixes in \a text which are lexicographically
    in the range [low,top).
*/
template<typename string_type>
std::vector<size_t> stringRangeMatch(const string_type & text, const string_type & low, const string_type & top)
{
    std::vector<size_t> positions;
    stringRangeMatch(text,low,top,positions);
    return std::move(positions);
}

}

#endif // CROCHEMORE_HPP
