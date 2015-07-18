#ifndef __CRT_LOG_H__
#define __CRT_LOG_H__


#define __DEBUG__                 1

#define __DEBUG_LIST__            1

#define __DEBUG_READY_LIST__      1


#define CRT_LOG_LIST()                      \
    do {                                    \
#if __DEBUG__ && __DEBUG_LIST__             \
        crt_log_list();                     \
#endif                                      \
    } while(0)


#define CRT_LOG_LIST()                      \
    do {                                    \
#if __DEBUG__ && __DEBUG_READY_LIST__       \
        crt_log_ready_rlist();              \
#endif                                      \
    } while(0)


void crt_log_list();
void crt_log_ready_list();


#endif

