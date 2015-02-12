#include "kmp_match.hpp"
#include "check_macros.h"
#include "TestCase.hpp"
#include "TestGenerator.hpp"
#include <vector>
#include <iterator>

using namespace rmatch;

/* Algorithm tests. */

using namespace std;

/*!
    simple test for kmp range match
    all suffixes
*/
TEST(KMP, NORMAL) {
    string text = "banana";
    string low = "0";
    string top = "z";
    TestCase<char> test = TestCase<char>(text, low, top);
    vector<size_t> r;
    kmp_match_range(text,low,top,back_inserter(r));
    test.check(r);
}

/*!
    Generate tests for kmp algorithm.
*/
void kmp_test(size_t tn, size_t ln, size_t un)
{
    TestGenerator generator;
    TestCase<char> test = generator.generateRandomTestCase(tn, ln, un);
    vector<size_t> r;
    kmp_match_range(test.getData(),test.getLowerBound(),test.getUpperBound(),
            back_inserter(r));
    test.check(r);
}

/*!
    small random test to check again that it works correctly
*/
TEST(KMP, RANDOM_TEST_SMALL) {
    kmp_test(100, 10, 15);
}

/*!
    check on larger string with very short prefixes
*/
TEST(KMP, RANDOM_TEST_MEDIUM) {
    kmp_test(10000, 1, 2);
}

/*!
    check on larger string with relatively long prefixes
*/
TEST(KMP, RANDOM_TEST_MEDIUM_LONG_PREFIX) {
    kmp_test(10000, 443, 377);
}

/*!
    check with a big random case 10^6 of length to check that it works fast
    enough long prefixes
*/
TEST(KMP, RANDOM_TEST_BIG) {
    kmp_test(1000000, 443, 377);
}

/*!
    check with a big random case 10^6 of length to check that it works fast
    enough long prefixes
*/
TEST(KMP, RANDOM_TEST_BIG_LONG_PREFIX) {
    kmp_test(1000000, 1773, 4565);
}
