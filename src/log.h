#ifndef __CRT_LOG_H__
#define __CRT_LOG_H__


#define __DEBUG__ 1

#define __DEBUG_LIST__ 1

#define __DEBUG_READY_LIST__ 1


#if __DEBUG__ && __DEBUG_LIST__
#define CRT_LOG_LIST do {                   \
        crt_log_list();                     \
    } while(0)
#endif

#if __DEBUG__ && __DEBUG_READY_LIST__
#define CRT_LOG_READY_LIST do {             \
        crt_log_ready_list();               \
    } while(0)
#endif

#ifndef CRT_LOG_LIST
#define CRT_LOG_LIST
#endif

#ifndef CRT_LOG_READY_LIST
#define CRT_LOG_READY_LIST
#endif


void crt_log_list();
void crt_log_ready_list();


#endif

