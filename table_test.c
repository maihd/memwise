#include <stdio.h>
#include <stdlib.h>

#define TABLE_CONFIG
#define TABLE_PREFIX  itable
#include "table.h"

#define table_key_t   int
#define table_value_t int
#define TABLE_HASHFUNC(x) (x)
#define TABLE_KEY_EQUAL(a, b) ((a) == (b))
#define TABLE_IMPL
#include "table.h"

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
    membuf_t membuf_default;
    membuf_default.data    = NULL;
    membuf_default.collect = membuf_default_collect;
    membuf_default.extract = membuf_default_extract;
    
    itable_t tb;
    itable_init(&tb, &membuf_default);

    itable_set(&tb, 10, 100);
    if (itable_has(&tb, 10))
    {
	printf("tb[%d]: %d\n", 10, itable_get(&tb, 10, -1));
    }
    else
    {
	printf("tb[%d]: is not exists\n", 10);
    }

    int val;
    if (itable_try_get(&tb, 10, &val))
    {
	printf("tb[%d]: %d, try_get successfully\n", 10, val);
    }

    if (itable_remove(&tb, 10))
    {
	printf("tb[%d]: remove successfully\n", 10);
    }

    if (itable_has(&tb, 10))
    {
	printf("tb[%d]: %d\n", 10, itable_get(&tb, 10, -1));
    }
    else
    {
	printf("tb[%d]: is not exists\n", 10);
    }
    
    itable_free(&tb);
    return 0;
}
