#include "mallocate.hpp"

void *mallocate(size_t bytes)
{
    return std::malloc(bytes);
}
