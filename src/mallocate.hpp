#ifndef MALLOCATE_HPP
#define MALLOCATE_HPP

#include <cstdlib>
#include <limits>
#include <new>

extern "C" void *mallocate(size_t bytes);

template <typename T>
struct mallocator {
    using value_type = T;

    mallocator() = default;
    template <class U>
    mallocator(const mallocator<U>&) {}

    T* allocate(std::size_t n) {
        if (n <= std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            if (auto ptr = mallocate(n * sizeof(T))) {
                return static_cast<T*>(ptr);
            }
        }
        throw std::bad_alloc();
    }
    void deallocate(T* ptr, std::size_t n) {
        std::free(ptr);
    }
};

template <typename T, typename U>
inline bool operator == (const mallocator<T>&, const mallocator<U>&) {
    return true;
}

template <typename T, typename U>
inline bool operator != (const mallocator<T>& a, const mallocator<U>& b) {
    return !(a == b);
}

#endif
