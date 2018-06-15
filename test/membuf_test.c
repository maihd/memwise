#include <stdio.h>
#include <stdlib.h>

#define MEMBUF_IMPL
#include "membuf.h"

int main(int argc, char* argv[])
{
    membuf_t* heap = membuf_heap();

    int* arr = (int*)membuf_extract(heap, 20 * sizeof(int), 4);
    for (int i = 0; i < 20; i++)
    {
	arr[i] = i;
    }

    for (int i = 0; i < 20; i++)
    {
	printf("arr[%d]: %d\n", i, arr[i]);
    }

    membuf_collect(heap, arr);

    membuf_t* pool = membuf_new_pool(10, 20 * sizeof(int));
    arr = (int*)membuf_extract(pool, 20 * sizeof(int), 4);
    for (int i = 0; i < 20; i++)
    {
	arr[i] = i;
    }

    for (int i = 0; i < 20; i++)
    {
	printf("arr[%d]: %d\n", i, arr[i]);
    }
    membuf_free(pool);
    return 0;
}
