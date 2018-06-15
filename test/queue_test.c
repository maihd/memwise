#include <stdio.h>
#include <stdlib.h>

#define QUEUE_CONFIG
#define QUEUE_PREFIX iqueue
#include "queue.h"

#define QUEUE_IMPL
#define queue_element_t int
#include "queue.h"

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
    membuf_default.data = NULL;
    membuf_default.collect = membuf_default_collect;
    membuf_default.extract = membuf_default_extract;

    iqueue_t que;
    iqueue_init(&que, &membuf_default);

    iqueue_enque(&que, 10);
    iqueue_enque(&que, 10);
    iqueue_enque(&que, 10);
    iqueue_enque(&que, 10);
    iqueue_enque(&que, 10);
    iqueue_enque(&que, 10);
    iqueue_enque(&que, 10);
    iqueue_enque(&que, 10);
    iqueue_enque(&que, 10);
    iqueue_enque(&que, 10);
    iqueue_enque(&que, 10);
    iqueue_enque(&que, 10);
    iqueue_enque(&que, 10);
    iqueue_enque(&que, 10);
    iqueue_enque(&que, 10);
    iqueue_enque(&que, 10);

    printf("queue_peek(): %d\n", iqueue_peek(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    printf("queue_deque(): %d\n", iqueue_deque(&que));
    
    iqueue_free(&que);
    return 0;
}
