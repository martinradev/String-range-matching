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
    bounds bb = generateBounds(lowBoundLen, topBoundLen);
    return TestCase<char>(randomText, bb.first, bb.second);
}

TestCase<char> TestGenerator::generateTestCase(istream & stream, size_t prefixLen, bool useSubstring) {
    basic_string<char> container;
    bounds bb;
    cout << "start reading" << endl;
    stream.seekg(0, std::ios::end);
    container.reserve(stream.tellg());
    stream.seekg(0, std::ios::beg);
    container.assign((std::istreambuf_iterator<char>(stream)),
            std::istreambuf_iterator<char>());
    cout << "end reading" << endl;
    if (useSubstring) {
        size_t to = container.length()-prefixLen;
        size_t idx1 = 0, idx2 = 0;
        bb = bounds("b","a");
        while (!areBoundsCorrect(bb)) {
            idx1 = rand()%to;
            idx2 = rand()%to;
            bb = bounds(container.substr(idx1, prefixLen), container.substr(idx2, prefixLen));
        }
    } else {
        bb = generateBounds(prefixLen, prefixLen);
    }
    return TestCase<char>(container, bb.first, bb.second);
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

bool TestGenerator::areBoundsCorrect(const bounds & bb) {
    return bb.first <= bb.second;
}

TestGenerator::bounds TestGenerator::generateBounds(size_t lowBoundLen, size_t upBoundLen) {
    string randomLowBound = "b";
    string randomTopBound = "a";
    while(randomLowBound > randomTopBound) {
        randomLowBound = generateRandomString(lowBoundLen);
        randomTopBound = generateRandomString(lowBoundLen);
    }
    return bounds(randomLowBound, randomTopBound);
}
