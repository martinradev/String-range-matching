#ifndef TEST_GENERATOR_HPP
#define TEST_GENERATOR_HPP

#include "TestCase.hpp"

#include <string>

class TestGenerator {
    public:
    TestGenerator();
    TestCase<char> generateRandomTestCase(size_t textLen, size_t lowBoundLen, size_t topBoundLen, size_t numOfRepetitions = 0);
    std::string generateRandomString(size_t len);
    private:
    char generateRandomLetter();
};

#endif // TEST_GENERATOR_HPP
