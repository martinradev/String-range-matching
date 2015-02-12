/*
 * Custom memory allocator that delegates allocations to function mallocate()
 * with simple c-linkage. Using this allocator with STL containers enables
 * monitoring memory consumption of these containers by watching the
 * mallocate() function with external tools (like valgrind).
 *
 * Copyright (c) 2015 Jarno Leppänen
 */

#ifndef MALLOCATE_HPP
#define MALLOCATE_HPP

#include <cstddef>
#include <type_traits>
#include <new>

/* The function doing all memory allocations. */
extern "C" void *mallocate(size_t bytes);

/* The function doing all deallocations. */
extern "C" void mdeallocate(void *p);

/**
 * An allocator class delegating allocations and deallocations to mallocate()
 * and mdeallocate(). Adheres to c++ allocator requirements.
 */
template <class T> class mallocator
{
public:
    typedef T                 value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef std::size_t       size_type;
    typedef std::ptrdiff_t    difference_type;

    typedef std::true_type propagate_on_container_move_assignment;

    template <class U>
    struct rebind { typedef mallocator<U> other; };

    mallocator() {}
    template <class U>
    mallocator(const mallocator<U>&) {}

    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const
    {
        return x;
    }

    pointer allocate(size_type n, const_pointer = 0)
    {
        if (n > max_size()) throw std::bad_alloc();
        void *p = mallocate(n*sizeof(T));
        if (!p) throw std::bad_alloc();
        return static_cast<pointer>(p);
    }

    void deallocate(pointer p, size_type)
    {
        mdeallocate(p);
    }

    size_type max_size() const
    {
        return static_cast<size_type>(-1) / sizeof(T);
    }

    void construct(pointer p, const value_type& x)
    {
        new(p) value_type(x);
    }
    void destroy(pointer p) { p->~value_type(); }
};

/* Specialization for void* types. */
template<> class mallocator<void>
{
    typedef void        value_type;
    typedef void*       pointer;
    typedef const void* const_pointer;

    template <class U>
    struct rebind { typedef mallocator<U> other; };
};

template <typename T, typename U>
inline bool operator==(const mallocator<T>&, const mallocator<U>&)
{
    return true;
}

#endif
