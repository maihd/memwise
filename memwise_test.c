#include <stdio.h>
#include "memwise.h"

typedef array_t(int) intarr_t;

void* membuf_default_extract(void* data, int size, int align)
{
    (void)data;
    (void)align;
    return malloc(size);
}

void membuf_default_collect(void* data, void* pointer)
{
    (void)data;
    free(pointer);
}

void intarr_init(intarr_t* arr, membuf_t* membuf)
{
    array_init(*arr, membuf);
}

void intarr_free(intarr_t* arr)
{
    array_free(*arr);
}

void intarr_set(intarr_t* arr, int idx, int value)
{
    array_set(*arr, idx, value);
}

void intarr_push(intarr_t* arr, int value)
{
    array_push(*arr, value);
}

int intarr_pop(intarr_t* arr)
{
    return array_pop(*arr);
}

int main(int argc, char* argv[])
{
    printf("Memwise unit test\n");
    
    membuf_t membuf_default;
    membuf_default.data = NULL;
    membuf_default.collect = membuf_default_collect;
    membuf_default.extract = membuf_default_extract;
    
    intarr_t arr;
    intarr_init(&arr, &membuf_default);
    intarr_push(&arr, 10);
    intarr_push(&arr, 10);
    intarr_push(&arr, 10);
    intarr_push(&arr, 10);
    intarr_push(&arr, 10);
    intarr_push(&arr, 10);
    intarr_push(&arr, 10);
    intarr_push(&arr, 10);
    intarr_push(&arr, 10);
    intarr_push(&arr, 10);

    array_foreach(arr, int, i, v)
    {
	printf("%2d. %d\n", i + 1, v);
    }
    
    intarr_free(&arr);
    
    return 0;
}
