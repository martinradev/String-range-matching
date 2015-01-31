#include <iostream>
#include <string>
#include "TestSuite.h"
#include "SuffixArray.hpp"
#include "check_macros.h"
#include "sais.hxx"
#include "TestGenerator.hpp"
using namespace std;

int main()
{
    //TestSuite::run_tests();
    size_t len = 1000;
    size_t step = 1000;
    TestGenerator gen;
    for (int i = 0; i < 5; ++i) {
        TestCase<char> test = gen.generateRandomTestCase(len, len / 4, len / 4);
        string testCaseName = "testcases/random_test_" + std::to_string(i);
        test.save(testCaseName);
        len += step;
    }
    return 0;
}
