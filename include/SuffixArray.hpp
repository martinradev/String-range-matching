#ifndef SUFFIX_ARRAY_HPP
#define SUFFIX_ARRAY_HPP

#include "sais.hxx"

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <iostream>

namespace rmatch {
/*!
    Suffix array wrapper. It uses the SAIS algorithm, implementation of Yuta Mori in the file sais.hxx
    It generates an lcp array and the inverse suffix array. It also supports range search over the suffixes.
*/
template<typename string_type, typename index_type = size_t>
class SuffixArray {
    public:
    /*!
        Creates a suffix array given the \a data string.
        Additionally, the inverse suffix array is constructed and the lcp array.
    */
    SuffixArray(const string_type & data)
     : m_data(data), m_array(data.length()), m_array_inv(data.length()), m_lcp(data.length()) {
        int err = saisxx(m_data.begin(), m_array.begin(), static_cast<int>(m_data.length()));
        if (err) {
            throw std::runtime_error("Could not create suffix array. Error: " + err);
        }
        buildInv();
        buildLcp();
    }
    public:
    /*!
        data storred
    */
    string_type m_data;

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
            while (i+l < m_data.length() && j+l < m_data.length() && m_data[i+l]==m_data[j+l]) ++l;
            m_lcp[k] = l;
            if (l>0) --l;
        }
    }
    /*!
        returns the index in the suffix array for which
        array[0...k] is a subarray for which
        p = array[i], then data[p] < top
    */
    int lowerBound(const string_type & top) {
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
        p = array[i], then data[p] >= bottom
    */
    int upperBound(const string_type & bottom) {
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
            if (m_data[i] >= bottom[j]) {
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
        stores the starting positions of the suffixes which are
        bigger or equal than \a bottom and smaller than \a top.
    */
    template <typename output_container>
    void rangeQuery(const string_type & bottom, const string_type & top, output_container& positions) {
        int from = upperBound(bottom);
        int to = lowerBound(top);
        if (from > to) {
            /*
                top is bigger than bottom
            */
            return;
        }
        positions.resize(to-from+1);
        int i = 0;
        while (from+i <= to) {
            positions[i] = m_array[from+i];
            ++i;
        }
    }
    
    /*!
        returns the starting positions of the suffixes which are
        bigger or equal than \a bottom and smaller than \a top.
    */
    std::vector<size_t> rangeQuery(const string_type & bottom, const string_type & top) {
        std::vector<size_t> positions;
        rangeQuery(bottom,top,positions);
        return positions;
    }
};
    
    template <typename string_type, typename output_container>
    void rangeQuery(const string_type& t, const string_type& b, const string_type& e, output_container& o)
    {
        SuffixArray<string_type>(t).rangeQuery(b,e,o);
    }
}

#endif // SUFFIX_ARRAY_HPP
