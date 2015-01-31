#ifndef TEST_CASE_HPP
#define TEST_CASE_HPP

#include "SuffixArray.hpp"

#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>


/*!
    TestCase entity which contains the text, the lower bound prefix and the upper bound prefix.
    Also it contains the correct output.
*/
template<typename T, typename index_type = size_t>
class TestCase {
    private:
    typedef std::basic_string<T> storage;
    storage m_data;
    storage m_lowerBound;
    storage m_upperBound;
    std::vector<index_type> m_output;
    /*!
        generates correct output for the stored \a m_output, \a m_lowerBound and \a m_upperBound
    */
    void generateOutput() {
        SuffixArray<T> arr = SuffixArray<T, index_type>(m_data);
        m_output = arr.rangeQuery(m_lowerBound, m_upperBound);
        // sort to guarantee that order is preserved
        std::sort(m_output.begin(), m_output.end());
    }
    /*!
        reads the data from the stream
        N
        N elements of type T
    */
    void readDataInto(storage & container, std::istream & is) {
        T tmp;
        size_t sz;
        is >> sz;
        container.resize(sz);
        while (sz--) {
            container.push_back(tmp);
        }
        if (!is.good()) {
            std::cerr << "Error reading" << std::endl;
            exit(1);
        }
    };
    public:
    /*!
        Read the test case from a file.
        If the file does not exist or some error occurs during opening it,
        an error will be printed and the program will exit with status 1.
        Format for test case:
        [length of text]
        [text]
        [length of lower bound prefix]
        [lower bound]
        [length of upper bound prefix]
        [upper bound]
        [N = number of suffixes in the bound]
        [N integers]
    */
    TestCase(const std::string & file) {
        std::ifstream in(file);
        if (!in.good()) {
            std::cerr << "Problem opening file: " << file << std::endl;
            exit(1);
        }
        readDataInto(m_data, in);
        readDataInto(m_lowerBound, in);
        readDataInto(m_upperBound, in);
        /*
            a bit unnecessary
        */
        size_t n;
        in >> n;
        m_output.reserve(n);
        index_type idx;
        while (n--) {
            in >> idx;
            m_output.push_back(idx);
        }
    }
    TestCase(const storage & data, const storage & lowerBound, const storage & upperBound)
        : m_data(data), m_lowerBound(lowerBound), m_upperBound(upperBound)
    {
        generateOutput();
    };
    /*!
        initialize the test case by providing the input and output
    */
    TestCase(const storage & data, const storage & lowerBound, const storage & upperBound, const std::vector<index_type> & output)
        : m_data(data), m_lowerBound(lowerBound), m_upperBound(upperBound), m_output(output)
    {
        // sort to guarantee that order is preserved
        std::sort(m_output.begin(), m_output.end());
    };
    /*!
        save the test case to a given filename
    */
    void save(const std::string & fileName) {
        std::ofstream out(fileName);
        out << m_data << std::endl;
        out << m_lowerBound << std::endl;
        out << m_upperBound << std::endl;
        out << m_output.size() << std::endl;
        for (index_type i : m_output) {
            out << i << " ";
        }
    }
    bool check(const std::vector<index_type> & toBeChecked) {
        if (m_output.size() != toBeChecked.size()) return false;
        for (size_t i = 0; i < m_output.size(); ++i) {
            if (m_output[i] != toBeChecked[i]) return false;
        }
        return true;
    }
};

#endif // TEST_CASE_HPP
