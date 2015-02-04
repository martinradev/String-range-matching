#include "TestSuite.h"
#include "TestFailure.h"

void TestSuite::add_test(UnitTest * test) {
    TestSuite::get_container().push_back(test);
}

void TestSuite::run_tests() {
    std::vector<UnitTest*> local = TestSuite::get_container();
    std::vector<std::pair<UnitTest*,std::string> > errors;
    std::cout << "Total tests: " << local.size() << std::endl;
    for (size_t i = 0; i < local.size(); ++i) {
        UnitTest * test = local[i];
        try {
            test->run();
            std::cout << ".";
        } catch (TestFailure & ex) {
            errors.push_back(std::make_pair(test, ex.what()));
            std::cout << "!";
        }
    }
    std::cout << std::endl << std::endl;

    std::string delimiter(75, '-');

    if (errors.size() == 0) {
        std::cout << "ALL TESTS PASS" << std::endl;
    } else {
        std::cerr << "TESTS NOT PASSING: " << errors.size() << std::endl;
        std::cout << std::endl << delimiter << std::endl << std::endl;
        for (size_t i = 0; i < errors.size(); ++i) {
            std::pair<UnitTest*, std::string> cur = errors[i];
            UnitTest * failingTest = cur.first;
            std::string message = cur.second;
            std::cerr << failingTest->m_class_name << " "
                << failingTest->m_method_name << " " << message << std::endl;

            std::cerr << std::endl << delimiter << std::endl << std::endl;
        }
    }
}

std::vector<UnitTest*> & TestSuite::get_container() {
    static std::vector<UnitTest*> vt;
    return vt;
}
