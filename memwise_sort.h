#ifndef __MEMWISE_SORT_H__
#define __MEMWISE_SORT_H__

#ifndef __cplusplus
# error "Generics sorting functions is not support non-C++ languages"
#endif

template <typename type_t>
inline void memwise__swap(type_t& a, type_t& b)
{
    type_t c = a;
    a = b;
    b = c;
}

template <typename type_t>
inline int defcmp(const type_t& a, const type_t& b)
{
    return a - b;
}

template <typename type_t>
void isort(type_t* array, int count)
{
    /* Nothing for sorting
     */
    if (count <= 1) return;

    type_t* lo = array;
    type_t* hi = array + count - 1;
    while (lo < hi)
    {
	type_t* max = lo;
	for (type_t* p = lo + 1; p <= hi; p++)
	{
	    if (*max < *p)
	    {
		max = p;
	    }
	}
	memwise__swap(*max, *hi);
    }
}

template <typename type_t, typename func_t>
void isort(type_t* array, int count, const func_t& compare)
{
    /* Nothing for sorting
     */
    if (count <= 1) return;

    type_t* lo = array;
    type_t* hi = array + count - 1;
    while (lo < hi)
    {
	type_t* max = lo;
	for (type_t* p = lo + 1; p <= hi; p++)
	{
	    if (compare(*max, *p) < 0)
	    {
		max = p;
	    }
	}
	memwise__swap(*max, *hi--);
    }
}

#endif /* __SORT_H__ */
