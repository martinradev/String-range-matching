#ifndef TEST_GENERATOR_HPP
#define TEST_GENERATOR_HPP

#include "TestCase.hpp"

#include <string>
#include <istream>

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
        generates a test case with only the provided to be read. The whole stream will be exhausted
        to read the text. A random lower and upper bound of length \a prefixLen will be
        generated. If opetion \a useSubstring is used, then the bounds will be distinct substrings of
        the text.
    */
    TestCase<char> generateTestCase(std::istream & stream, size_t prefixLen, bool useSubstring = false);
    /*!
        generates a random string of length \a len
    */
    std::string generateRandomString(size_t len);
    private:
    typedef std::pair<std::string, std::string> bounds;
    /*!
        generates a randomm letter
    */
    char generateRandomLetter();
    /*!
        generate lower and upper bound
    */
    bounds generateBounds(size_t lowBoundLen, size_t upBoundLen);
    /*!
        checks whether the bounds are correct
    */
    bool areBoundsCorrect(const bounds & bb);
};

#endif // TEST_GENERATOR_HPP
