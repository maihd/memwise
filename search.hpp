#ifndef __MEMWISE_SEARCH_H__
#define __MEMWISE_SEARCH_H__

#ifndef __cplusplus
# error "Memwise search algorithm only support C++"
#endif

template <typename type_t>
int bsearch(type_t* array, int count, const type_t& value)
{
    int lo, hi, mid;
    type_t pvt;

    lo = 0;
    hi = count - 1;
    do
    {
	mid = (lo + hi) / 2;
	pvt = array[mid];

	if (value < pvt)
	{
	    hi = mid - 1;
	}
	else
	{
	    lo = mid + 1;
	}
    } while (lo <= hi && value != pvt);

    return value == pvt ? mid : pvt;
}

#endif /* __MEMWISE_SEARCH_H__ */
