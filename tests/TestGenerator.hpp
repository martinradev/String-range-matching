#ifndef TEST_GENERATOR_HPP
#define TEST_GENERATOR_HPP

#include "TestCase.hpp"

#include <string>

/*!
    Generates tests, random strings, etc
*/
class TestGenerator {
    public:
    /*!
        inits the test generator
    */
    TestGenerator();
    /*!
        generates a random test case with a text of length \a textLen, lower bound of length
        \a lowBoundLen and upper bould of length \a topBoundLen.
    */
    TestCase<char> generateRandomTestCase(size_t textLen, size_t lowBoundLen, size_t topBoundLen);
    /*!
        generates a random string of length \a len
    */
    std::string generateRandomString(size_t len);
    private:
    /*!
        generates a randomm letter
    */
    char generateRandomLetter();

};

#endif // TEST_GENERATOR_HPP
