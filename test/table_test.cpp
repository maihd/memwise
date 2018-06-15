#include <stdio.h>
#include <stdlib.h>

#include "../table.hpp"

#define MEMBUF_IMPL
#include "../membuf.h"

int main(int argc, char* argv[])
{   
    table_t<int, int> tb(membuf_heap(), 64);

    table::set(tb, 10, 100);
    if (table::has(tb, 10))
    {
	    printf("tb[%d]: %d\n", 10, table::get(tb, 10, -1));
    }
    else
    {
	    printf("tb[%d]: is not exists\n", 10);
    }

    int val;
    if (table::tryget(tb, 10, val))
    {
	    printf("tb[%d]: %d, try_get successfully\n", 10, val);
    }

    if (table::remove(tb, 10))
    {
	    printf("tb[%d]: remove successfully\n", 10);
    }

    if (table::has(tb, 10))
    {
	    printf("tb[%d]: %d\n", 10, table::get(tb, 10, -1));
    }
    else
    {
	    printf("tb[%d] has been removed\n", 10);
    }
    
    /* tb auto free at here */
    return 0;
}
