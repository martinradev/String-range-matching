#ifndef SUFFIX_ARRAY_HPP
#define SUFFIX_ARRAY_HPP

#include "sais.hxx"

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>

template<typename T>
class SuffixArray {
    public:
    SuffixArray(std::basic_string<T> * data)
     : m_data(data), m_array(data.length()), m_array_inv(data.length()), m_lcp(data.length()) {
        int err = saisxx(m_data.begin(), m_array.begin(), (int)m_data.length());
        if (err) {
            throw std::runtime_error("Could not create suffix array. Error: " + err);
        }
        buildInv();
        buildLcp();
    }
    private:
    /*!
        pointer to string
    */
    std::basic_string<T> * m_data;

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
            while (*m_data[i+l]==*m_data[j+l]) ++l;
            m_lcp[k] = l;
            if (l>0) --l;
        }
    }
};

#endif // SUFFIX_ARRAY_HPP
