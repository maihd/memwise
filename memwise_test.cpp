#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "memwise.h"

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

int main(int argc, char* argv[])
{
    srand(time(NULL));
    
    printf("Memwise unit test\n");
    
    membuf_t membuf_default;
    membuf_default.data    = NULL;
    membuf_default.collect = membuf_default_collect;
    membuf_default.extract = membuf_default_extract;
    
    array_t<int> arr;
    array_init(arr, &membuf_default);

    printf("Add 10 elements has value from '1' to '10'\n");
    array_push(arr, 1);
    array_push(arr, 2);
    array_push(arr, 3);
    array_push(arr, 4);
    array_push(arr, 5);
    array_push(arr, 6);
    array_push(arr, 7);
    array_push(arr, 8);
    array_push(arr, 9);
    array_push(arr, 10);
    
	
    array_foreach(arr, int, i, v)
    {
	printf("%2d. %d\n", i + 1, v);
    }

    printf("\n");
    printf("Find and erase element has value '4'\n");
    int idx = array_find(arr, 4);
    if (idx > -1)
    {
	array_erase(arr, idx);
    }

    printf("Remove the element has value '9'\n");
    array_remove(arr, 9);

    printf("\n");
    printf("New arrays:\n");
    for (int i = 0, n = arr.count; i < n; i++)
    {
	printf("%2d. %d\n", i + 1, array_get(arr, i));
    }
    
    array_free(arr);

    table_t<int, int> table;
    table_init(table, &membuf_default);

    int count = 10;
    while (count--)
    {
	int key = rand() % 100;
	table_set(table, key, rand());
	printf("table[%2d] => %d\n", key, table_get(table, key));
    }
    
    table_free(table);
    
    return 0;
}
