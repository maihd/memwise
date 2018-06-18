#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMBUF_IMPL
#include "../membuf.h"

#define ARRAY_CONFIG
#define ARRAY_PREFIX intarr
#include "../array.h"

#define ARRAY_IMPL
#define array_element_t int
#include "../array.h"

int main(int argc, char* argv[])
{
    membuf_t* heap = membuf_heap();
    
    intarr_t arr;
    intarr_init(&arr, heap);

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
    printf("==> [DONE] Test array_push() successfully.\n");

    int i;
    int v;
    for (i = 0; i < arr.count; i++)
    {
	v = arr.elements[i];
	printf("%2d. %d\n", i + 1, v);
    }

    printf("\n");
    printf("Find and erase element has value '4'\n");
    int idx = intarr_find(&arr, 4);
    if (idx > -1)
    {
	intarr_erase(&arr, idx);
    }
    printf("==> [DONE] Test array_find() and array_erase() successfully.\n");

    printf("Remove the element has value '9'\n");
    idx = intarr_remove(&arr, 9);
    printf("==> [DONE] Test array_remove() successfully.\n");

    printf("\n");
    printf("Array current values:\n");
    for (int i = 0, n = arr.count; i < n; i++)
    {
	printf("%2d. %d\n", i + 1, intarr_get(&arr, i));
    }
    printf("==> [DONE] Test array_get() successfully.\n");
    
    intarr_free(&arr);
    return 0;
}
