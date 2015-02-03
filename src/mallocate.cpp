#include "mallocate.hpp"
#include <iostream>

using namespace std;

void *mallocate(size_t bytes)
{
    cout << "mallocate(" << bytes << ")" << endl;
    return std::malloc(bytes);
}
