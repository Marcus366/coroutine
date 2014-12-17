#include <stdlib.h>
#include "context.h"
#include "coroutine.h"


hlist_head g_coroutine_map[1024];
list_head  g_coroutine_list = LIST_HEAD_INIT(g_coroutine_list);
list_head  g_coroutine_ready_list = LIST_HEAD_INIT(g_coroutine_ready_list);

coroutine_ctx_t *g_coroutine_running_ctx;

