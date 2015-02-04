#ifndef MALLOCATE_HPP
#define MALLOCATE_HPP

#include <cstddef>
#include <new>

extern "C" void *mallocate(size_t bytes);

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

    template <class U>
    struct rebind { typedef mallocator<U> other; };

    mallocator() {}
    mallocator(const mallocator&) {}
    template <class U>
    mallocator(const mallocator<U>&) {}
    ~mallocator() {}

    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const
    {
        return x;
    }

    pointer allocate(size_type n, const_pointer = 0)
    {
        void *p = mallocate(n*sizeof(T));
        if (!p)
          throw std::bad_alloc();
        return static_cast<pointer>(p);
    }

    void deallocate(pointer p, size_type)
    {
        ::operator delete(p);
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

private:
    void operator=(const mallocator&);
};

template<> class mallocator<void>
{
    typedef void        value_type;
    typedef void*       pointer;
    typedef const void* const_pointer;

    template <class U>
    struct rebind { typedef mallocator<U> other; };
};

template <class T>
inline bool operator==(const mallocator<T>&, const mallocator<T>&)
{
    return true;
}

template <class T>
inline bool operator!=(const mallocator<T>&, const mallocator<T>&)
{
    return false;
}

#endif
