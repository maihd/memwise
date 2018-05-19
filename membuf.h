#ifndef __MEMBUF_H__
#define __MEMBUF_H__

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
#endif

#ifndef MEMBUF_API
#define MEMBUF_API
#endif

MEMBUF_API membuf_t* membuf_heap(void);

#endif /* __MEMWISE_MEMBUF_H__ */

#ifdef MEMBUF_IMPL

#include <stdlib.h>

static void* membuf_heap_extract(void* data, int size, int align)
{
    (void)data;
    (void)align;
    return malloc(size);
}

static void  membuf_heap_collect(void* data, void* pointer)
{
    (void)data;
    free(pointer);
}

static membuf_t membuf__heap =
{
    NULL,
    &membuf_heap_extract,
    &membuf_heap_collect,
};

membuf_t* membuf_heap(void)
{
    return &membuf__heap;
}

#endif
