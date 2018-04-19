#ifndef __MEMWISE_SORT_H__
#define __MEMWISE_SORT_H__

#ifndef __cplusplus
# error "Generics sorting functions is not support non-C++ languages"
#endif

#if   _MSC_VER
#define __mwinline__ __forceinline inline
#else
#defien __mwinline__ __attribute__((always_inline)) inline
#endif

/**
 * Swap value between two variables
 */
template <typename type_t>
__mwinline__ void memwise__swap(type_t& a, type_t& b)
{
    type_t c = a;
    a = b;
    b = c;
}

/** 
 * Default compare function
 */
template <typename type_t>
__mwinline__ int memwise__defcmp(const type_t& a, const type_t& b)
{
    return a - b;
}

/**
 * Insert sort algorithm implement
 */
template <typename type_t, typename func_t>
__mwinline__ void isort_impl(type_t* array, int count, const func_t& compare)
{
    /* Nothing for sorting */
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

/**
 * Insertion sorting
 */
template <typename type_t>
void isort(type_t* array, int count)
{
    isort_impl(array, count, memwise__defcmp<type_t>);
}

/**
 * Insertion sorting with custom compare function
 */
template <typename type_t, typename func_t>
void isort(type_t* array, int count, const func_t& compare)
{
    isort_impl(array, count, compare);
}

/**
 * Quick sort
 */
template <typename type_t>
void quicksort(type_t* array, int count)
{
    if (count <= 2) return;

    const int maxlevels = 128;
    int lo[maxlevels] = { 0 };
    int hi[maxlevels] = { count - 1 };
    int lv = 0;

    while (lv > -1)
    {
	int i = lo[lv];
	int j = hi[lv];
	if ((j - i) >= 4 && (lv < maxlevels - 1))
	{
	    int idx = (i + j) / 2;

	    type_t pivot = array[idx];
	    memwise__swap(array[idx], array[j--]);

	    do
	    {
		while (array[i] < pivot && ++i >= j) break;
		while (array[j] > pivot && --j <= i) break;

		memwise__swap(array[i], array[j]);
		i += 1;
		j -= 1;
	    } while (i < j);

	    while (array[i] <= pivot && i < hi[lv]) i++;
	    while (array[j] >= pivot && j > lo[lv]) j--;

	    memwise__swap(array[i], array[hi[lv]]);

	    lo[lv + 1] = i;
	    hi[lv + 1] = hi[lv];
	    hi[lv] = j;
	    lv += 1;
	}
	else
	{
	    for (; j > i; j--)
	    {
		int m = i;
		for (int k = i + 1; k <= j; k++)
		{
		    if (array[k] > array[m])
		    {
			m = k;
		    }
		}
		memwise__swap(array[m], array[j]);
	    }
	    
	    lv -= 1;
	}
    }
}

/**
 * Quick sort with custom compare functions
 */
template <typename type_t, typename func_t>
void quicksort(type_t* array, int count, const func_t& compare)
{
    if (count <= 2) return;

    const int maxlevels = 128;
    int lo[maxlevels] = { 0 };
    int hi[maxlevels] = { count - 1 };
    int lv = 0;

    while (lv > -1)
    {
	int i = lo[lv];
	int j = hi[lv];
	if ((j - i) >= 4 && (lv < maxlevels - 1))
	{
	    int idx = (i + j) / 2;

	    type_t pivot = array[idx];
	    memwise__swap(array[idx], array[j--]);

	    do
	    {
		while (compare(array[i], pivot) < 0 && ++i >= j) break;
		while (compare(array[j], pivot) && --j <= i) break;

		memwise__swap(array[i], array[j]);
		i += 1;
		j -= 1;
	    } while (i < j);

	    while (compare(array[i], pivot) <= 0 && i < hi[lv]) i++;
	    while (compare(array[j], pivot) >= 0 && j > lo[lv]) j--;

	    memwise__swap(array[i], array[hi[lv]]);

	    lo[lv + 1] = i;
	    hi[lv + 1] = hi[lv];
	    hi[lv] = j;
	    lv += 1;
	}
	else
	{
	    for (; j > i; j--)
	    {
		int m = i;
		for (int k = i + 1; k <= j; k++)
		{
		    if (compare(array[k], array[m]) > 0)
		    {
			m = k;
		    }
		}
		memwise__swap(array[m], array[j]);
	    }
	    
	    lv -= 1;
	}
    }
}

template <typename type_t>
void shellsort(type_t* array, int count)
{
    if (count < 2) return;

    int gap, i, j;
    for (gap = count / 2; gap > 0; gap /= 2)
    {
	for (i = gap; i < count; i++)
	{
	    for (j = i - gap; j >= 0 && array[j] > array[j + gap]; j -= gap)
	    {
		memwise__swap(array[i], array[j]);
	    }
	}
    }
}

#endif /* __SORT_H__ */
