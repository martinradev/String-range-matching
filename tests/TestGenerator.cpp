#include "TestGenerator.hpp"

#include <ctime>
#include <cstdlib>

using namespace std;

TestGenerator::TestGenerator() {
    srand(time(NULL));
};

string TestGenerator::generateRandomString(size_t len) {
    string str;
    str.reserve(len);
    while (len--) {
        str.push_back(generateRandomLetter());
    }
    return str;
}

char TestGenerator::generateRandomLetter() {
    // generate from 0 to 127
    int num = rand()%128;

    // 1 to 128 ASCII
    return num+'0';
}
