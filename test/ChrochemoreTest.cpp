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
    simple test for chrochemore range match
    all suffixes
*/
TEST(CHROCHEMORE, NORMAL) {
    string text = "banana";
    string low = "0";
    string top = "z";
    TestCase<char> test = TestCase<char>(text, low, top);
    vector<size_t> out = stringRangeMatch(text, low, top);
    test.check(out);
}

/*!
    simple test for chrcohemore range match
    fewer suffixes
*/
TEST(CHROCHEMORE, NORMAL_FEWER) {
    string text = "banana";
    string low = "0";
    string top = "z";
    TestCase<char> test = TestCase<char>(text, low, top);
    vector<size_t> out = stringRangeMatch(text, low, top);
    test.check(out);
}

/*!
    small random test to check again that it works correctly
*/
TEST(CHROCHEMORE, RANDOM_TEST_SMALL) {
    TestCase<char> test = generator.generateRandomTestCase(100, 10, 15);
    vector<size_t> out = stringRangeMatch(test.getData(), test.getLowerBound(), test.getUpperBound());
    test.check(out);
}

/*!
    check on larger string with very short prefixes
*/
TEST(CHROCHEMORE, RANDOM_TEST_MEDIUM) {
    TestCase<char> test = generator.generateRandomTestCase(10000, 1, 2);
    vector<size_t> out = stringRangeMatch(test.getData(), test.getLowerBound(), test.getUpperBound());
    test.check(out);
}

/*!
    check on larger string with relatively long prefixes
*/
TEST(CHROCHEMORE, RANDOM_TEST_MEDIUM_LONG_PREFIX) {
    TestCase<char> test = generator.generateRandomTestCase(10000, 443, 377);
    vector<size_t> out = stringRangeMatch(test.getData(), test.getLowerBound(), test.getUpperBound());
    test.check(out);
}

/*!
    check with a big random case 10^6 of length to check that it works fast enough
    short prefixes
*/
TEST(CHROCHEMORE, RANDOM_TEST_BIG) {
    TestCase<char> test = generator.generateRandomTestCase(1000000, 443, 377);
    vector<size_t> out = stringRangeMatch(test.getData(), test.getLowerBound(), test.getUpperBound());
    test.check(out);
}

/*!
    check with a big random case 10^6 of length to check that it works fast enough
    long prefixes
*/
TEST(CHROCHEMORE, RANDOM_TEST_BIG_LONG_PREFIX) {
    TestCase<char> test = generator.generateRandomTestCase(1000000, 1773, 4565);
    vector<size_t> out = stringRangeMatch(test.getData(), test.getLowerBound(), test.getUpperBound());
    test.check(out);
}

