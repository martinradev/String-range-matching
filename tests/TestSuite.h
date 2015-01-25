#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#include <iostream>
#include <vector>
#include "UnitTest.h"

class TestSuite {
    public:
    static std::vector<UnitTest*> & get_container();
    static void add_test(UnitTest * test);
    static void run_tests();
};

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
