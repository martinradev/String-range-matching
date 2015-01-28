#include <iostream>
#include <string>
#include "../src/ProjectInc.hpp"
#include "TestSuite.h"
#include "check_macros.h"
#include "SuffixArray.hpp"
#include "sais.hxx"
using namespace str;
using namespace std;
using namespace boost;

TEST(CROCHEMORE , NORMAL) {
    string text = "tova e kakvoto 6te da bude";
    string pattern = "da";
    vector<size_t> output = crochemoreSearch<char>(text, pattern);
    CHECK_EQUAL(1,output.size());
    CHECK_EQUAL(19, output[0]);
}

TEST(CROCHEMORE , NOT_FOUND) {
    string text = "tovo e kakvoto 6te da bude";
    string pattern = "toto";
    vector<size_t> output = crochemoreSearch(text, pattern);
    CHECK_EQUAL(0,output.size());
}

TEST(CROCHEMORE, VEC) {
    basic_string<int> text = {7,8,789,87,1,2,3,1,2,3,0,0,1,0,0,12,123,1456,1465};
    basic_string<int> pattern = {1,2,3,0,0,1,0,0,12,123};
    vector<size_t> output = crochemoreSearch(text, pattern);
    CHECK_EQUAL(1,output.size());
    CHECK_EQUAL(7, output[0]);
}

void check_contains(const str::detail::s_p_t<int>& s_p, int x, int i)
{
    using namespace str::detail;
    int b, e, c;
    contains(s_p,x,&b,&e,&c);
    if (i < 0) {
        CHECK_EQUAL(b,0);
    } else {
        const bec_t<int>& bec = s_p[i];
        CHECK_EQUAL(b,bec.b);
        CHECK_EQUAL(e,bec.e);
        CHECK_EQUAL(c,bec.c);
    }
}

TEST(ONE_SIDED_STRING_RANGE_COUNTING_CONTAINS, NORMAL) {
    using namespace str::detail;
    s_p_t<int> s{{0,1,0},{3,6,0},{6,7,0},{8,10,1}};
    check_contains(s,10,-1);
    check_contains(s,8,3);
    check_contains(s,6,2);
    check_contains(s,5,1);
    check_contains(s,1,-1);
    check_contains(s,0,0);
}

int main()
{
    TestSuite::run_tests();

    std::string str = "banana";
    string from = "0";
    string to = "z";
    SuffixArray<char> arr = SuffixArray<char>(&str);
    vector<size_t> pos = arr.rangeQuery(from, to);
    for (int i = 0; i < pos.size(); ++i) {
        cout << pos[i] << " ";
    }
    cout << endl;
    pos = str::stringRangeMatchZ(str, from, to);
    for (int i = 0; i < pos.size(); ++i) {
        cout << pos[i] << " ";
    }
    cout << endl;
    pos = str::stringRangeMatch(str, from, to);
    for (int i = 0; i < pos.size(); ++i) {
        cout << pos[i] << " ";
    }
    cout << endl;
    return 0;
}
