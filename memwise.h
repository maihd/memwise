#ifndef __MEMWISE_H__
#define __MEMWISE_H__


#ifndef MEMWISE_NOSTD
# include <assert.h>
# include <stdlib.h>
#endif

#define __MCONCAT(x, y) x ## y
#define _MCONCAT(x, y) __MCONCAT(x, y)
#define _MGENSYM(x)    _MCONCAT(x, __LINE__)

#define MEMWISE_ERROR_INDEX  0
#define MEMWISE_ERROR_EMPTY  1
#define MEMWISE_ERROR_MEMORY 2

#define MEMWISE_ERROR(func, code)  func ": " VLIB_ERROR_STRING(code)
#define MEMWISE_ERROR_STRING(code) _MCONCAT(VLIB_ERROR_STRING, code)
#define MEMWISE_ERROR_STRING0      "Index out of range"
#define MEMWISE_ERROR_STRING1      "No elements"
#define MEMWISE_ERROR_STRING2      "Out of memory"

#define MEMWISE_ASSERT(exp, f, e)  assert(exp, MEMWISE_ERROR(f, e))

/* ----------------- MEMORY BUFFER ----------------- */
/**
 * Memory buffer
 */
typedef struct
{
    void* data;
    void  (*collect)(void* data, void* pointer);
    void* (*extract)(void* data, int size, int align);
} membuf_t;

#define membuf_collect(buf, ptr)  (buf)->collect((buf)->data, ptr)
#define membuf_extract(buf, s, a) (buf)->extract((buf)->data, s, a)

/* ----------------- CONTAINERS -------------------- */
#if MEMWISE_C_VERSION || !defined(__cplusplus)
/* BEGIN OF C VERSIONS */

#define MEMWISE_DEFCMP(a, b) ((a) == (b))

/**************************
 * Temporary array
 **************************/
#define TEMPO_FIELDS(type_t, c) int count; type_t elements[c]
#define tempo_t(type_t, c)      struct { TEMPO_FIELDS(type_t, c); }
#define tempo_init(t) do { (t).count = 0; } while (0)
#define tempo_count(t)    (t).count
#define tempo_capacity(t) (sizeof((t).elements) / sizeof((t).elements[0]))
#define tempo_elements(t) (t).elements
#define tempo_set(t, i, e)						\
    do {								\
	if (i <= (t).count) {						\
	    (t).count = i + 1;						\
	    MEMWISE_ASSERT((t).count <= tempo_capacity(t),		\
			   "tempo_set", MEMWISE_ERROR_MEMORY);		\
	}								\
	(t).elements[i] = e;						\
    } while (0)

#define tempo_get(t, i)							\
    (MEMWISE_ASSERT(i > -1 && i < (t).count,				\
		    "tempo_get", MEMWISE_ERROR_INDEX),			\
     (t).elements[i])

#define tempo_ref(t, i)							\
    (MEMWISE_ASSERT(i > -1 && i < tempo_capacity(t),			\
		    "tempo_ref", MEMWISE_ERROR_INDEX),		\
     (t).elements + i)

#define tempo_pop(t)							\
    (MEMWISE_ASSERT((t).count > 0,					\
		    "tempo_pop", MEMWISE_ERROR_COUNT),		\
     (t).elements[--(t).count])

#define tempo_push(t, e)					\
    do {							\
	int _i_ = (t).count;					\
	tempo_set(t, _i_, e);					\
    } while (0)

#define tempo_find(t, e, i) tempo_find_cmp(t, e, i, MEMWISE_DEFCMP)
#define tempo_find_cmp(t, e, i, cmp)			\
    do {						\
	int _j_; uint_t _m_ = (t).count;		\
	for (_j_ = 0; _j_ < _m_; _j_++)			\
	    if (cmp(e, (t).elements[_j_])) {		\
	        (i) = _j_; break;			\
	    }						\
    } while (0)

#define tempo_erase(t, i)						\
    do {								\
	MEMWISE_ASSERT(!(i < 0 || i >= (t).count),			\
		       "tempo_erase", MEMWISE_ERROR_INDEX);		\
	int _i_; uint_t _n_ = (t).count;				\
	for (_i_ = i; _i_ < _n_; _i_++)					\
	    (t).elements[_i_] = (t).elements[_i_ + 1];			\
    } while (0)

#define tempo_remove(t, e) tempo_remove_cmp(t, e, MEMWISE_DEFCMP)
#define tempo_remove_cmp(t, e, cmp)			\
    do {						\
	int _k_;					\
	tempo_find_cmp(t, e, _k_, cmp);			\
	temp_erase(t, _k_);				\
    } while (0)


#define tempo_insert(t, i, e)						\
    do {								\
        MEMWISE_ASSERT((t).count < tempo_capacity(t),			\
		       "tempo_insert", MEMWISE_ERROR_MEMORY);		\
	int _i_; uint_t _n_ = (t).count++, _e_ = i;			\
	for (_i_ = _n_; _i_ < _e_; _i_--)				\
	    (t).elements[_i_] = (t).elements[_i_ - 1];			\
	(t).elements[_e_] = e;						\
    } while (0)


#define tempo__foreach(t, type_t, v, i, n, elm)				\
    type_t v; type_t* elm = (t).elements;				\
    int i = 0; uint_t n = (t).count;					\
    for (v = elm[i]; i < n; v = elm[++i]) 

#define tempo_foreach(t, type_t, v)				\
    tempo__foreach(t, type_t, v,				\
		   _MGENSYM(_i_),				\
		   _MGENSYM(_n_),				\
		   _MGENSYM(_a_))


/**************************
 * Array
 **************************/
#define ARRAY_FIELDS(type_t) uint_t count, capacity; type_t* elements
#define array_t(type_t)      struct { ARRAY_FIELDS(type_t); }
#define array_init(a, c, m)						\
    do {								\
	(a).count     = 0;						\
	(a).capacity  = c;						\
	(a).elements  = 0;						\
	(a).membuffer = m;						\
    } while (0)

#define array_free(a)							\
    do {								\
	membuf_collect((a).membuffer, (a).elements);			\
	(a).count    = 0;						\
	(a).capacity = 0;						\
	(a).elements = 0;						\
    } while (0)

#define array_set(a, i, e)						\
    do {								\
	MEMWISE_ASSERT(i > -1, "array_set", MEMWISE_ERROR_MEMORY);	\
	if (i >= (a).count) (a).count = i + 1;				\
	if ((a).count > (a).capacity)					\
	{								\
	    array_expand(a);						\
	}								\
	(a).elements[i] = e;						\
    } while (0)

#define array_ref(a, i)							\
    (MEMWISE_ASSERT(i > -1, "array_ref", MEMWISE_ERROR_INDEX),		\
     (a).elements + i)

#define array_get(a, i)					\
    (MEMWISE_ASSERT(i > -1 && i < (a).count,		\
		    "array_get", MEMWISE_ERROR_INDEX),			\
     (a).elements[i])

#define array_pop(a)							\
    (MEMWISE_ASSERT((a).count > 0, "array_pop", MEMWISE_ERROR_EMPTY),	\
     (a).elements[--(a).count])

#define array_push(a, e)						\
    do {								\
	uint_t _GENSYM(__i) = (a).count;				\
	array_set(a, VLIB_GENSYM(__i), e);				\
    } while (0)

#define array_ensure(a) if ((a).count < (a).capacity) array_expand(a)

#define array_expand(a)							\
    ((a).elements = vlib__realloc((a).elements,				\
				  ((a).capacity *= 2)			\
				  * sizeof((a).elements[0])))

#define array_erase(a, i)						\
    do {								\
	vlib__assert(i > -1 && i < (a).count,				\
		     "array_erase", MEMWISE_ERROR_INDEX);		\
	int _k_; uint_t _n_ = (a).count;				\
	for (_k_ = i; _k_ < _n_; _k_++)					\
	    (a).elements[_k_] = (a).elements[_k_ + 1];			\
    } while (0)

#define array_remove(a, e)						\
    do {								\
	int _j_; uint_t _n_ = (a).count;				\
	for (_j_ = 0; _j_ < _n_; _j_++)					\
	    if ((a).elements[_j_] == e)	array_erase(_i_);		\
    } while (0)

#define array__foreach(a, type_t, v, i, n, arr)				\
    type_t v; type_t* arr = (a).elements;				\
    int i = 0; uint_t n = (a).count;					\
    for (v = arr[i]; i < n; v = arr[++i]) 

#define array_foreach(a, type_t, v)				\
    array__foreach(a, type_t, v,				\
		   _MGENSYM(__i_),				\
		   _MGENSYM(__n_),				\
		   _MGENSYM(__a_))


/**************************
 * Queue
 **************************/
#define QUEUE_FIELDS(val_t) int head, tail; uint_t capacity; val_t* elements
#define queue_t(val_t) struct { QUEUE_FIELDS(val_t); }

#define queue_init(q, c)						\
    do {								\
	(q).head     = 0;						\
	(q).tail     = 0;						\
	(q).count    = 0;						\
	(q).capacity = c;						\
	(q).elements = vlib__malloc(c * sizeof((s).elements[0]));	\
    } while (0)
	
#define queue_free(q)							\
    do {								\
	vlib__free((q).elements);					\
	(q).head     = 0;						\
	(q).tail     = 0;						\
	(q).count    = 0;						\
	(q).capacity = 0;						\
	(q).elements = 0;						\
    } while (0)

#define queue_peek(q) (q).elements[(q).head]

#define queue_enque(q, e)			\
    do {					\
	(q).elements[(q).tail++] = e;		\
    } while (0)

#define queue_deque(q, e)			\
    do {					\
	e = queue_peek(q);			\
	(q).head++;				\
	(q).count--;				\
    } while (0)


/**************************
 * Stack
 **************************/
#define STACK_FIELDS(val_t) uint_t count, capacity; val_t* elements
#define stack_t(val_t) struct { STACK_FIELDS(val_t); }

#define stack_init(s, c)						\
    do {								\
	(s).count    = 0;						\
	(s).capacity = c;						\
	(s).elements = vlib__malloc(c * sizeof((s).elements[0]));	\
    } while (0)
	
#define stack_free(s)							\
    do {								\
	vlib__free((s).elements);					\
	(s).count    = 0;						\
	(s).capacity = 0;						\
	(s).elements = 0;						\
    } while (0)

#define stack_peek(s) (s).elements[(s).count - 1]

#define stack_push(s, e)						\
    do {								\
	(s).count++;							\
	if ((s).count > (s).capacity) {					\
	    const uint_t n = (s).capacity *= 2;				\
	    const uint_t m = sizeof((s).elements[0]);			\
	    (s).elements   = vlib__realloc((s).elements, n * m);	\
	}								\
	(s).elements[(s).count - 1] = e;				\
    } while (0)
      
#define stack_pop(s) (s).elements[--(s).count]	


/**************************
 * Table
 **************************/
#define TABLE_BUCKET 32
#define TABLE_FIELDS(key_t, val_t)				\
    uint_t hashs[TABLE_BUCKET];		 /* Hash  data */	\
    uint_t count, capacity;		 /* Table size */	\
    int* nexts; key_t* keys; val_t* vals /* Table data */

#define table_t(key_t, val_t) struct { TABLE_FIELDS(key_t, val_t); }

#define table_init(t, c)						\
    do {								\
	(t).count = 0;							\
	(t).capacity = c;						\
	(t).nexts = vlib__malloc(c * sizeof((t).nexts[0]));		\
	(t).keys  = vlib__malloc(c * sizeof((t).keys[0]));		\
	(t).vals  = vlib__malloc(c * sizeof((t).vals[0]));		\
	int _i_;							\
	for (_i_ = 0; _i_ < TABLE_BUCKET; _i_++) {			\
	    (t).hashs[_i_] = -1;					\
	}								\
	for (_i_ = 0; _i_ < c; _i_++) {					\
	    (t).nexts[_i_] = -1;					\
	}								\
    } while (0)

#define table_free(t)							\
    do {								\
	vlib__free((t).nexts);						\
	vlib__free((t).keys);						\
	vlib__free((t).vals);						\
	(t).count    = 0;						\
	(t).capacity = 0;						\
	(t).nexts    = 0;						\
	(t).keys     = 0;						\
	(t).vals     = 0;						\
    } while (0)

#define table_get(t, k, v, hash, cmp)			\
    do {						\
	uint_t _h_ = hash(k) % TABLE_BUCKET;		\
	int    _i_ = (t).hashs[_h_];			\
	while (_i_ > -1) {				\
	    if (cmp(k, (t).keys[_i_])) {		\
		v = (t).vals[_i_];			\
		break;					\
	    }						\
	    _i_ = (t).nexts[_i_];			\
	}						\
    } while (0)

#define table_has(t, k, v, hash, cmp)					\
    do {								\
	v = 0;								\
	uint_t _h_ = hash(k) % TABLE_BUCKET;				\
	int    _i_ = (t).hashs[_h_];					\
	while (_i_ > -1) {						\
	    if (cmp(k, (t).keys[_i_])) {				\
		v = 1;							\
		break;							\
	    }								\
	    _i_ = (t).nexts[_i_];					\
	}								\
    } while (0)

#define table_set(t, k, v, hash, cmp)					\
    do {								\
	uint_t _h_; int _i_, _p_ = -1;					\
        _h_ = hash(k) % TABLE_BUCKET;					\
	_i_ = (t).hashs[_h_];						\
	while (_i_ > -1) {						\
	    if (cmp(k, (t).keys[_i_])) {				\
		goto __set__;						\
	    }								\
	    _p_ = _i_; _i_ = (t).nexts[_p_];				\
	}								\
	_i_ = (t).count;						\
	if (++(t).count > (t).capacity) {				\
	    const uint_t c = (t).capacity *= 2;				\
	    (t).nexts = vlib__realloc((t).nexts, c * sizeof((t).nexts[0])); \
	    (t).keys  = vlib__realloc((t).keys, c * sizeof((t).keys[0])); \
	    (t).vals  = vlib__realloc((t).vals, c * sizeof((t).vals[0])); \
	    int _j_;							\
	    for (_j_ = (t).count; _j_ < c; _j_++) {			\
		(t).nexts[_i_] = -1;					\
	    }								\
	}								\
	if (_p_ < 0) {							\
	    (t).hashs[_h_] = _i_;					\
	} else {							\
	    (t).nexts[_p_] = _i_;					\
	}								\
	(t).nexts[_i_] = -1;						\
	(t).keys[_i_] = k;						\
    __set__:								\
	(t).vals[_i_] = v;						\
    } while (0)

/* END OF C VERSION */
#else
/* BEGIN OF C++ VERSION */

/**
 * Temporary array
 */
template <typename type_t, int capacity>
struct tempo_t
{
    int    count;
    type_t elements[capacity];

    inline T& operator[](int index)
    {
	return elements[index];
    }

    inline const T& operator[](int index) const
    {
	return elements[index];
    }
};

/**
 * Resizable array
 */
template <typename type_t>
struct array_t
{
    int       count;
    int       capacity;
    type_t*   elements;
    membuf_t* membuffer;

    inline T& operator[](int index)
    {
	return elements[index];
    }

    inline const T& oeprator[](int index) const
    {
	return elements[index];
    }
};

/**
 * Stack
 */
template <typename type_t>
struct stack_t
{
    array_t<type_t> buffer;
};

/**
 * Queue
 */
template <typename type_t>
struct queue_t
{
    int       head;
    int       tail;
    int       capacity;
    type_t*   elements;
    membuf_t* membuffer;
};


/* END OF C++ VERSIONS */
#endif

#undef __MCONCAT
#undef _MCONCAT
#undef _MGENSYM

#endif /* __CONTAINERS_H__ */
