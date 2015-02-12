#ifndef TEST_CASE_HPP
#define TEST_CASE_HPP

#include "SuffixArray.hpp"

#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>
#include <map>


/*!
    TestCase entity which contains the text, the lower bound prefix, the upper bound prefix and
    the correct output.
    It supports loading test cases from files or saving a given test case.
*/
template<typename T, typename index_type = size_t>
class TestCase {
    public:
    typedef std::basic_string<T> storage;
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
        in.close();
        checkClassCondition();
    }
    /*!
        Creates a test case given \a data text string, the lower bound \a lowerBound and
        the upper bound \a upperBound.
        The correct output is generated using the suffix array.
        The program will exit with status 1 if \a lowerBound > \a upperBound.
    */
    TestCase(const storage & data, const storage & lowerBound, const storage & upperBound)
        : m_data(data), m_lowerBound(lowerBound), m_upperBound(upperBound)
    {
        checkClassCondition();
        generateOutput();
    };
    /*!
        initialize the test case by providing the input and output
        The program will exit with status 1 if \a lowerBound > \a upperBound.
    */
    TestCase(const storage & data, const storage & lowerBound, const storage & upperBound, const std::vector<index_type> & output)
        : m_data(data), m_lowerBound(lowerBound), m_upperBound(upperBound), m_output(output)
    {
        checkClassCondition();
        // sort to guarantee that order is preserved
        std::sort(m_output.begin(), m_output.end());
    };
    /*!
        save the test case to a given filename in a possible-to-load format.
    */
    void save(const std::string & fileName) {
        std::ofstream out(fileName);
        out << m_data.length() << std::endl;
        out << m_data << std::endl;
        out << m_lowerBound.length() << std::endl;
        out << m_lowerBound << std::endl;
        out << m_upperBound.length() << std::endl;
        out << m_upperBound << std::endl;
        out << m_output.size() << std::endl;
        for (index_type i : m_output) {
            out << i << " ";
        }
        out.close();
    }
    /*!
        checks whether the \a toBeChecked output is correct against the test case.
    */
    bool check(const std::vector<index_type> & toBeChecked) {
        if (m_output.size() != toBeChecked.size()) return false;
        for (size_t i = 0; i < m_output.size(); ++i) {
            if (m_output[i] != toBeChecked[i]) return false;
        }
        return true;
    }
    /*!
        checks \a toBeChecked is the correct suffix match count.
    */
    bool checkCount(index_type n)
    {
        return m_output.size() == n;
    }
    /*!
        Does a naive check whether the output \a toBeChecked is correct.
        MUST BE USED FOR VERY SMALL TEST CASES
        IF USED FOR TEXTS OF LENGTH BIGGER THAN 100, THE PROGRAM WILL EXIT OTHERWISE.
    */
    bool naiveCheck(const std::vector<index_type> & toBeChecked) {
        if (m_data.length() > 100) {
            std::cerr << "Text length is too big" << std::endl;
            exit(1);
        }
        // suffix, index
        std::map<storage, size_t> suffixes;
        for (size_t i = 0; i < m_data.length(); ++i) {
            suffixes.insert(std::make_pair(m_data.substr(i), i));
        }
        typename std::map<storage,size_t>::iterator low, up;
        low = suffixes.lower_bound(m_lowerBound);
        up = suffixes.lower_bound(m_upperBound);
        std::vector<size_t> tmpOut;
        tmpOut.reserve(m_data.length());
        while (low != up) {
            tmpOut.push_back(low->second);
            ++low;
        }
        std::sort(tmpOut.begin(), tmpOut.end());
        if (tmpOut.size() != toBeChecked.size()) return false;
        for (size_t i = 0; i < tmpOut.size(); ++i) {
            if (tmpOut[i] != toBeChecked[i]) return false;
        }
        return true;
    }
    /*!
        returns the text
    */
    const storage & getData() {return m_data;};
    /*!
        returns the lower bound string
    */
    const storage & getLowerBound() {return m_lowerBound;};
    /*!
        return the upper bound string
    */
    const storage & getUpperBound() {return m_upperBound;};
    private:
    storage m_data;
    storage m_lowerBound;
    storage m_upperBound;
    std::vector<index_type> m_output;
    /*!
        generates correct output for the stored \a m_output, \a m_lowerBound and \a m_upperBound
        it uses the \a SuffixArray wrapper to do so. Output is sorted.
    */
    void generateOutput() {
	using namespace rmatch;        
	SuffixArray<storage> arr = SuffixArray<storage>(m_data);
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
        container.reserve(sz);
        while (sz--) {
            is >> tmp;
            container.push_back(tmp);
        }

        if (!is.good()) {
            std::cerr << "Error reading" << std::endl;
            exit(1);
        }
    };
    /*!
        checks the class invariant
        that \a m_lowerBound <= \a m_upperBound
    */
    void checkClassCondition() {
        assert(m_lowerBound <= m_upperBound);
    };
};

#endif // TEST_CASE_HPP
