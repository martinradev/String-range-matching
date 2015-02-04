#ifndef PUSH_BACK_ITERATOR_HPP
#define PUSH_BACK_ITERATOR_HPP

#include <iterator>

template <typename C>
struct push_back_iterator : public std::back_insert_iterator<C> {
    push_back_iterator(C& c): std::back_insert_iterator<C>(c) {}
};

namespace std {
    template <typename C>
    struct iterator_traits<push_back_iterator<C>> :
    public std::iterator_traits<back_insert_iterator<C>> {
        typedef typename C::value_type value_type;
    };
}

template <typename C>
push_back_iterator<C> push_backer(C& c)
{
    return push_back_iterator<C>(c);
}

#endif
