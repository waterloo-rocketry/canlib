#ifndef SAFE_RING_BUFFER_H_
#define SAFE_RING_BUFFER_H_

/*
 *
 */

#include <stdbool.h>
#include <stddef.h>

/*
 * Context variable for the module. This allows you to initialize multiple
 * ring buffers in the same application code, and keeping track of different
 * context variables for each ring buffer.
 */
typedef struct {
    void *memory_pool;
    size_t element_size;
    size_t max_elements;
    size_t rd_idx;
    size_t wr_idx;
} srb_ctx_t;

/*
 * Initialize ring buffer. ctx is the ring buffer context to be initialized,
 * pool is a memory pool for the buffer to use. pool_size is the number of
 * bytes in the pool. Make sure that no other code outside this module edits
 * any of the bytes in pool. element_size is the size, in bytes, of the type
 * of element that you'd like to store in this buffer.
 *
 * Note that each ring buffer can only hold one type of element, it is
 * impossible to store elements of different size in the same ring buffer.
 */
void srb_init(srb_ctx_t *ctx,
              void *pool,
              size_t pool_size,
              size_t element_size);

/*
 * Push an element into the ring buffer. If there is not enough space left in
 * the buffer, this function will return false. Otherwise, it will copy the
 * element in (using memcpy, coyping element_size bytes) and return true
 */
bool srb_push(srb_ctx_t *ctx,
              const void *element);

/*
 * Returns true if there is no space left in the ring buffer. False otherwise
 */
bool srb_is_full(const srb_ctx_t *ctx);

/*
 * Returns true if there are no elements in the ring buffer. False otherwise
 */
bool srb_is_empty(const srb_ctx_t *ctx);

/*
 * Gets the oldest buffered element in the ring buffer and copies it into
 * element. Then that element is removed from the ring buffer. Returns true
 * if it was successfully able to fetch an element, false otherwise
 */
bool srb_pop(srb_ctx_t *ctx,
             void *element);

/*
 * Gets the oldest buffered element in the ring buffer and copies it into
 * element. This element is not removed from the buffer. Returns true if
 * it was successfully able to fetch an element, false otherwise
 */
bool srb_peek(const srb_ctx_t *ctx,
              void *element);

#endif
