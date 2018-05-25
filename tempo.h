#ifndef __TEMPO_H__
#define __TEMPO_H__


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
#define membuf_extract(buf, size) (buf)->extract((buf)->data, size, 4)
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

#else
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
void tempo_insert(tempo_t<type_t, capacity>& tempo,
		  int index,
		  const type_t& value)
{
    memwise__tempo_insert(tempo, index, value);
}
#endif /* MEMWISE_C_VERSIONS */
#endif /* __TEMPO_H__ */
