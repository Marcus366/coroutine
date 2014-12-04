#include <stdlib.h>
#include "context.h"
#include "coroutine.h"


hlist_head g_coroutine_map[1024];


static int
hash_int(int val)
{
    return val % 1024;
}


coroutine_ctx_t*
coroutine_get_ctx(coroutine_t cid)
{
    int index;
    coroutine_ctx_t *ctx;

    ctx = NULL;
    index = hash_int(cid);
    hlist_for_each_entry(ctx, &g_coroutine_map[index], hash) {
        if (ctx->cid == cid) {
            break;
        }
    }
    return ctx;
}


void
coroutine_set_ctx(coroutine_t cid, coroutine_ctx_t *ctx)
{
    int index;

    ctx->cid = cid;
    index = hash_int(cid);
    hlist_add_head(&ctx->hash, &g_coroutine_map[index]);
}

