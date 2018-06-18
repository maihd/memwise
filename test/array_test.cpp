#include <stdio.h>
#include "../array.hpp"

#define MEMBUF_IMPL
#include "../membuf.h"

int main(int argc, char* argv[])
{
    array_t<int> arr(membuf_heap());

    array::set(arr, 0, 10);
    array::push(arr, 10);

    printf("arr[%d] = %d\n", 0, arr[0]);
    printf("arr[%d] = %d\n", 0, array::get(arr, 0));

    printf("arr[%d] = %d\n", 1, arr[1]);
    printf("arr[%d] = %d\n", 1, array::get(arr, 1));

    /* Add new slot */
    printf("arr.count = %d\n", arr.count);
    array::add(arr) = 10;
    printf("arr.count = %d\n", arr.count);

    return 0;
}
