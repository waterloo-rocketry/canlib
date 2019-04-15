#include "safe_ring_buffer.h"
#include <string.h>
#include <stdint.h>

static size_t get_offset_bytes(const srb_ctx_t *ctx,
                               size_t index)
{
    if (index >= ctx->max_elements) {
        return 0;
    }
    return index * (ctx->element_size);
}

void srb_init(srb_ctx_t *ctx,
              void *pool,
              size_t pool_size,
              size_t element_size)
{
    ctx->memory_pool = pool;
    ctx->element_size = element_size;
    ctx->max_elements = (pool_size / (element_size));
    ctx->rd_idx = 0;
    ctx->wr_idx = 0;
}

bool srb_push(srb_ctx_t *ctx,
              const void *element)
{
    if (srb_is_full(ctx)) {
        return false;
    }
    size_t offset = get_offset_bytes(ctx, ctx->wr_idx);
    memcpy(((uint8_t *) ctx->memory_pool) + offset, element, ctx->element_size);
    if ( ++(ctx->wr_idx) >= ctx->max_elements) {
        ctx->wr_idx = 0;
    }
    return true;
}

bool srb_is_full(const srb_ctx_t *ctx)
{
    if ((ctx->wr_idx + 1 == ctx->rd_idx) || (ctx->wr_idx + 1 == ctx->max_elements && ctx->rd_idx == 0)) {
        return true;
    } else {
        return false;
    }
}

bool srb_is_empty(const srb_ctx_t *ctx)
{
    if (ctx->wr_idx == ctx->rd_idx) {
        return true;
    } else {
        return false;
    }
}

bool srb_pop(srb_ctx_t *ctx,
             void *element)
{
    if (srb_is_empty(ctx)) {
        return false;
    }
    size_t offset = get_offset_bytes(ctx, ctx->rd_idx);
    memcpy(element, ((uint8_t *)ctx->memory_pool) + offset, ctx->element_size);
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
    memcpy(element, ctx->memory_pool + offset, ctx->element_size);
    return true;
}
