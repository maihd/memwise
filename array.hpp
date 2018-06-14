#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

#ifndef __cplusplus
#error "This array version does not support other languages from C++"
#endif

#include <assert.h>

/* COPY FROM membuf.h */
#ifndef HAS_MEMBUF_T
#define HAS_MEMBUF_T
/**
 * Memory buffer
 */
typedef struct
{
    void* data;

    void  (*clear)(void* data);
    void* (*resize)(void* data, void* ptr, int size, int align);
    void  (*collect)(void* data, void* pointer);
    void* (*extract)(void* data, int size, int align);
} membuf_t;

#define membuf_clear(buf)                (buf)->clear((buf)->data)
#define membuf_resize(buf, ptr, s, a)    (buf)->resize((buf)->data, ptr, s, a)
#define membuf_collect(buf, ptr)         (buf)->collect((buf)->data, ptr)
#define membuf_extract(buf, size, align) (buf)->extract((buf)->data, size, align)
#endif

template <typename type_t>
union array_t /* union instead of other type for make sure there is no vtable */
{   
public: /* Property feature implement */
    template <typename T>
    union property
    {
    public:
	inline property(void)           : value()      {}
	inline property(const T& value) : value(value) {}

	inline operator const T&(void) const {
	    return value; 
	}
	
    private:
	inline T& operator=(const T& other) {
	    return (value = other);
	}
	
    private:
	T value;
	friend array_t<type_t>;
    };
    
public: /* Fields */
    struct
    {
	property<int>       count;
	property<int>       capacity;
	property<type_t*>   elements;
	property<membuf_t*> membuffer;
    };
    
public:
    inline array_t(membuf_t* membuf)
	: count(0)
	, capacity(0)
	, elements(0)
	, membuffer(membuf)
	{}
    
    inline ~array_t(void) {
	membuf_collect((membuf_t*)membuffer, elements);
	
	count     = 0;
	capacity  = 0;
	elements  = 0;
	membuffer = 0;
    }

    inline type_t& operator[](int index) {
	assert(index >= 0 && index < count);
	return elements[index];
    }
    
    inline const type_t& operator[](int index) const {
	assert(index >= 0 && index < count);
	return elements[index];
    }

    inline operator type_t*(void) {
	return elements;
    }
    
    inline operator const type_t*(void) const {
	return elements;
    }
};

namespace array
{
    template <typename type_t>
    inline bool expand(array_t<type_t>& array, int capacity)
    {
	if (array.capacity < capacity)
	{
	    int new_capacity = array.capacity > 0 ? (int)array.capacity : 64;
	    while (new_capacity < capacity)
	    {
		new_capacity *= 2;
	    }

	    *((int*)&array.capacity)     = new_capacity;
	    *((type_t**)&array.elements) =
		(type_t*)membuf_resize((membuf_t*)array.membuffer,
				       array.elements,
				       new_capacity * sizeof(type_t), 4);
	    return array.elements != NULL;
	}
	else
	{
	    return true;
	}
    }
    
    template <typename type_t>
    inline bool ensure(array_t<type_t>& array, int capacity)
    {
	if (array.capacity < capacity)
	{
	    return expand(array, capacity);
	}
	else
	{
	    return true;
	}
    }

    template <typename type_t>
    inline bool set(array_t<type_t>& array, int index, const type_t& value)
    {
	if (index >= array.count)
	{
	    *((int*)&array.count) = index + 1;
	}
	
	if (ensure(array, array.count))
	{
	    array[index] = value;
	    return true;
	}
	else
	{
	    return false;
	}
    }

    template <typename type_t>
    inline const type_t& get(const array_t<type_t>& array, int index)
    {
	assert(index >= 0 && index < array.count);
	return array.elements[index];
    }

    template <typename type_t>
    inline bool push(array_t<type_t>& array, type_t value)
    {
	return set(array, array.count, value);
    }

    template <typename type_t>
    inline const type_t& pop(array_t<type_t>& array)
    {
	assert(array.count > 0);
	*((int*)&array.count) = array.count - 1;
	return array.elements[array.count];
    }
};

#endif
