#ifndef __ARRAY_SORT_HPP__
#define __ARRAY_SORT_HPP__

#include "sort.hpp"
#include "array.hpp"

namespace array
{
    template <typename type_t>
    inline void sort(array_t<type_t>& array)
    {
        ::sort::quick(array, array.count);
    }

    template <typename type_t>
    inline void sort(array_t<type_t>& array, int start, int end)
    {
        assert(start > -1 && start < end && end <= array.count);
        ::sort::quick(array.elements + start, end - start);
    }

    template <typename type_t, typename func_t>
    inline void sort(array_t<type_t>& array, const func_t& compare)
    {
        ::sort::quick(array, array.count, compare);
    }

    template <typename type_t, typename func_t>
    inline void sort(array_t<type_t>& array, int start, int end, const func_t& compare)
    {
        assert(start > -1 && start < end && end <= array.count);
        ::sort::quick(array.elements + start, end - start, compare);
    }
}

#endif /* __ARRAY_SORT_HPP__ */