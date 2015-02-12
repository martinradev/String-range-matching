#include "ProjectInc.hpp"
#include "TestSuite.h"
#include "SuffixArray.hpp"
#include "check_macros.h"
#include "TestCase.hpp"
#include "TestGenerator.hpp"

#include <string>
#include <vector>

using namespace rmatch;
using namespace std;
using namespace boost;

static TestGenerator generator;

/*!
    test with a normal short case
    all suffixes
*/
TEST(Z_ALGORITHM, NORMAL) {
    string text = "banana";
    string low = "0";
    string top = "z";
    TestCase<char> test = TestCase<char>(text, low, top);
    vector<size_t> out = stringRangeMatchZ(text, low, top);
    test.check(out);
}

/*!
    test with a normal short case
    fewer suffixes
*/
TEST(Z_ALGORITHM, NORMAL_FEWER) {
    string text = "banana";
    string low = "0";
    string top = "z";
    TestCase<char> test = TestCase<char>(text, low, top);
    vector<size_t> out = stringRangeMatchZ(text, low, top);
    test.check(out);
}

/*!
    test with a random short case
*/
TEST(Z_ALGORITHM, RANDOM_TEST_SMALL) {
    TestCase<char> test = generator.generateRandomTestCase(100, 10, 15);
    vector<size_t> out = stringRangeMatchZ(test.getData(), test.getLowerBound(), test.getUpperBound());
    test.check(out);
}

/*!
    test with a random medium case
    very short prefix
*/
TEST(Z_ALGORITHM, RANDOM_TEST_MEDIUM) {
    TestCase<char> test = generator.generateRandomTestCase(10000, 1, 2);
    vector<size_t> out = stringRangeMatchZ(test.getData(), test.getLowerBound(), test.getUpperBound());
    test.check(out);
}

/*!
    test with a random medium case
    medium length prefix
*/
TEST(Z_ALGORITHM, RANDOM_TEST_MEDIUM_LONG_PREFIX) {
    TestCase<char> test = generator.generateRandomTestCase(10000, 443, 377);
    vector<size_t> out = stringRangeMatchZ(test.getData(), test.getLowerBound(), test.getUpperBound());
    test.check(out);
}

/*!
    test with a random big case 10^6
    medium length prefix
*/
TEST(Z_ALGORITHM, RANDOM_TEST_BIG) {
    TestCase<char> test = generator.generateRandomTestCase(1000000, 443, 377);
    vector<size_t> out = stringRangeMatchZ(test.getData(), test.getLowerBound(), test.getUpperBound());
    test.check(out);
}

/*!
    test with a random big case 10^6
    long length prefix
*/
TEST(Z_ALGORITHM, RANDOM_TEST_BIG_LONG_PREFIX) {
    TestCase<char> test = generator.generateRandomTestCase(1000000, 1773, 4565);
    vector<size_t> out = stringRangeMatchZ(test.getData(), test.getLowerBound(), test.getUpperBound());
    test.check(out);
}
