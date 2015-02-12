#include "gs_count.hpp"
#include "check_macros.h"
#include "TestCase.hpp"
#include "TestGenerator.hpp"

using namespace rmatch;

/* Auxiliary function tests. */

typedef typename detail::s_p_t<int>::type s_p_t;

void check_contains(const s_p_t& s_p, int x, int i)
{
    using namespace rmatch::detail;
    int b, e, c;
    contains(s_p,x,b,e,c);
    if (i < 0) {
        CHECK_EQUAL(b,0);
    } else {
        const bec_t<int>& bec = s_p[i];
        CHECK_EQUAL(b,bec.b);
        CHECK_EQUAL(e,bec.e);
        CHECK_EQUAL(c,bec.c);
    }
}

typedef typename rmatch::detail::s_n_t<int>::type s_n_t;

void check_pred(const s_n_t& s_n, int x, int i)
{
    using namespace rmatch::detail;
    int b, c;
    pred(s_n,x,b,c);
    const bc_t<int>& bc = s_n[i];
    CHECK_EQUAL(b,bc.b);
    CHECK_EQUAL(c,bc.c);
}

TEST(GS_CONTAINS, NORMAL) {
    s_p_t s{{0,1,0},{3,6,0},{6,7,0},{8,10,1}};
    check_contains(s,10,-1);
    check_contains(s,8,3);
    check_contains(s,6,2);
    check_contains(s,5,1);
    check_contains(s,1,-1);
    check_contains(s,0,0);
    s_p_t r;
    check_contains(r,5,-1);
    check_contains(r,0,-1);
}

TEST(GS_PRED, NORMAL) {
    s_n_t s{{0,0},{1,2},{3,3}};
    check_pred(s,0,0);
    check_pred(s,1,1);
    check_pred(s,2,1);
    check_pred(s,3,2);
    check_pred(s,4,2);
}

/* Algorithm tests. */

using namespace std;

/*!
    simple test for chrochemore range match
    all suffixes
*/
TEST(GS, NORMAL) {
    string text = "banana";
    string low = "0";
    string top = "z";
    TestCase<char> test = TestCase<char>(text, low, top);
    size_t n;
    n = gs_count_range(text, low, top, 3);
    test.checkCount(n);
    n = gs_count_range(text, low, top, 4);
    test.checkCount(n);
    n = gs_count_range(text, low, top, 10);
    test.checkCount(n);
}

/*!
    Generate tests for gs algorithm with different values for k.
*/
void gs_test(size_t tn, size_t ln, size_t un)
{
    TestGenerator generator;
    TestCase<char> test = generator.generateRandomTestCase(tn, ln, un);
    size_t n;
    n = gs_count_range(test.getData(), test.getLowerBound(),
            test.getUpperBound(), 3);
    test.checkCount(n);
    n = gs_count_range(test.getData(), test.getLowerBound(),
            test.getUpperBound(), 4);
    test.checkCount(n);
    n = gs_count_range(test.getData(), test.getLowerBound(),
            test.getUpperBound(), 10);
    test.checkCount(n);
}

/*!
    small random test to check again that it works correctly
*/
TEST(GS, RANDOM_TEST_SMALL) {
    gs_test(100, 10, 15);
}

/*!
    check on larger string with very short prefixes
*/
TEST(GS, RANDOM_TEST_MEDIUM) {
    gs_test(10000, 1, 2);
}

/*!
    check on larger string with relatively long prefixes
*/
TEST(GS, RANDOM_TEST_MEDIUM_LONG_PREFIX) {
    gs_test(10000, 443, 377);
}

/*!
    check with a big random case 10^6 of length to check that it works fast
    enough long prefixes
*/
TEST(GS, RANDOM_TEST_BIG) {
    gs_test(1000000, 443, 377);
}

/*!
    check with a big random case 10^6 of length to check that it works fast
    enough long prefixes
*/
TEST(GS, RANDOM_TEST_BIG_LONG_PREFIX) {
    gs_test(1000000, 1773, 4565);
}
