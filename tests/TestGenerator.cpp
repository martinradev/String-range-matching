#include "TestGenerator.hpp"

#include <ctime>
#include <cstdlib>
#include <iostream>
using namespace std;

TestGenerator::TestGenerator() {
    srand(time(NULL));
}

TestCase<char> TestGenerator::generateRandomTestCase(size_t textLen, size_t lowBoundLen, size_t topBoundLen) {
    string randomText = generateRandomString(textLen);
    string randomLowBound = "b";
    string randomTopBound = "a";
    while(randomLowBound > randomTopBound) {
        randomLowBound = generateRandomString(lowBoundLen);
        randomTopBound = generateRandomString(lowBoundLen);
    }
    return TestCase<char>(randomText, randomLowBound, randomTopBound);
}

string TestGenerator::generateRandomString(size_t len) {
    string str;
    str.reserve(len);
    while (len--) {
        str.push_back(generateRandomLetter());
    }
    return str;
}

char TestGenerator::generateRandomLetter() {
    static char ALPHABET[16] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'
    };
    static size_t ALPHABET_SIZE = 16;
    int num = rand()%ALPHABET_SIZE;
    return ALPHABET[num];
}
