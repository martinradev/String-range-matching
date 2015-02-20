/*
 * Output iterator that counts the number of times a value has been assigned to
 * it.
 *
 * Copyright (c) 2015 Jarno Leppänen
 */

#ifndef COUNT_ITERATOR_HPP
#define COUNT_ITERATOR_HPP

template <typename container>
struct count_iterator {

    typedef typename container::value_type container_value_type;

    /* typedefs required for stl iterators. */
    typedef void difference_type;
    typedef void value_type;
    typedef void pointer;
    typedef void reference;
    typedef std::output_iterator_tag iterator_category;

    container& c;

    count_iterator(container& c) : c(c)
    {
        c.push_back(0);
    }
    count_iterator& operator=(const container_value_type&)
    {
        ++c.back();
        return *this;
    }
    count_iterator& operator*() { return *this; }
    count_iterator& operator++() { return *this; }
    count_iterator& operator++(int) { return *this; }
};

template <typename container>
count_iterator<container> counter(container& c)
{
    return count_iterator<container>(c);
}

#endif // COUNT_ITERATOR_HPP
