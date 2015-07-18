#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

#include "log.h"
#include "context.h"
#include "list.h"

void
crt_log_list()
{
    crt_ctx_t *ctx;

    printf("%s:\n", __func__);
    list_for_each_entry(ctx, &g_crt_list, list) {
        printf("%" PRIu64 " ", ctx->cid);
        switch (ctx->flag) {
            case RUNNING:
                printf("running\n");
                if (!list_is_suspend(&ctx->queue)) {
                    printf("state not consistent, running but in queue\n");
                    exit(-1);
                }
                break;
            case BLOCKING:
                printf("blocking\n");
                if (list_is_suspend(&ctx->queue)) {
                    printf("state not consistent blocking but not in queue\n");
                    exit(-1);
                }
                break;
            case READY:
                printf("ready\n");
                if (list_is_suspend(&ctx->queue)) {
                    printf("state not consistent ready but not in queue\n");
                    exit(-1);
                }
                break;
            default:
                printf("\n");
        }
    }
}

void
crt_log_ready_list()
{
    crt_ctx_t *ctx;

    printf("%s:\n", __func__);
    list_for_each_entry(ctx, &g_crt_ready_list, queue) {
        printf("%" PRIu64 " ", ctx->cid);
        switch (ctx->flag) {
            case RUNNING:
                printf("state not consistent running\n");
                exit(-1);
            case BLOCKING:
                printf("state not consistent blocking\n");
                exit(-1);
            default:
                break;
        }
    }
    printf("\n");
}

