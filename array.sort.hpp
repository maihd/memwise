#ifndef __ARRAY_SORT_HPP__
#define __ARRAY_SORT_HPP__

#include "sort.hpp"
#include "array.hpp"

namespace array
{
    template <typename type_t>
    void sort(array_t<type_t>& array)
    {
        ::sort::quick(array, array.count);
    }

    template <typename type_t>
    void sort(array_t<type_t>& array, int start, int end)
    {
        assert(start > -1 && start < end);
        ::sort::quick(array.elements + start, end - start);
    }
}

#endif /* __ARRAY_SORT_HPP__ */