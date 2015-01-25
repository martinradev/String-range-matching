#include <iostream>
#include <string>
#include "../src/ProjectInc.hpp"
#include "TestSuite.h"
#include "check_macros.h"
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

int main()
{
    TestSuite::run_tests();
    std::string T = "abcdefghijklmnopqrstuvz";
    std::string l = "ghiz";
    std::string t = "g";
    cout << lowerBound(T, l) << endl;
    cout << lowerBoundZ(T, l) << endl;
    return 0;
}
