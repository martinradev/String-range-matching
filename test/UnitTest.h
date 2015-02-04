#ifndef UNIT_TEST_H
#define UNIT_TEST_H

/*!
    unit test case
*/
class UnitTest {
    public:
    const char * m_class_name;
    const char * m_method_name;
    char result;
    UnitTest(const char * class_name, const char * method_name) :
        m_class_name(class_name), m_method_name(method_name){};
    virtual void run()=0;
};

#endif // UNIT_TEST_H
