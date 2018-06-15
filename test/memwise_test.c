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
    /* generic: array_push(*arr, value); */
    /* faster version */
    intarr_set(arr, arr->count, value);
}

int intarr_pop(intarr_t* arr)
{
    return array_pop(*arr);
}

int intarr_get(intarr_t* arr, int idx)
{
    return array_get(*arr, idx);
}

int* intarr_ref(intarr_t* arr, int idx)
{
    return array_ref(*arr, idx);
}

int intarr_find(intarr_t* arr, int value)
{
    int idx;
    array_find(*arr, value, idx);
    return idx;
}

int intarr_rfind(intarr_t* arr, int value)
{
    int idx;
    array_rfind(*arr, value, idx);
    return idx;
}

void intarr_erase(intarr_t* arr, int idx)
{
    array_erase(*arr, idx);
}

int intarr_remove(intarr_t* arr, int value)
{
    /* generic: array_remove(*arr, value) */
    /* better version */
    int idx = intarr_find(arr, value);
    if (idx > -1)
    {
	intarr_erase(arr, idx);
    }
    return idx;
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

    printf("Add 10 elements has value from '1' to '10'\n");
    intarr_push(&arr, 1);
    intarr_push(&arr, 2);
    intarr_push(&arr, 3);
    intarr_push(&arr, 4);
    intarr_push(&arr, 5);
    intarr_push(&arr, 6);
    intarr_push(&arr, 7);
    intarr_push(&arr, 8);
    intarr_push(&arr, 9);
    intarr_push(&arr, 10);
    	
    array_foreach(arr, int, i, v)
    {
	printf("%2d. %d\n", i + 1, v);
    }

    printf("\n");
    printf("Find and erase element has value '4'\n");
    int idx = intarr_find(&arr, 4);
    if (idx > -1)
    {
	intarr_erase(&arr, idx);
    }

    printf("Remove the element has value '9'\n");
    idx = intarr_remove(&arr, 9);

    printf("\n");
    printf("New arrays:\n");
    for (int i = 0, n = arr.count; i < n; i++)
    {
	printf("%2d. %d\n", i + 1, intarr_get(&arr, i));
    }
    
    intarr_free(&arr);
    
    return 0;
}
