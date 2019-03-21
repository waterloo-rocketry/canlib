#include "safe_ring_buffer.h"
#include <string.h>
#include <stdint.h>

#define   VALID_FLAG 0xff
#define INVALID_FLAG 0x00

static size_t get_offset_bytes(const srb_ctx_t *ctx,
                               size_t index)
{
    if (index >= ctx->max_elements) {
        return 0;
    }
    return index * (ctx->element_size + 1);
}

void srb_init(srb_ctx_t *ctx,
              void *pool,
              size_t pool_size,
              size_t element_size)
{
    ctx->memory_pool = pool;
    ctx->element_size = element_size;
    ctx->max_elements = (pool_size / (element_size + 1));
    ctx->rd_idx = 0;
    ctx->wr_idx = 0;
    size_t i;
    for (i = 0; i < ctx->max_elements; ++i) {
        size_t offset = get_offset_bytes(ctx, i);
        *(((uint8_t *) ctx->memory_pool) + offset) = INVALID_FLAG;
    }
}

bool srb_push(srb_ctx_t *ctx,
              const void *element)
{
    if (srb_is_full(ctx)) {
        return false;
    }
    size_t offset = get_offset_bytes(ctx, ctx->wr_idx);
    memcpy(((uint8_t *) ctx->memory_pool) + offset + 1, element, ctx->element_size);
    *(((uint8_t *) ctx->memory_pool) + offset) = VALID_FLAG;
    if ( ++(ctx->wr_idx) >= ctx->max_elements) {
        ctx->wr_idx = 0;
    }
    return true;
}

bool srb_is_full(const srb_ctx_t *ctx)
{
    size_t offset = get_offset_bytes(ctx, ctx->wr_idx);
    if ( *(((uint8_t *) ctx->memory_pool) + offset) == VALID_FLAG) {
        return true;
    } else {
        return false;
    }
}

bool srb_is_empty(const srb_ctx_t *ctx)
{
    size_t offset = get_offset_bytes(ctx, ctx->rd_idx);
    if ( *(((uint8_t *) ctx->memory_pool) + offset) == VALID_FLAG) {
        return false;
    } else {
        return true;
    }
}

bool srb_pop(srb_ctx_t *ctx,
             void *element)
{
    if (srb_is_empty(ctx)) {
        return false;
    }
    size_t offset = get_offset_bytes(ctx, ctx->rd_idx);
    memcpy(element, ((uint8_t *)ctx->memory_pool) + offset + 1, ctx->element_size);
    *(((uint8_t *) ctx->memory_pool) + offset) = INVALID_FLAG;
    if ( ++(ctx->rd_idx) >= ctx->max_elements) {
        ctx->rd_idx = 0;
    }
    return true;
}

bool srb_peek(const srb_ctx_t *ctx,
              void *element)
{
    if (srb_is_empty(ctx)) {
        return false;
    }
    size_t offset = get_offset_bytes(ctx, ctx->rd_idx);
    memcpy(element, ctx->memory_pool + offset + 1, ctx->element_size);
    return true;
}
