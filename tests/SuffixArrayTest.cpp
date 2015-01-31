#ifndef SUFFIX_ARRAY_TEST_HPP
#define SUFFIX_ARRAY_TEST_HPP

/*!
    Added to ensure that the suffix array works properly
*/

#include "TestSuite.h"
#include "SuffixArray.hpp"
#include "check_macros.h"
#include "TestCase.hpp"

#include <string>
#include <algorithm>
#include <vector>
using namespace std;

TEST(SUFFIX_ARRAY,SIMPLE_TEST) {
    basic_string<char> data = "banana";
    basic_string<char> from = "0";
    basic_string<char> to = "z";
    vector<size_t> correct = {0,1,2,3,4,5};
    TestCase<char> test = TestCase<char>(data, from, to, correct);
    SuffixArray<char> arr = SuffixArray<char>(data);
    vector<size_t> out = arr.rangeQuery(from, to);
    sort(out.begin(), out.end());
    CHECK_EQUAL(true, test.check(out));
};

#endif // SUFFIX_ARRAY_TEST_HPP
