#if defined(QUEUE_CLEAN)
#undef QUEUE_CLEAN

#elif defined(QUEUE_CONFIG)
#undef QUEUE_CONFIG

#undef QUEUE__DEFINE
#undef QUEUE__CONCAT
#undef QUEUE__CONCAT_IN
#define QUEUE__DEFINE(name) QUEUE__CONCAT(QUEUE_PREFIX, name)
#define QUEUE__CONCAT(a, b) QUEUE__CONCAT_IN(a, b)
#define QUEUE__CONCAT_IN(a, b) a ## b

#define queue_t     QUEUE__DEFINE(_t)
#define queue_init  QUEUE__DEFINE(_init)
#define queue_free  QUEUE__DEFINE(_free)
#define queue_peek  QUEUE__DEFINE(_peek)
#define queue_enque QUEUE__DEFINE(_enque)
#define queue_deque QUEUE__DEFINE(_deque)

#else

#ifndef HAS_MEMBUF_T
#define HAS_MEMBUF_T
/**
 * Memory buffer
 */
typedef struct
{
    void* data;
    void  (*collect)(void* data, void* pointer);
    void* (*extract)(void* data, int size, int align);
} membuf_t;

#define membuf_collect(buf, ptr)         (buf)->collect((buf)->data, ptr)
#define membuf_extract(buf, size, align) (buf)->extract((buf)->data, size, align)
#endif /* HAS_MEMBUF_T */

#ifndef __cplusplus
# if __STDC_VERSION__ == 201112L
#  include <stdbool.h>
# else
#  ifndef bool
#  define bool  unsigned char
#  endif
#  ifndef true
#  define true  0
#  endif
#  ifndef false
#  define false 1
#  endif
# endif
#endif

#ifndef QUEUE_API
#define QUEUE_API
#endif

typedef struct
{
    int              head;
    int              tail;
    int              capacity;
    queue_element_t* elements;
    membuf_t*        membuffer;
} queue_t;

QUEUE_API void queue_init(queue_t* queue, membuf_t* membuffer);
QUEUE_API void queue_free(queue_t* queue);

QUEUE_API queue_element_t queue_peek(const queue_t* queue);
QUEUE_API bool            queue_enque(queue_t* queue, queue_element_t element);
QUEUE_API queue_element_t queue_deque(queue_t* queue);

#if defined(QUEUE_IMPL)

#include <string.h>
#ifndef queue_assert
#include <assert.h>
#define queue_assert(q, e) assert(q != NULL); assert(e)
#endif

#ifndef QUEUE_CAPACITY
#define QUEUE_CAPACITY 64
#endif

void queue_init(queue_t* queue, membuf_t* membuffer)
{
    queue_assert(queue, membuffer != NULL);
    
    queue->head      = 0;
    queue->tail      = 0;
    queue->capacity  = 0;
    queue->elements  = 0;
    queue->membuffer = membuffer;
}

void queue_free(queue_t* queue)
{
    queue_assert(queue, queue->membuffer != NULL);

    membuf_collect(queue->membuffer, queue->elements);

    queue->head      = 0;
    queue->tail      = 0;
    queue->capacity  = 0;
    queue->elements  = 0;
}

queue_element_t queue_peek(const queue_t* queue)
{
    queue_assert(queue, queue->head < queue->tail);
    return queue->elements[queue->head];
}

bool queue_enque(queue_t* queue, queue_element_t element)
{
    queue_assert(queue, queue->membuffer != NULL);
    
    if (queue->tail >= queue->capacity)
    {
	if (queue->head > 0)
	{
	    const int count  = queue->tail - queue->head;
	    const int offset = queue->head;
	    memcpy(queue->elements,
		   queue->elements + offset,
		   count * sizeof(queue_element_t));
	    
	    queue->head = 0;
	    queue->tail = count;
	}
	else
	{
	    const int old_size = queue->capacity * sizeof(queue_element_t);
	    if (queue->capacity <= 0) queue->capacity  = QUEUE_CAPACITY;
	    else                      queue->capacity *= 2;
	    const int new_size = queue->capacity * sizeof(queue_element_t);

	    void* old_data = queue->elements;
	    void* new_data = membuf_extract(queue->membuffer, new_size, 4);
	    if (new_data)
	    {
		memcpy(new_data, old_data, old_size);
		membuf_collect(queue->membuffer, old_data);

		queue->elements = (queue_element_t*)new_data;
	    }
	    else
	    {
		return false;
	    }
	}
    }
    
    queue->elements[queue->tail++] = element;
    return true;
}

queue_element_t queue_deque(queue_t* queue)
{
    queue_assert(queue, queue->head < queue->tail);
    return queue->elements[queue->head++];
}

#endif

#endif
