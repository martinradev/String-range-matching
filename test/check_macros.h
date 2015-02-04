#ifndef CHECK_MACROS_H
#define CHECK_MACROS_H

#include "TestSuite.h"
#include "UnitTest.h"
#include "TestFailure.h"
#include <iostream>
#include <string>
#include <cstring>

inline const char * to_str(int num) {
    std::string s;
    while (num != 0) {
        int r = num%10;
        num /= 10;
        s.insert(0,1,r+'0');
    }
    return s.c_str();
}


#define CHECK_EQUAL(A,B) if (!(A == B)) \
    {throw TestFailure(to_str(A),to_str(B),"==",__PRETTY_FUNCTION__,__FILE__,__LINE__);}


#define FAIL() throw TestFailure("TEST FAILED", __PRETTY_FUNCTION__,__FILE__,__LINE__)

#endif // CHECK_MACROS_H
