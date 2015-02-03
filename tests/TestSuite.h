#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#include <iostream>
#include <vector>
#include "UnitTest.h"

/*!
    A test suite which contains all of the unit tests to be run
*/
class TestSuite {
    public:
    /*!
        returns the static unit test container
    */
    static std::vector<UnitTest*> & get_container();
    /*!
        adds a unit test
    */
    static void add_test(UnitTest * test);
    /*!
        retuns the tests
    */
    static void run_tests();
};

/*!
    Creates a unit test
*/
#define TEST(testClassName, testMethodName) \
    class testClassName##_##testMethodName : public UnitTest \
    {\
    public:\
    testClassName##_##testMethodName  () : UnitTest(#testClassName,#testMethodName) {\
        TestSuite::add_test(this);\
    };\
    void run();\
    };\
    testClassName##_##testMethodName  testClassName##_##testMethodName##_inst;\
    void testClassName##_##testMethodName  ::run()


#endif
