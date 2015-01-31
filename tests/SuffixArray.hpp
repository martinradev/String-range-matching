#ifndef SUFFIX_ARRAY_HPP
#define SUFFIX_ARRAY_HPP

#include "sais.hxx"

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <iostream>

template<typename T, typename index_type = size_t>
class SuffixArray {
    public:
    SuffixArray(const std::basic_string<T> & data)
     : m_data(data), m_array(data.length()), m_array_inv(data.length()), m_lcp(data.length()) {
        int err = saisxx(m_data.begin(), m_array.begin(), (int)m_data.length());
        if (err) {
            throw std::runtime_error("Could not create suffix array. Error: " + err);
        }
        buildInv();
        buildLcp();
        //buildBinaryLcp(0,data->length()-1);
    }
    public:
    /*!
        pointer to string
    */
    std::basic_string<T> m_data;

    /*!
        suffix array
    */
    std::vector<int> m_array;

    /*!
        inverse suffix array
    */
    std::vector<int> m_array_inv;

    /*!
        lcp array
    */
    std::vector<int> m_lcp;

    /*!
        builds the inversed suffix array
    */
    void buildInv() {
        for (int i = 0; i < m_array.size(); ++i) {
            m_array_inv[m_array[i]] = i;
        }
    }

    /*!
        constructs the lcp array
    */
    void buildLcp() {
        int l = 0, k, j;
        for (int i = 0; i < m_array.size(); ++i) {
            k=m_array_inv[i];
            j=m_array[k-1];
            while (m_data[i+l]==m_data[j+l]) ++l;
            m_lcp[k] = l;
            if (l>0) --l;
        }
    }
    /*!
        returns the index in the suffix array for which
        array[0...k] is a subarray for which
        p = array[i], then data[p] < top
    */
    int lowerBound(const std::basic_string<T> & top) {
        int lstr = 0, rstr = 0, off = 0;
        int l = 0, r = m_array.size()-1;
        int i,j;
        while (l<=r) {
            int mid = (l+r)>>1;
            int idx = m_array[mid];
            i = off+idx;
            j = off;

            while (i<m_data.length() && j < top.length() && m_data[i] == top[j]) ++i, ++j;

            if (m_data[i]<top[j]) {
                l = mid+1;
                lstr = j;
            } else {
                r = mid-1;
                rstr = j;
            }
            off = std::min(lstr, rstr);
        }
        return r;
    }

    /*!
        returns the index in the suffix array for which
        array[t...) is a subarray for which
        p = array[i], then data[p] > bottom
    */
    int upperBound(const std::basic_string<T> & bottom) {
        int lstr = 0, rstr = 0, off = 0;
        int l = 0, r = m_array.size()-1;
        int i,j;
        while (l<=r) {
            int mid = (l+r)>>1;
            int idx = m_array[mid];
            i = off+idx;
            j = off;

            while (i<m_data.length() && j < bottom.length() && m_data[i] == bottom[j]) ++i, ++j;

            // go right on pattern smaller or equal
            if (m_data[i] > bottom[j]) {
                rstr = j;
                r = mid-1;
            } else {
                l = mid+1;
                lstr = j;
            }
            off = std::min(lstr, rstr);
        }
        return l;
    }

    /*!

    */
    std::vector<size_t> rangeQuery(const std::basic_string<T> & bottom, const std::basic_string<T> & top) {
        int from = upperBound(bottom);
        int to = lowerBound(top);
        std::vector<size_t> positions(to-from+1);
        int i = 0;
        while (from+i <= to) {
            positions[i] = m_array[from+i];
            ++i;
        }
        return positions;
    }
};

#endif // SUFFIX_ARRAY_HPP
