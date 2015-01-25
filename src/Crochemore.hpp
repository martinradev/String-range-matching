#ifndef CROCHEMORE_HPP
#define CROCHEMORE_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <bitset>
#include <boost/dynamic_bitset.hpp>

namespace str
{

/*!
    computes the next maximum suffix (MS) given the previous one. Also computes the period of it.
    \a l -> end index of pattern
    \a s -> starting position of the maximum suffix
    \a p -> per(MS(pattern[0...l]))
    Function is needed later in determining the jump to make by checking whether
    per(pattern[0..l]) == per(MS(pattern[0..l]))
*/
template<typename T>
void updateMS(const std::basic_string<T> & pattern, size_t & l, size_t & s, size_t & p)
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
    searches for \a pattern in \a text
    returned is a \a std::vector of starting possitions.
*/
template<typename T>
std::vector<size_t> crochemoreSearch(const std::basic_string<T> & text, const std::basic_string<T> & pattern)
{
    std::vector<size_t> positions;
    size_t i=0,l=0, p=0, s= 0;
    while (i < text.length())
    {
        while (i+l < text.length() && l < pattern.length() && text[i+l]==pattern[l])
        {
            /*
                update the maximum suffix and the maximum pariod of it
            */
            updateMS(pattern, l, s, p);
        }
        if (l == pattern.length())
        {
            /*
                pattern[0..l) == text[i..i+l)
            */
            positions.push_back(i);
        }
        if (p <= l/3 && s != 0 && equal(pattern.begin(), pattern.begin()+s, pattern.begin()+p))
        {
            /*
                we have that pattern[0..s) == pattern[p..p+s)
                => per(pattern[0...l)) == per(MS(pattern[0...l))
                if p <= l/3 then we have that pattern[0...l) is 3 periodic
                essentially we skip l/3 characters since we have
                i+=p and l-=p
            */
            i += p;
            l -= p;
        }
        else
        {
            /*
                otherwise safely just skip l/3 characters and restart matching
            */
            i = i + l/3 + 1;
            l=0,s=0,p=0;
        }
    }
    /*
        use strict move semantics to prevent copying of data no matter the compiler
    */
    return move(positions);
}

/*!
    returns a \a boost::dynamic_bitset of size \a text.length() where
    bit[i] = 1 then text[i...) < pattern
    bit[i] = 0 else
*/
template<typename T>
boost::dynamic_bitset<> lowerBound(const std::basic_string<T> & text, const std::basic_string<T> & pattern)
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
        if ((0 < p && p <= l/3) && s != 0 && std::equal(pattern.begin(), pattern.begin()+s, pattern.begin()+p))
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
    returns the starting positions of all suffixes in \a text which are lexicographically
    in the range (low,top).
*/
template<typename T>
std::vector<size_t> stringRangeMatch(const std::basic_string<T> & text, const std::basic_string<T> & low, const std::basic_string<T> & top)
{
    boost::dynamic_bitset<> lowbits = lowerBound<T>(text,low);
    boost::dynamic_bitset<> topbits = lowerBound<T>(text,top);
    /*
        we have the bitsets for the lower and the upper bound
        obviously the lower bound is a subset of the upper bound
        namely if we have 1 for some index in the lower bound, the we have it in the upper bound
        we have to set those bits to 0
        we just have to do an XOR
    */
    lowbits ^= topbits;

    /*
        we have the bits now 1 where the suffix is in the given bound
        we have to search for them.
        lowbits.count() gives the count of the bits where we have 1
    */
    std::vector<size_t> positions(lowbits.count());
    size_t i = lowbits.find_first();
    size_t idx = 0;

    /*
        find the next bit's position and store the index
        note that if one decides to stop using the bitset and use multiple memory blocks to work with
        the best way to find the next bit set is to use the expression
        (val & -val) which is equivalen to (val & (~val + 1)) due to 2-complement of how integers are stored in memory
    */
    while (i != lowbits.npos) {
        positions[idx++] = i;
        i = lowbits.find_next(i);
    }

    /*
        explicitly move data
    */
    return std::move(positions);
}

}

#endif // CROCHEMORE_HPP
