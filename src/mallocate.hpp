#ifndef MALLOCATE_HPP
#define MALLOCATE_HPP

#include <cstdlib>
#include <limits>
#include <memory>

extern "C" void *mallocate(size_t bytes);

template <typename T>
struct mallocator {
    using value_type = T;

    // circumventing gcc basic_string implementation
    using pointer = value_type*;
    using ptraits = std::pointer_traits<pointer>;
    using const_pointer = value_type const*;
    using difference_type = typename ptraits::difference_type;
    using size_type = typename std::make_unsigned<difference_type>::type;
    using reference = value_type&;
    using const_reference = value_type const&;

    mallocator() = default;
    template <class U>
    mallocator(const mallocator<U>&) {}

    T* allocate(std::size_t n) {
        if (n <= std::numeric_limits<std::size_t>::max() / sizeof(T)) {
            if (auto ptr = mallocate(n*sizeof(T))) {
                return static_cast<T*>(ptr);
            }
        }
        throw std::bad_alloc();
    }
    void deallocate(T* ptr, std::size_t n) {
        std::free(ptr);
    }

    template<typename U> struct rebind { typedef mallocator<U> other; };
};

/*
template <typename T, typename U>
inline bool operator == (const mallocator<T>&, const mallocator<U>&) {
    return true;
}

template <typename T, typename U>
inline bool operator != (const mallocator<T>& a, const mallocator<U>& b) {
    return !(a == b);
}
*/

#endif
