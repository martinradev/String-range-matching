#include <iostream>
#include <string>
#include "TestSuite.h"
#include "SuffixArray.hpp"
#include "check_macros.h"
#include "sais.hxx"
#include "TestGenerator.hpp"
#include "ProjectInc.hpp"
#include "main.hpp"
#include <ctime>

using namespace std;

string app_path;

void set_app_path(char *path)
{
    app_path = path;
    size_t i = app_path.rfind('/');
    if (i == string::npos) return;
    app_path.resize(i+1);
}

int main(int argc, char *argv[])
{
    set_app_path(argv[0]);
    TestSuite::run_tests();
    //TestGenerator gen;
    //ifstream in("testcases/Escherichia_Coli");
    //TestCase<char> test = gen.generateTestCase(in, 100, true);
    //test.save("testcases/escerichia_coli_processed_1.txt");
    /*std::cout << "START ALGO" << std::endl;
    clock_t begin = clock();
    vector<size_t> out = str::stringRangeMatch(test.getData(), test.getLowerBound(), test.getUpperBound());
    cout << "END ALGO" << endl;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    bool res = test.check(out);
    cout << "Res: " << res << endl;
    cout << elapsed_secs << endl;*/
    return 0;
}
