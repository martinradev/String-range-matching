/*!
    Added to ensure that the suffix array works properly
*/

#include "TestSuite.h"
#include "SuffixArray.hpp"
#include "check_macros.h"
#include "TestCase.hpp"
#include "TestGenerator.hpp"

#include <string>
#include <algorithm>
#include <vector>
using namespace std;

/*!
    does a simple test to check whether the indices match
*/
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

/*!
    does another simple test, but now it uses the slow check just to ensure
*/
TEST(SUFFIX_ARRAY, SIMPLE_TEST_2) {
    basic_string<char> data = "banana";
    basic_string<char> from = "0";
    basic_string<char> to = "z";
    vector<size_t> correct = {0,1,2,3,4,5};
    TestCase<char> test = TestCase<char>(data, from, to, correct);
    SuffixArray<char> arr = SuffixArray<char>(data);
    vector<size_t> out = arr.rangeQuery(from, to);
    sort(out.begin(), out.end());
    CHECK_EQUAL(true, test.naiveCheck(out));
}

TEST(SUFFIX_ARRAY, TEST_GENERATOR_TEST) {
    TestGenerator generator;
    TestCase<char> test = generator.generateRandomTestCase(10, 3, 7);
    SuffixArray<char> arr = SuffixArray<char>(test.getData());
    vector<size_t> out = arr.rangeQuery(test.getLowerBound(), test.getUpperBound());
    sort(out.begin(), out.end());
    CHECK_EQUAL(true, test.naiveCheck(out));
}

TEST(SUFFIX_ARRAY, TEST_GENERATOR_TEST_LOAD_FROM_FILE) {
    TestGenerator generator;
    TestCase<char> test = TestCase<char>("testcases/simple_test.txt");
    SuffixArray<char> arr = SuffixArray<char>(test.getData());
    vector<size_t> out = arr.rangeQuery(test.getLowerBound(), test.getUpperBound());
    sort(out.begin(), out.end());
    CHECK_EQUAL(true, test.check(out));
}
