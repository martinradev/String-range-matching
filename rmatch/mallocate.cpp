#include "mallocate.hpp"

using namespace std;

void *mallocate(size_t bytes)
{
    return ::operator new(bytes,std::nothrow);
}
