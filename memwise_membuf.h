#ifndef __MEMWISE_MEMBUF_H__
#define __MEMWISE_MEMBUF_H__

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

#endif /* __MEMWISE_MEMBUF_H__ */
