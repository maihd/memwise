#ifndef __MEMWISE_H__
#define __MEMWISE_H__


#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define __MCONCAT(x, y) x ## y
#define _MCONCAT(x, y) __MCONCAT(x, y)
#define _MGENSYM(x)    _MCONCAT(x, __LINE__)

#if defined(__cplusplus)
#define _MPTR_ASSIGN(a, b) ((a) = (decltype(a))(b))
#else
#define _MPTR_ASSIGN(a, b) ((a) = (b))
#endif

#define MEMWISE_ERROR_INDEX  0
#define MEMWISE_ERROR_EMPTY  1
#define MEMWISE_ERROR_MEMORY 2

#define MEMWISE_ERROR(func, code)  func ": " MEMWISE_ERROR_STRING(code)
#define MEMWISE_ERROR_STRING(code) _MCONCAT(MEMWISE_ERROR_STRING, code)
#define MEMWISE_ERROR_STRING0      "Index out of range"
#define MEMWISE_ERROR_STRING1      "No elements"
#define MEMWISE_ERROR_STRING2      "Out of memory"

#define MEMWISE_ASSERT(exp, f, e)  assert(exp && MEMWISE_ERROR(f, e))

/* ------------------------------------------------- */
/* ----------------- MEMORY BUFFER ----------------- */
/* ------------------------------------------------- */

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

#define membuf_collect(buf, ptr)  (buf)->collect((buf)->data, ptr)
#define membuf_extract(buf, size) (buf)->extract((buf)->data, size, 1)
#endif

/* --------------------------------------------------- */
/* ------------------- CONTAINERS -------------------- */
/* --------------------------------------------------- */

#define MEMWISE_DEFCMP(a, b) ((a) == (b))

/**************************
 * Temporary array
 **************************/
#define memwise__tempo_t(type_t, c) \
    int    count;		    \
    type_t elements[c]	    

#define memwise__tempo_init(t)      \
    do { (t).count = 0; } while (0)

#define memwise__tempo_capacity(t)			\
    (sizeof((t).elements) / sizeof((t).elements[0]))

#define memwise__tempo_set(t, i, e)					\
    do {								\
	if (i <= (t).count) {						\
	    (t).count = i + 1;						\
	    MEMWISE_ASSERT((t).count <= memwise__tempo_capacity(t),	\
			   "tempo_set", MEMWISE_ERROR_MEMORY);		\
	}								\
	(t).elements[i] = e;						\
    } while (0)

#define memwise__tempo_get(t, i)					\
    (MEMWISE_ASSERT(i > -1 && i < (t).count,				\
		    "tempo_get",					\
		    MEMWISE_ERROR_INDEX),				\
     (t).elements[i])

#define memwise__tempo_ref(t, i)					\
    (MEMWISE_ASSERT(i > -1 && i < memwise__tempo_capacity(t),		\
		    "tempo_ref",					\
		    MEMWISE_ERROR_INDEX),				\
     (t).elements + i)

#define memwise__tempo_pop(t)						\
    (MEMWISE_ASSERT((t).count > 0,					\
		    "tempo_pop",					\
		    MEMWISE_ERROR_EMPTY),				\
     (t).elements[--(t).count])

#define memwise__tempo_push(t, e)					\
    do {								\
	int _i_ = (t).count;						\
	tempo_set(t, _i_, e);						\
    } while (0)

#define memwise__tempo_find(t, e, i)			\
    memwise__tempo_find_cmp(t, e, i, MEMWISE_DEFCMP)

#define memwise__tempo_find_cmp(t, e, i, cmp)		\
    do {						\
	int _j_; int _m_ = (t).count;			\
	for (_j_ = 0; _j_ < _m_; _j_++)	{		\
	    if (cmp(e, (t).elements[_j_])) {		\
	        (i) = _j_; break;			\
	    }						\
	}						\
    } while (0)

#define memwise__tempo_rfind(t, e, i)			\
    memwise__tempo_rfind_cmp(t, e, i, MEMWISE_DEFCMP)

#define memwise__tempo_rfind_cmp(t, e, i, cmp)		\
    do {						\
	int _j_; int _m_ = (t).count;			\
	for (_j_ = _m_ - 1; _j_ > 0; _j_--)	{	\
	    if (cmp(e, (t).elements[_j_])) {		\
	        (i) = _j_; break;			\
	    }						\
	}						\
    } while (0)

#define memwise__tempo_erase(t, i)					\
    do {								\
	MEMWISE_ASSERT(!(i < 0 || i >= (t).count),			\
		       "tempo_erase", MEMWISE_ERROR_INDEX);		\
	int _i_; int _n_ = (t).count;					\
	for (_i_ = i; _i_ < _n_; _i_++) {				\
	    (t).elements[_i_] = (t).elements[_i_ + 1];			\
	}								\
    } while (0)

#define memwise__tempo_remove(t, e)			\
    memwise__tempo_remove_cmp(t, e, MEMWISE_DEFCMP)

#define memwise__tempo_remove_cmp(t, e, cmp)		\
    do {						\
	int _k_;					\
	memwise__tempo_find_cmp(t, e, _k_, cmp);	\
	memwise__tempo_erase(t, _k_);			\
    } while (0)


#define memwise__tempo_insert(t, i, e)					\
    do {								\
        MEMWISE_ASSERT((t).count < memwise__tempo_capacity(t),		\
		       "tempo_insert", MEMWISE_ERROR_MEMORY);		\
	int _i_; int _n_ = (t).count++, _e_ = i;			\
	for (_i_ = _n_; _i_ < _e_; _i_--)				\
	    (t).elements[_i_] = (t).elements[_i_ - 1];			\
	(t).elements[_e_] = e;						\
    } while (0)


#define memwise__tempo__foreach(t, type_t, v, i, n, elm)		\
    type_t v; type_t* elm = (t).elements;				\
    int i = 0; int n = (t).count;					\
    for (v = elm[i]; i < n; v = elm[++i]) 

#define tempo_foreach(t, type_t, v)				\
    memwise__tempo__foreach(t, type_t, v,			\
			    _MGENSYM(_i_),			\
			    _MGENSYM(_n_),			\
			    _MGENSYM(_a_))


/**************************
 * Dynamic size array
 **************************/

/**
 * Dynamic array container structure fields
 */
#define memwise__array_t(type_t)			\
    int       count;					\
    int       capacity;					\
    type_t*   elements;					\
    membuf_t* membuffer

/**
 * Initialize array
 *
 * @param a - The array
 * @param m - The memory buffer for elements space
 */
#define memwise__array_init(a, m)					\
    do {								\
	(a).count     = 0;						\
	(a).capacity  = 0;						\
	(a).elements  = 0;						\
	(a).membuffer = m;						\
    } while (0)

/**
 * Return all memory usage of the array to its own memory buffer
 *
 * @param a - The array
 */
#define memwise__array_free(a)						\
    do {								\
	membuf_collect((a).membuffer, (a).elements);			\
									\
	(a).count     = 0;						\
	(a).capacity  = 0;						\
	(a).elements  = 0;						\
    } while (0)

/**
 * Set array element's value at given index
 *
 * @param a - The array
 * @param i - The index
 * @param v - The value
 */
#define memwise__array_set(a, i, v)					\
    do {								\
	int _i_ = i;							\
									\
	MEMWISE_ASSERT(_i_ > -1, "array_set", MEMWISE_ERROR_INDEX);	\
									\
	if (_i_ >= (a).count) (a).count = _i_ + 1;			\
        memwise__array_ensure(a, (a).count);				\
	(a).elements[_i_] = v;						\
    } while (0)

/**
 * Get the pointer reference to element's slot at given index
 *
 * @param a - The array
 * @param i - The index
 * @return pointer to the element
 */
#define memwise__array_ref(a, i)					\
    (MEMWISE_ASSERT((i) > -1, "array_ref", MEMWISE_ERROR_INDEX),	\
     (a).elements + i)

/**
 * Get the element's value at given index
 *
 * @param a - The array
 * @param i - The index
 * @return The element's value
 */
#define memwise__array_get(a, i)					\
    (MEMWISE_ASSERT((i) > -1 && (i) < (a).count,			\
		    "array_get",					\
		    MEMWISE_ERROR_INDEX),				\
     (a).elements[i])

/**
 * Remove the last element and return its value, 
 * abort() when array is empty
 *
 * @param a - The array
 * @return The last element's value
 */
#define memwise__array_pop(a)						\
    (MEMWISE_ASSERT((a).count > 0, "array_pop", MEMWISE_ERROR_EMPTY),	\
     (a).elements[--(a).count])

/**
 * Create new element at the last position with given value
 * 
 * @param a - The array
 * @param v - The value
 */
#define memwise__array_push(a, v) memwise__array_set(a, (a).count, v)

/**
 * Ensure the array that has enough space to contain 'c' elements
 *
 * @param a - The array
 * @param c - The expected capacity
 */
#define memwise__array_ensure(a, c)				\
    do {							\
	int _c_ = (int)(c);					\
								\
	if (_c_ > (a).capacity) memwise__array_expand(a, _c_);	\
    } while (0)

/**
 * Expand the array that make enough space to contain 'c' elements
 * 
 * @param a - The array
 * @param c - Minimal capacity
 */
#define memwise__array_expand(a, c)					\
      do {								\
	  /* New container capacity */					\
	  int _c1_ = (a).capacity; /* Add number to avoid with _c_ */	\
	  if (_c1_ <= 0)   _c1_ = 64;					\
	  while (_c1_ < c) _c1_ *= 2;					\
									\
	  /* Elements container sizes and buffer */			\
	  int   size    = ((a).capacity * sizeof((a).elements[0]));	\
	  int   new_size     = (_c1_ * sizeof((a).elements[0]));	\
	  void* new_elements = membuf_extract((a).membuffer, new_size); \
									\
	  /* Moving data and return memory to membuffer */		\
	  memcpy(new_elements, (a).elements, size);			\
	  membuf_collect((a).membuffer, (a).elements);			\
									\
	  /* Storing new variables */					\
	  (a).capacity = _c1_;						\
	  _MPTR_ASSIGN((a).elements, new_elements);			\
      } while (0)

/**
 * Find the element has the given value, with specified compare expression
 * 
 * @param a   - The array
 * @param v   - The value
 * @param out - The variable that receive output
 * @param cmp - Compare expression
 */
#define memwise__array_find_cmp(a, v, out, cmp)	\
    do {					\
	out = -1;				\
						\
	int _i2_; int _n2_ = (a).count;		\
	for (_i2_ = 0; _i2_ < _n2_; _i2_++)	\
	{					\
	    if (cmp((a).elements[_i2_], v))	\
	    {					\
		out = _i2_;			\
		break;				\
	    }					\
	}					\
    } while (0)

/**
 * Find the element has the given value in reverse order
 * 
 * @param a   - The array
 * @param v   - The value
 * @param out - The variable that receive output
 */
#define memwise__array_find(a, v, out) \
    memwise__array_find_cmp(a, v, out, MEMWISE_DEFCMP)

/**
 * Find the element has the given value in reverse order,
 * with specified compare expression
 * 
 * @param a   - The array
 * @param v   - The value
 * @param out - The variable that receive output
 * @param cmp - Compare expression
 */
#define memwise__array_rfind_cmp(a, v, out, cmp)	\
    do {						\
	out = -1;					\
							\
	int _i2_;					\
	for (_i2_ = (a).count; _i2_ > -1; _i2_--)	\
	{						\
	    if (cmp((a).elements[_i2_], v))		\
	    {						\
		out = _i2_;				\
		break;					\
	    }						\
	}						\
    } while (0)

/**
 * Find the element has the given value in reverse order
 * 
 * @param a   - The array
 * @param v   - The value
 * @param out - The variable that receive output
 */
#define memwise__array_rfind(a, v, out)		\
    memwise__array_rfind_cmp(a, v, out, MEMWISE_DEFCMP)

/**
 * Erase the element at given the position
 *
 * @param a - The array
 * @param i - The index of the element position
 */
#define memwise__array_erase(a, i)					\
    do {								\
	int _i1_ = i;							\
									\
	MEMWISE_ASSERT(_i1_ > -1 && _i1_ < (a).count,			\
		       "array_erase", MEMWISE_ERROR_INDEX);		\
									\
	void* _dst_ = &(a).elements[_i1_];				\
	void* _src_ = &(a).elements[_i1_ + 1];				\
	int   _sze_ = ((a).count - _i1_ - 1) * sizeof((a).elements[0]);	\
	memcpy(_dst_, _src_, _sze_);					\
	(a).count -= 1;							\
    } while (0)

/**
 * Remove the element that fit the requirement
 * 
 * @param a   - The array
 * @param v   - The value
 * @param cmp - Compare expression (macros or function)
 */
#define memwise__array_remove_cmp(a, v, cmp)				\
    do {								\
	int _j_; int _n_ = (a).count;					\
	for (_j_ = 0; _j_ < _n_; _j_++)					\
	{								\
	    if (cmp((a).elements[_j_], v))				\
	    {								\
		memwise__array_erase(a, _j_);				\
		break;							\
	    }								\
	}								\
    } while (0)

/**
 * Remove the element that has given value
 *
 * @param a - The array
 * @param v - The value
 */
#define memwise__array_remove(a, v)			\
    memwise__array_remove_cmp(a, v, MEMWISE_DEFCMP)

#define memwise__array__foreach(a, type_t, v, i, n, arr)		\
    type_t v; type_t* arr = (a).elements;				\
    int i = 0; int n = (a).count;					\
    if (n > 0) v = arr[0];						\
    for (/* empty */; i < n; v = arr[++i]) 

/**
 * Loop through the array 
 *
 * @param a      - The array
 * @param type_t - The type of value
 * @param i      - The index variable name
 * @param v      - The value variable name
 */
#define array_foreach(a, type_t, i, v)				\
    memwise__array__foreach(a, type_t, v, i,			\
			    _MGENSYM(__n_),			\
			    _MGENSYM(__a_))


/**************************
 * Queue
 **************************/
#define memwise__queue_t(type_t)					\
    int head, tail, capacity; type_t* elements; membuf_t* membuffer

#define memwise__queue_init(q, m)					\
    do {								\
	(q).head      = 0;						\
	(q).tail      = 0;						\
	(q).capacity  = 0;						\
	(q).elements  = 0;						\
	(q).membuffer = m;						\
    } while (0)
	
#define memwise__queue_free(q)						\
    do {								\
	membuf_collect((q).membuffer, (q).elements);			\
									\
	(q).head     = 0;						\
	(q).tail     = 0;						\
	(q).capacity = 0;						\
	(q).elements = 0;						\
    } while (0)

#define memwise__queue_peek(q)  (q).elements[(q).head]
#define memwise__queue_count(q) ((q).tail - (q).head)
#define memwise__queue_deque(q)						\
    (MEMWISE_ASSERT(memwise__queue_count(q) > 0,			\
		    "queue_deque: ", MEMWISE_ERROR_EMPTY),		\
     (q).elements[(q).head++])

#define memwise__queue_enque(q, e)					\
    do {								\
	if ((q).tail >= (q).capacity) {					\
	    if ((q).head > 0) {						\
		memcpy((q).elements,					\
		       (q).elements + (q).head,				\
		       (q).tail - (q).head);				\
	    } else {							\
		int   new_capacity;					\
		if ((q).capacity <= 0) new_capacity = 64;		\
		else                   new_capacity = (q).capacity * 2;	\
									\
		int   element_size = sizeof ((q).elements[0]);	        \
		void* new_elements =					\
		    membuf_extract((q).membuffer,			\
				   new_capacity * element_size);	\
									\
		memset(new_elements,					\
		       (q).elements,					\
		       (q).capacity * element_size);			\
		membuf_collect((q).membuffer, (q).elements);		\
									\
		(q).capacity = new_capacity;				\
		_MPTR_ASSIGN((q).elements, new_elements);		\
	    }								\
	}								\
	(q).elements[(q).tail++] = e;					\
    } while (0)


/**************************
 * Table
 **************************/
#define MEMWISE__TABLE_BUCKET 32
#define MEMWISE__TABLE_FIELDS(key_t, val_t)			\
    int hashs[MEMWISE__TABLE_BUCKET]; /* Hash  data */	        \
    int count, capacity;		 /* Table size */	\
    int* nexts; key_t* keys; val_t* vals /* Table data */

#define memwise__table_init(t, c)					\
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

#define memwise__table_free(t)						\
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

#define memwise__table_get(t, k, v, hash, cmp)		\
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

#define memwise__table_has(t, k, v, hash, cmp)				\
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

#define memwise__table_set(t, k, v, hash, cmp)				\
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


#if MEMWISE_C_VERSION || !defined(__cplusplus)
/* BEGIN OF C VERSIONS */


#define MEMWISE_DEFCMP(a, b) ((a) == (b))

/**************************
 * Temporary array
 **************************/
#define tempo_t(type_t, c) struct { memwise__tempo_(type_t, c); }     

#define tempo_init(t) memwise__tempo_init(t)

#define tempo_capacity(t) memwise__tempo_capacity(t)

#define tempo_set(t, i, e) memwise__tempo_set(t, i, e)

#define tempo_get(t, i) memwise__tempo_get(t, i)

#define tempo_ref(t, i) memwise__tempo_ref(t, i)

#define tempo_pop(t) memwise__tempo_pop(t)

#define tempo_push(t, e) memwise__tempo_push(t, e)

#define tempo_find(t, e, i) memwise__tempo_find(t, e, i)

#define tempo_find_cmp(t, e, i, cmp) memwise__tempo_find_cmp(t, e, i, cmp)

#define tempo_erase(t, i) memwise__tempo_erase(t, i) 

#define tempo_remove(t, e) memwise__tempo_remove(t, e)

#define tempo_remove_cmp(t, e, cmp) memwise__tempo_remove_cmd(t, e, cmp)

#define tempo_insert(t, i, e) memwise__tempo_insert(t, i, e)


/**************************
 * Dynamic size array
 **************************/

/**
 * Dynamic array container structure fields
 */
#define array_t(type_t) struct { memwise__array_t(type_t); }

/**
 * Initialize array
 *
 * @param a - The array
 * @param m - The memory buffer for elements space
 */
#define array_init(a, m) memwise__array_init(a, m)

/**
 * Return all memory usage of the array to its own memory buffer
 *
 * @param a - The array
 */
#define array_free(a) memwise__array_free(a)

/**
 * Set array element's value at given index
 *
 * @param a - The array
 * @param i - The index
 * @param v - The value
 */
#define array_set(a, i, v) memwise__array_set(a, i, v)

/**
 * Get the pointer reference to element's slot at given index
 *
 * @param a - The array
 * @param i - The index
 * @return pointer to the element
 */
#define array_ref(a, i) memwise__array_ref(a, i)

/**
 * Get the element's value at given index
 *
 * @param a - The array
 * @param i - The index
 * @return The element's value
 */
#define array_get(a, i) memwise__array_get(a, i)

/**
 * Remove the last element and return its value, 
 * abort() when array is empty
 *
 * @param a - The array
 * @return The last element's value
 */
#define array_pop(a) memwise__array_pop(a)

/**
 * Create new element at the last position with given value
 * 
 * @param a - The array
 * @param v - The value
 */
#define array_push(a, v) memwise__array_push(a, v)

/**
 * Ensure the array that has enough space to contain 'c' elements
 *
 * @param a - The array
 * @param c - The expected capacity
 */
#define array_ensure(a, c) memwise__array_ensure(a, c)

/**
 * Expand the array that make enough space to contain 'c' elements
 * 
 * @param a - The array
 * @param c - Minimal capacity
 */
#define array_expand(a, c) mmewise__array_expand(a, c)

/**
 * Find the element has the given value, with specified compare expression
 * 
 * @param a   - The array
 * @param v   - The value
 * @param out - The variable that receive output
 * @param cmp - Compare expression
 */
#define array_find_cmp(a, v, out, cmp)		\
    memwise__array_find_cmp(a, v, out, cmp)

/**
 * Find the element has the given value in reverse order
 * 
 * @param a   - The array
 * @param v   - The value
 * @param out - The variable that receive output
 */
#define array_find(a, v, out) memwise__array_find(a, v, out)

/**
 * Find the element has the given value in reverse order,
 * with specified compare expression
 * 
 * @param a   - The array
 * @param v   - The value
 * @param out - The variable that receive output
 * @param cmp - Compare expression
 */
#define array_rfind_cmp(a, v, out, cmp)		\
    memwise__array_rfind_cmp(a, v, out, cmp)

/**
 * Find the element has the given value in reverse order
 * 
 * @param a   - The array
 * @param v   - The value
 * @param out - The variable that receive output
 */
#define array_rfind(a, v, out) memwise__array_rfind(a, v, out)

/**
 * Erase the element at given the position
 *
 * @param a - The array
 * @param i - The index of the element position
 */
#define array_erase(a, i) memwise__array_erase(a, i)

/**
 * Remove the element that fit the requirement
 * 
 * @param a   - The array
 * @param v   - The value
 * @param cmp - Compare expression (macros or function)
 */
#define array_remove_cmp(a, v, cmp) memwise__array_remove_cmp(a, v, cmp)

/**
 * Remove the element that has given value
 *
 * @param a - The array
 * @param v - The value
 */
#define array_remove(a, v) memwise__array_remove(a, v)

#define queue_t(type_t)   struct { memwise__queue_t(type_t); }
#define queue_init(q, m)  memwise__queue_init(q, m)
#define queue_free(q)     memwise__queue_free(q)
#define queue_peek(q)     memwise__queue_peek(q)
#define queue_count(q)    memwise__queue_count(q)
#define queue_deque(q)    memwise__queue_deque(q)
#define queue_enque(q, v) memwise__queue_enque(q, v)

/* END OF C VERSION */
#else
/* BEGIN OF C++ VERSION */

/**
 * Temporary array
 */
template <typename type_t, int capacity>
struct tempo_t
{
    memwise__tempo_t(type_t, capacity);

    inline type_t& operator[](int index)
    {
	return elements[index];
    }

    inline const type_t& operator[](int index) const
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
    memwise__array_t(type_t);

    inline type_t& operator[](int index)
    {
	return elements[index];
    }

    inline const type_t& operator[](int index) const
    {
	return elements[index];
    }
};

template <typename type_t>
struct queue_t
{
    memwise__queue_t(type_t);
};

template <typename type_t, int capacity>
inline void tempo_init(tempo_t<type_t, capacity>& tempo)
{
    memwise__tempo_init(tempo);
}

template <typename type_t, int capacity>
void tempo_set(tempo_t<type_t, capacity>& tempo, int index, const type_t& value)
{
    memwise__tempo_set(tempo, index, value);
}

template <typename type_t, int capacity>
inline const type_t& tempo_get(const tempo_t<type_t, capacity>& tempo, int index)
{
    return memwise__tempo_get(tempo, index);
}

template <typename type_t, int capacity>
inline type_t* tempo_ref(tempo_t<type_t, capacity>& tempo, int index)
{
    return memwise__tempo_ref(tempo, index);
}

template <typename type_t, int capacity>
inline const type_t* tempo_ref(const tempo_t<type_t, capacity>& tempo, int index)
{
    return memwise__tempo_ref(tempo, index);
}

template <typename type_t, int capacity>
inline const type_t& tempo_pop(tempo_t<type_t, capacity>& tempo)
{
    return memwise__tempo_pop(tempo);
}

template <typename type_t, int capacity>
inline void tempo_push(tempo_t<type_t, capacity>& tempo, const type_t& value)
{
    tempo_set(tempo, value);
}

template <typename type_t, int capacity>
int tempo_find(const tempo_t<type_t, capacity>& tempo, const type_t& value)
{
    int res;
    memwise__tempo_find(tempo, value, res);
    return res;
}

template <typename type_t, int capacity>
int tempo_rfind(const tempo_t<type_t, capacity>& tempo, const type_t& value)
{
    int res;
    memwise__tempo_rfind(tempo, value, res);
    return res;
}

template <typename type_t, int capacity>
void tempo_erase(tempo_t<type_t, capacity>& tempo, int index)
{
    memwise__tempo_erase(tempo, index);
}

template <typename type_t, int capacity>
void tempo_remove(tempo_t<type_t, capacity>& tempo, const type_t& value)
{
    memwise__tempo_remove(tempo, value);
}


template <typename type_t, int capacity>
void tempo_insert(tempo_t<type_t, capacity>& tempo, int index, const type_t& value)
{
    memwise__tempo_insert(tempo, index, value);
}

template <typename type_t>
inline void array_init(array_t<type_t>& array, membuf_t* membuffer)
{
    memwise__array_init(array, membuffer);
}

template <typename type_t>
inline void array_free(array_t<type_t>& array)
{
    memwise__array_free(array);
}

template <typename type_t>
void array_set(array_t<type_t>& array, int index, const type_t& value)
{
    memwise__array_set(array, index, value);
}

template <typename type_t>
inline const type_t& array_get(const array_t<type_t>& array, int index)
{
    return memwise__array_get(array, index);
}

template <typename type_t>
inline type_t* array_ref(array_t<type_t>& array, int index)
{
    return memwise__array_ref(array, index);
}

template <typename type_t>
inline const type_t* array_ref(const array_t<type_t>& array, int index)
{
    return memwise__array_ref(array, index);
}

template <typename type_t>
inline const type_t& array_pop(array_t<type_t>& array)
{
    return memwise__array_pop(array);
}

template <typename type_t>
inline void array_push(array_t<type_t>& array, const type_t& value)
{
    array_set(array, array.count, value);
}

template <typename type_t>
void array_expand(array_t<type_t>& array, int capacity)
{
    memwise__array_expand(array, capacity);
}

template <typename type_t>
inline bool array_ensure(array_t<type_t>& array, int capacity)
{
    if (capacity > array.capacity)
    {
	array_expand(array, capacity);
    }
    return true;
}

template <typename type_t>
inline bool array_ensure(const array_t<type_t>& array, int capacity)
{
    return (capacity <= array.capacity);
}

template <typename type_t>
int array_find(const array_t<type_t>& array, const type_t& value)
{
    int res;
    memwise__array_find(array, value, res);
    return res;
}

template <typename type_t>
int array_rfind(const array_t<type_t>& array, const type_t& value)
{
    int res;
    memwise__array_rfind(array, value, res);
    return res;
}

template <typename type_t>
void array_erase(array_t<type_t>& array, int index)
{
    memwise__array_erase(array, index);
}

template <typename type_t>
void array_remove(array_t<type_t>& array, const type_t& value)
{
    memwise__array_remove(array, value);
}

template <typename type_t>
void array_insert(array_t<type_t>& array, int index, const type_t& value)
{
    memwise__array_insert(array, index, value);
}

template <typename type_t>
inline void queue_init(queue_t<type_t>& queue, membuf_t* membuffer)
{
    memwise__queue_init(queue, membuffer);
}

template <typename type_t>
inline void queue_free(queue_t<type_t>& queue)
{
    memwise__queue_free(queue);
}

template <typename type_t>
inline type_t& queue_peek(queue_t<type_t>& queue)
{
    return memwise__queue_peek(queue);
}

template <typename type_t>
inline const type_t& queue_peek(const queue_t<type_t>& queue)
{
    return memwise__queue_peek(queue);
}

template <typename type_t>
inline int queue_count(queue_t<type_t>& queue)
{
    return memwise__queue_count(queue);
}

template <typename type_t>
inline const type_t& queue_deque(queue_t<type_t>& queue)
{
    return memwise__queue_deque(queue);
}

template <typename type_t>
void queue_enque(queue_t<type_t>& queue, const type_t& value)
{
    memwise__queue_enque(queue, value);
}

/* END OF C++ VERSIONS */
#endif

#endif /* __CONTAINERS_H__ */
