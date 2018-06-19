#include <stdio.h>
#include <stdlib.h>

#include "../queue.hpp"

#define MEMBUF_IMPL
#include "../membuf.h"

int main(int argc, char* argv[])
{
    queue_t<int> que(membuf_heap());

    queue::enque(que, 10);
    queue::enque(que, 10);
    queue::enque(que, 10);
    queue::enque(que, 10);
    queue::enque(que, 10);
    queue::enque(que, 10);
    queue::enque(que, 10);
    queue::enque(que, 10);
    queue::enque(que, 10);
    queue::enque(que, 10);
    queue::enque(que, 10);
    queue::enque(que, 10);
    queue::enque(que, 10);
    queue::enque(que, 10);
    queue::enque(que, 10);
    queue::enque(que, 10);

    printf("queue_peek():  %d\n", queue::peek (que));
    printf("queue_deque(): %d\n", queue::deque(que));
    printf("queue_deque(): %d\n", queue::deque(que));
    printf("queue_deque(): %d\n", queue::deque(que));
    printf("queue_deque(): %d\n", queue::deque(que));
    printf("queue_deque(): %d\n", queue::deque(que));
    printf("queue_deque(): %d\n", queue::deque(que));
    printf("queue_deque(): %d\n", queue::deque(que));
    printf("queue_deque(): %d\n", queue::deque(que));
    printf("queue_deque(): %d\n", queue::deque(que));
    printf("queue_deque(): %d\n", queue::deque(que));
    printf("queue_deque(): %d\n", queue::deque(que));
    printf("queue_deque(): %d\n", queue::deque(que));
    printf("queue_deque(): %d\n", queue::deque(que));
    printf("queue_deque(): %d\n", queue::deque(que));
    printf("queue_deque(): %d\n", queue::deque(que));
    printf("queue_deque(): %d\n", queue::deque(que));
    
    /* que implicit free here */
    return 0;
}
