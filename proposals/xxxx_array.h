/*
 * @url: http://cr.yp.to/lib/array.html
 * @taglist: ADT
 * @lang: c89
 *
 * A linear and growable region of memory.
 *
 * TODO(uucidl): figure out whether to use int64, size_t or?
 * size_t is problematic in that it's not signed and therefore potentially hard
 * to use. DJB uses int64 which we don't have a definition of in C89
 *
 * NOTE(uucidl): this interface does not support custom allocators.
 */

/*
 * The array library interface
 * ---------------------------
 *
 * An allocated array variable keeps track of:
 *
 * - a (nonzero) pointer to a dynamically allocated region of memory;
 * - the number of bytes allocated (always positive); and
 * - the number of bytes initialized (between 0 and the number of bytes
 * allocated).
 *
 * There are two other possibilities for the state of an array variable:
 * unallocated and failed. In both cases, there is no dynamically allocated
 * region of memory.
 *
 * A new array variable must be initialized to all-0, meaning unallocated,
 * before it is given to any of the array functions. It must be returned to the
 * unallocated (or failed) state, with @see{array_reset} or @see{array_fail},
 * before it is destroyed. These rules prevent all memory leaks.
 *
 * @code{@lang{c}
 * #include "array.h"
 *
 * Array x = {0,};
 * }
 *
 * At this point it is unallocated. The array library provides various
 * allocation and inspection functions.
 */

struct ArrayHeader;
typedef struct ArrayHeader Array;

/*
 * Expansion and inspection
 * ------------------------
 */

/* array_allocate makes sure that enough bytes are allocated in x for at least
 * pos+1 objects of type t. (The size of t must be positive; otherwise the
* effects
* are undefined.) If not enough bytes are allocated (or x is unallocated),
* array_allocate allocates more bytes, moving the dynamically allocated region
* if
* necessary. array_allocate often allocates somewhat more bytes than necessary,
* to
* save time later.
*
* array_allocate then makes sure that the number of bytes initialized covers at
* least those pos+1 objects. If not enough bytes are initialized, array_allocate
* initializes more bytes (setting them to 0), up to exactly the end of the
* pos+1st
* object.
*
* array_allocate then returns a pointer to the pos+1st object; i.e., object
* number
* pos, with objects numbered starting at 0. This pointer can be used to change
* or
* inspect the object. The pointer can continue to be used through subsequent
* calls
* to array_get, array_start, array_length, and array_bytes, but it must not be
* used after any other operations on this array.
*
* If something goes wrong, array_allocate returns 0, setting errno
* appropriately,
* without touching x. In particular, array_allocate returns 0 if
*
* - x has failed, or
* - pos is SIZE_MAX, or
* - not enough memory is available.
*
* array_allocate does not change x to have failed; if you want to do that, use
* array_fail.
*/
void *array_allocate(Array *x, size_t element_size, size_t pos);

/*
array_get is similar to array_allocate, but it does not allocate any extra
bytes, and it does not initialize any extra bytes. It returns 0 if x is
unallocated, for example, or if fewer than (pos+1)*sizeof(t) bytes are
initialized.
*/
void *array_get(Array *x, size_t element_size, size_t pos);

/* array_start is the same as array_get with pos equal to 0. */
void *array_start(Array *x);

/*
 * array_length returns the number of initialized bytes in x, divided by the
 * size of t. In other words,
 * array_get will succeed for positions 0 through array_length-1; it will fail
 * for position array_length.
 *
 * If x is unallocated, array_length return 0.
 * If x has failed, array_length and array_bytes return SIZE_MAX.
 */
size_t array_length(Array *x, size_t element_size);

/* array_bytes returns the number of initialized bytes in x, without regard to
 * t.
 * - If x is unallocated, array_bytes return 0.
 * - If x has failed, array_length and array_bytes return SIZE_MAX.
 */
size_t array_bytes(Array *x);

/*
 * Truncation and deallocation
 * ---------------------------
 */

/*
 * array_truncate reduces the number of initialized bytes in x to exactly
 * @code{len*sizeof(t)}. If the number of initialized bytes was already this
 * small (or smaller), array_truncate has no effect. If len is negative,
 * array_truncate has no effect. If x is unallocated, array_truncate has no
 * effect. If x has failed, array_truncate has no effect.
 *
 * array_truncate does not change the allocation in x. If you want to free the
 * memory used by x, use array_reset.
 */
void array_truncate(Array *x, size_t element_size, size_t len);

/* array_trunc is the same as array_truncate with len equal to 0. */
void array_trunc(Array *x);

/*
 * If x is allocated, array_reset frees the region that x points to, and
 * switches x to being unallocated.
 *
 * If x has failed, array_reset simply switches x to being unallocated.
 *
 * If x is unallocated, array_reset has no effect.
 */
void array_reset(Array *x);

/*
 * If x is allocated, array_fail frees the region that x points to, and switches
 * x to have failed.
 *
 * If x is unallocated, array_fail simply switches x to have failed.
 *
 * If x has already failed, array_fail has no effect.
 */
void array_fail(Array *x);

/*
 * Comparison
 * ----------
 */

/*
 * array_equal returns nonzero if x and y have the same contents: i.e.,
 *
 * - x and y are both unallocated; or
 * - x is unallocated, y is allocated, and y has no initialized bytes; or
 * - x is allocated, y is unallocated, and x has no initialized bytes; or
 * - x and y are both allocated and have the same sequence of initialized bytes.
 *
 * Otherwise it returns 0.
 */
int array_equal(Array *x, Array *y);

/*
 * Concatenation
 * -------------
 */

/*
 * array_cat appends y to x; i.e., it changes x, allocating more space if
 * necessary, so that the initialized bytes in x are the previously initialized
 * bytes in x followed by a copy of the initialized bytes in y.
 *
 * - If x has failed, array_cat has no effect.
 * - If y has failed, array_cat switches x to have failed.
 * - If not enough memory is available, array_cat switches x to have failed.
 */
void array_cat(Array *x, Array *y);

/* array_catb appends the bytes y[0], y[1], ..., y[len-1] to the array x. It
 * handles failure in the same way as array_cat.
*/
void array_catb(Array *x, char const *y, size_t len);

/* array_cats appends the contents of the 0-terminated string y, not including
 * the terminating 0 byte, to the array x. For failure handling @see{array_cat}.
 */
void array_cats(Array *x, char const *y);

/* array_cats0 appends the contents of y, including the terminating 0 byte, to
 * x. For failure handling @see{array_cat}. */
void array_cats0(Array *x, char const *y);

/* array_cat0 appends a 0 byte to x. For failure handling @see{array_cat}.*/
void array_cat0(Array *x);

/* array_cate is like array_cat, but uses only byte positions pos through stop-1
 * in y. It fails if pos is negative, or if stop is smaller than pos, or if the
 * number of initialized bytes in y is smaller than stop. */
void array_cate(Array *x, Array *y, size_t pos, size_t stop);
